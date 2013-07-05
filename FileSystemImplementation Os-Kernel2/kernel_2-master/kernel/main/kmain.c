#include "types.h"
#include "globals.h"
#include "kernel.h"

#include "util/gdb.h"
#include "util/init.h"
#include "util/debug.h"
#include "util/string.h"
#include "util/printf.h"

#include "mm/mm.h"
#include "mm/page.h"
#include "mm/pagetable.h"
#include "mm/pframe.h"

#include "vm/vmmap.h"
#include "vm/shadow.h"
#include "vm/anon.h"

#include "main/acpi.h"
#include "main/apic.h"
#include "main/interrupt.h"
#include "main/cpuid.h"
#include "main/gdt.h"

#include "proc/sched.h"
#include "proc/proc.h"
#include "proc/kthread.h"

#include "drivers/dev.h"
#include "drivers/blockdev.h"
#include "drivers/tty/virtterm.h"

#include "api/exec.h"
#include "api/syscall.h"

#include "fs/vfs.h"
#include "fs/vnode.h"
#include "fs/vfs_syscall.h"
#include "fs/fcntl.h"
#include "fs/stat.h"

#include "test/kshell/kshell.h"

GDB_DEFINE_HOOK(boot)
GDB_DEFINE_HOOK(initialized)
GDB_DEFINE_HOOK(shutdown)

static void      *bootstrap(int arg1, void *arg2);
static void      *idleproc_run(int arg1, void *arg2);
static kthread_t *initproc_create(void);
static void      *initproc_run(int arg1, void *arg2);
static void       hard_shutdown(void);

static context_t bootstrap_context;
static context_t idleproc_context;

extern int vfstest_main(int argc, char **argv);
void printHeader(char *headerText)
{
	dbg_print("\n*************************************************************** \n");
	dbg_print("\t\tKernel1:Header:"); dbg_print(headerText, 0); dbg_print("\t");
	dbg_print("\n*************************************************************** \n");
}

void printTCHeader(char *testCase, char *desc)
{
	dbg_print("\n*************************************************************** \n");
	dbg_print("\t\tKernel1:Header:"); dbg_print(testCase, 0); dbg_print("\t");
	dbg_print("\nKernel1:Desc:"); dbg_print(desc, 0); dbg_print("\t");
	dbg_print("\n*************************************************************** \n");
}

/**
 * This is the first real C function ever called. It performs a lot of
 * hardware-specific initialization, then creates a pseudo-context to
 * execute the bootstrap function in.
 */
void
kmain()
{
        GDB_CALL_HOOK(boot);

        dbg_init();
        dbgq(DBG_CORE, "Kernel binary:\n");
        dbgq(DBG_CORE, "  text: 0x%p-0x%p\n", &kernel_start_text, &kernel_end_text);
        dbgq(DBG_CORE, "  data: 0x%p-0x%p\n", &kernel_start_data, &kernel_end_data);
        dbgq(DBG_CORE, "  bss:  0x%p-0x%p\n", &kernel_start_bss, &kernel_end_bss);

        page_init();

        pt_init();
        slab_init();
        pframe_init();

        acpi_init();
        apic_init();
        intr_init();

        gdt_init();

        /* initialize slab allocators */
#ifdef __VM__
        anon_init();
        shadow_init();
#endif
        vmmap_init();
        proc_init();
        kthread_init();

#ifdef __DRIVERS__
        bytedev_init();
        blockdev_init();
#endif

        void *bstack = page_alloc();
        pagedir_t *bpdir = pt_get();
        KASSERT(NULL != bstack && "Ran out of memory while booting.");
        context_setup(&bootstrap_context, bootstrap, 0, NULL, bstack, PAGE_SIZE, bpdir);
        context_make_active(&bootstrap_context);

        panic("\nReturned to kmain()!!!\n");
}

/**
 * This function is called from kmain, however it is not running in a
 * thread context yet. It should create the idle process which will
 * start executing idleproc_run() in a real thread context.  To start
 * executing in the new process's context call context_make_active(),
 * passing in the appropriate context. This function should _NOT_
 * return.
 *
 * Note: Don't forget to set curproc and curthr appropriately.
 *
 * @param arg1 the first argument (unused)
 * @param arg2 the second argument (unused)
 */
static void *
bootstrap(int arg1, void *arg2)
{
	printHeader("BOOTSTRAP FUNCTION");
        /* necessary to finalize page table information */
        pt_template_init();
	
	/* create idle proc*/
	curproc = proc_create("idle");
	KASSERT(NULL != curproc);
	KASSERT(PID_IDLE == curproc->p_pid);

	/* create idle thread */
	curthr = kthread_create(curproc,idleproc_run,0,NULL);
	KASSERT(NULL != curthr);

	dbg_print("Kernel1:SysMsg:  idle proc/thrd created. switching context.\n");
	
	/* switch to idleproc_run thread context */
        context_make_active(&curthr->kt_ctx);

        panic("weenix returned to bootstrap()!!! BAD!!!\n");
        return NULL;
}

/**
 * Once we're inside of idleproc_run(), we are executing in the context of the
 * first process-- a real context, so we can finally begin running
 * meaningful code.
 *
 * This is the body of process 0. It should initialize all that we didn't
 * already initialize in kmain(), launch the init process (initproc_run),
 * wait for the init process to exit, then halt the machine.
 *
 * @param arg1 the first argument (unused)
 * @param arg2 the second argument (unused)
 */
static void *
idleproc_run(int arg1, void *arg2)
{
	printHeader("IDLE PROCESS");

        int status;
        pid_t child;

        /* create init proc */
        kthread_t *initthr = initproc_create();

        init_call_all();
        GDB_CALL_HOOK(initialized);

        /* Create other kernel threads (in order) */

#ifdef __VFS__
        /* Once you have VFS remember to set the current working directory
         * of the idle and init processes */
	curproc->p_cwd = vfs_root_vn;
	initthr->kt_proc->p_cwd = vfs_root_vn;
	vref(vfs_root_vn);
	/*Only 1 vref to skip pagoutd which has already inc in proc_create*/

        /* Here you need to make the null, zero, and tty devices using mknod */
        /* You can't do this until you have VFS, check the include/drivers/dev.h
         * file for macros with the device ID's you will need to pass to mknod */
	do_mkdir("/dev");
	do_mknod("/dev/null",S_IFCHR, MKDEVID(1,0)); 
	do_mknod("/dev/zero",S_IFCHR, MKDEVID(1,1));
	do_mknod("/dev/tty0",S_IFCHR, MKDEVID(2,0)); 
	
#endif

        /* Finally, enable interrupts (we want to make sure interrupts
         * are enabled AFTER all drivers are initialized) */
        intr_enable();

        /* Run initproc */
        sched_make_runnable(initthr);

	dbg_print("Kernel1:SysMsg:  init proc/thrd created. idle_proc waiting for init_proc.\n");

        /* Now wait for it */
        child = do_waitpid(-1, 0, &status);

	dbg_print("Kernel1:SysMsg:  init_proc joined idle_proc.\n");

        KASSERT(PID_INIT == child);

#ifdef __MTP__
        kthread_reapd_shutdown();
#endif


#ifdef __VFS__
        /* Shutdown the vfs: */
        dbg_print("weenix: vfs shutdown...\n");
        vput(curproc->p_cwd);
        dbg_print("weenix: vfs vput returned...\n");
        if (vfs_shutdown())
                panic("vfs shutdown FAILED!!\n");

#endif

        /* Shutdown the pframe system */
#ifdef __S5FS__
        pframe_shutdown();
#endif

        dbg_print("\nweenix: halted cleanly!\n");
        GDB_CALL_HOOK(shutdown);
        hard_shutdown();
        return NULL;
}

/**
 * This function, called by the idle process (within 'idleproc_run'), creates the
 * process commonly refered to as the "init" process, which should have PID 1.
 *
 * The init process should contain a thread which begins execution in
 * initproc_run().
 *
 * @return a pointer to a newly created thread which will execute
 * initproc_run when it begins executing
 */
static kthread_t *
initproc_create(void)
{
	/*----Kernel1:PROCS:initproc_create:Begins---*/
	/* create init proc and assert */
	proc_t *init_proc = proc_create("init");
	KASSERT(NULL != init_proc);
	KASSERT(PID_INIT == init_proc->p_pid);

	/* create init thread and assert */
	kthread_t *init_thr = kthread_create(init_proc,initproc_run,0,NULL);
	KASSERT(init_thr != NULL);

        return init_thr;
	/*----Ends----*/
}

/**********************************************************************************
			TEST CASE FUNCTIONS BEGIN 
***********************************************************************************/
kmutex_t m1;
kmutex_t m2;
kmutex_t pc_mutex;
ktqueue_t prod_wait_queue;
ktqueue_t cons_wait_queue;
int PC_MAX = 10;
int pc_buffer = 0;

/*---Test Case 1: Print PIDs-----*/
static void*
test1_pid_print_function(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC1:\t New Proc/Thread Running.PID of current process = %d\n",curproc->p_pid);
	return NULL;

}

/*---Test Case 2: Lock Mutex for KT_SLEEP_CANCELLABLE-----*/
static void*
test2_mutex_function(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC2:\t Mutex about to be attempted on PID = %d\n",curproc->p_pid);
	kmutex_lock_cancellable(&m1);
	KASSERT( 0 && "should NOT have reached here");

	return NULL;
}

static void*
test2_proc_kill_all_function(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC2: proc_kill_all about to happen in PID = %d\n",curproc->p_pid);
	proc_kill_all();
	KASSERT( 0 && "should NOT have reached here");

	return NULL;
}

/*---Test Case 3: Lock Mutex for KT_RUN-----*/
static void*
test3_mutex_function(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC3:\t Mutex about to be attempted on PID = %d\n",curproc->p_pid);
	kmutex_lock_cancellable(&m1);
	KASSERT( 0 && "should NOT have reached here");

	return NULL;
}


/*---Test Case 4: Deadlock Scenario-----*/
static void*
test4_mutex_deadlock_func_1(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC4:\t Thread X; Trying to Acquire Mutex 1; PID = %d\n",curproc->p_pid);
	kmutex_lock(&m1);
		dbg_print("Kernel1:TC4:\t Thread X; Mutex 1 Acquired; PID = %d\n",curproc->p_pid);
		dbg_print("Kernel1:TC4:\tSwitching Context\n");
		sched_make_runnable(curthr);
		sched_switch();
		dbg_print("Kernel1:TC4:\t Thread X; Trying to Acquire Mutex 2; PID = %d\n",curproc->p_pid);
		kmutex_lock_cancellable(&m2);
			dbg_print("Kernel1:TC4:\t Thread X; Should Not Reach Here; Mutex 2 Acquired; PID = %d\n",curproc->p_pid);
		kmutex_unlock(&m2);
	kmutex_unlock(&m1);

	return NULL;
}

static void*
test4_mutex_deadlock_func_2(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC4:\t Thread Y; Trying to Acquire Mutex 2; PID = %d\n",curproc->p_pid);
	kmutex_lock(&m2);
		dbg_print("Kernel1:TC4:\t Thread Y; Mutex 2 Acquired; PID = %d\n",curproc->p_pid);
		dbg_print("Kernel1:TC4:\t Switching Context\n");
		sched_make_runnable(curthr);
		sched_switch();
		dbg_print("Kernel1:TC4:\t Thread Y; Trying to Acquire Mutex 1; PID = %d\n",curproc->p_pid);
		kmutex_lock_cancellable(&m1);
			dbg_print("Kernel1: Thread Y; Should Not Reach Here; Mutex 1 Acquired; PID = %d\n",curproc->p_pid);			
		kmutex_unlock(&m1);
	kmutex_unlock(&m2);

	return NULL;
}


/*---Test Case 5: Producer-Consumer-----*/
static void* pc_producer(int arg1, void *arg2)
{
	int i;
		
	dbg_print("\nKernel1:TC5:Producer: START OF PRODUCER\n");

	for(i=1; i<=PC_MAX; i++)
	{
		dbg_print("\nKernel1:TC5:Producer: Iteration for element: %d\n",i);
		kmutex_lock(&pc_mutex);	
		dbg_print("Kernel1:TC5:Producer: Acquired pc_mutex. \n");	
		while(pc_buffer!=0)
		{
			dbg_print("Kernel1:TC5:Producer: Buffer is filled. \n");
			kmutex_unlock(&pc_mutex);
			dbg_print("Kernel1:TC5:Producer: Relinquish pc_mutex and goes to sleep. \n");	
			sched_sleep_on(&prod_wait_queue);
			dbg_print("\nKernel1:TC5:Producer: Wakes up and acquires mutex. \n");	
			kmutex_lock(&pc_mutex);
		}
		pc_buffer=i;
		dbg_print("Kernel1:TC5:Producer: Adds element to buffer; WakeUp Consumer. \n");	
		sched_wakeup_on(&cons_wait_queue);		
		kmutex_unlock(&pc_mutex);
		dbg_print("Kernel1:TC5:Producer: Relinquish pc_mutex. \n");	
	}

	dbg_print("Kernel1:TC5:Producer: END OF PRODUCER\n");
	return NULL;
}


static void* pc_consumer(int arg1, void *arg2)
{
	int i;
		
	dbg_print("\nKernel1:TC5:Consumer: START OF CONSUMER\n");

	for(i=1; i<=PC_MAX; i++)
	{
		dbg_print("\nKernel1:TC5:Consumer: Iteration for element: %d\n",i);
		kmutex_lock(&pc_mutex);
		dbg_print("Kernel1:TC5:Consumer: Acquired pc_mutex. \n");			
		while(pc_buffer==0)
		{
			dbg_print("Kernel1:TC5:Consumer: Buffer is empty. \n");			
			kmutex_unlock(&pc_mutex);
			dbg_print("Kernel1:TC5:Consumer: Relinquish pc_mutex and goes to sleep. \n");	
			sched_sleep_on(&cons_wait_queue);
			dbg_print("\nKernel1:TC5:Consumer: Wakes up and acquires mutex. \n");	
			kmutex_lock(&pc_mutex);
		}
		pc_buffer=0;
		dbg_print("Kernel1:TC5:Consumer: Consumes element from buffer; WakeUp Producer. \n");	
		sched_wakeup_on(&prod_wait_queue);
		kmutex_unlock(&pc_mutex);
		dbg_print("Kernel1:TC5:Consumer: Relinquish pc_mutex. \n");	
	}

	dbg_print("Kernel1:TC5:Consumer: END OF CONSUMER\n\n");
	return NULL;
}

/*---Test Case 6: Print PIDs and exit after being reparented-----*/
static void*
test6_pid_print_function(int arg1, void *arg2)
{
	dbg_print("Kernel1:TC6:\t New Proc/Thread Running. PID %d, parent PID %d\n",curproc->p_pid,curproc->p_pproc->p_pid);
	return NULL;

}
static void*
test6_reparent_children(int arg1, void *arg2)
{
	int i;
	proc_t *proc;
	kthread_t *thr;

	dbg_print("\nKernel1:TC6:Reparent: CREATE CHILDREN of PID %d\n", curproc->p_pid);
	for (i=0; i<20; i++)
	{
		proc = proc_create("pid_print_exit");
		thr = kthread_create(proc,test6_pid_print_function,0,NULL);
		dbg_print("Kernel1:TC6: Child created PID %d, parent PID %d\n",proc->p_pid,proc->p_pproc->p_pid);
		sched_make_runnable(thr);
	}

	do_exit(0);
	KASSERT( 0 && "reparent_children should not reach here");
	return NULL;

}

/*---Test: KShell-----*/
static void*
test_kshell(int arg1, void *arg2)
{
	/* Create a kshell on a tty */ 
	int err = 0; 
	kshell_t *ksh = kshell_create(0); 
	KASSERT(ksh && "did not create a kernel shell as expected");
	while ((err = kshell_execute_next(ksh)) > 0);
	KASSERT(err == 0 && "kernel shell exited with an error\n");
 	kshell_destroy(ksh);

	return NULL;

}

/*---Test: Full VFS Test-----*/
static void*
test_vfs(int arg1, void *arg2)
{
	vfstest_main(1,NULL);
	return NULL;
}

/*---Test Case 8: Rename
	Make Dir. CD into it and make a file 'F1'. Then Rename it 'F2'-----*/
static void*
test8_vfs(int arg1, void *arg2)
{
	do_mkdir("TestCase8");
	do_chdir("TestCase8");
	do_open("F1.txt",O_CREAT);
	do_rename("F1.txt","F2.txt");
	return NULL;
}

/*---Test Case 9: Do_Dup
	Make Dir. CD into it Open a file 'F1'. Then Duplicate it's FD-----*/
static void*
test9_vfs(int arg1, void *arg2)
{
	do_mkdir("TestCase9");
	do_chdir("TestCase9");
	do_open("F1.txt",O_CREAT);

	int fd=0;
	while(curproc->p_files[fd] != NULL)
		fd++;
	dbg_print("Kernel2:TC9:Total Number of p_files in curproc before calling do_dup = %d\n",fd);	

	do_dup(fd-1);

	fd=0;
	while(curproc->p_files[fd] != NULL)
		fd++;
	
	dbg_print("Kernel2:TC9:Total Number of p_files in curproc after do_dup = %d\n",fd);	
	return NULL;
}

/*---Test Case 10: Do_Dup2
	Desc:Make Dir. CD into it Open a file 'F1'. Then Duplicate2 it's FD to a given FD-----*/
static void*
test10_vfs(int arg1, void *arg2)
{
	do_mkdir("TestCase10");
	do_chdir("TestCase10");
	do_open("F1.txt",O_CREAT);

	int fd=0;
	while(curproc->p_files[fd] != NULL)
		fd++;
	dbg_print("Kernel2:TC10:Total Number of p_files in curproc before calling do_dup2 = %d\n",fd);	

	do_dup2(fd-1,fd);

	fd=0;
	while(curproc->p_files[fd] != NULL)
		fd++;
	
	dbg_print("Kernel2:TC10:Total Number of p_files in curproc after do_dup2 = %d\n",fd);	
	
	return NULL;
}


/*---Test Case 11: Link
	Make Dir. CD into it Open a file 'F1'. Then Link it with new file 'F2'-----*/
static void*
test11_vfs(int arg1, void *arg2)
{

	do_mkdir("TestCase11");
	do_chdir("TestCase11");
	do_open("F1.txt",O_CREAT);

	int fd=0;
	while(curproc->p_files[fd] != NULL)
		fd++;
	do_write(fd-1, "Hello", 5);

	do_link("F1.txt","F2.txt");
	return NULL;
}

/*---Test Case 12: UnLink
	Make Dir. CD into it Open a file 'F1'. Then Link and Unlink it with new file 'F2'-----*/
static void*
test12_vfs(int arg1, void *arg2)
{

	do_mkdir("TestCase12");
	do_chdir("TestCase12");
	do_open("F1.txt",O_CREAT);

	int fd=0;
	while(curproc->p_files[fd] != NULL)
		fd++;
	do_write(fd-1, "Hello", 5);

	do_link("F1.txt","F2.txt");
	do_unlink("F2.txt");
	return NULL;
}

/*---Test Case 13: RmDir
	Make Dir. CD into it Make another Directory Temp. Then Remove It-----*/
static void*
test13_vfs(int arg1, void *arg2)
{

	do_mkdir("TestCase13");
	do_chdir("TestCase13");
	do_mkdir("Temp");
	do_rmdir("Temp");
	return NULL;
}

/**********************************************************************************
			TEST CASE FUNCTIONS END 
***********************************************************************************/

/**
 * The init thread's function changes depending on how far along your Weenix is
 * developed. Before VM/FI, you'll probably just want to have this run whatever
 * tests you've written (possibly in a new process). After VM/FI, you'll just
 * exec "/bin/init".
 *
 * Both arguments are unused.
 *
 * @param arg1 the first argument (unused)
 * @param arg2 the second argument (unused)
 */
static void *
initproc_run(int arg1, void *arg2)
{
	/*----Kernel1:PROCS:initproc_run:Begins---*/
        printHeader("INIT PROCESS");

        int status;
	int i;
        pid_t child;
	proc_t *proc;
	kthread_t *thr;


	/**************************************************************************************************
		TEST CASE  (Full VFSTest)
	**************************************************************************************************/
	printTCHeader("TEST CASE VFS", "Full 586 vfstest.c");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel2:VFS:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE VFS PASSED");



	/**************************************************************************************************
		TEST CASE 8 (Rename)
	**************************************************************************************************/
	printTCHeader("TEST CASE 8", "Rename Check Positive Scenarios");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test8_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:TC8:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE 8 PASSED");


	/**************************************************************************************************
		TEST CASE 9 (Do_Dup)
	**************************************************************************************************/
	printTCHeader("TEST CASE 9", "Do_Dup Check Positive Scnearios");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test9_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:TC9:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE 9 PASSED");

	/**************************************************************************************************
		TEST CASE 10 (Do_Dup2)
	**************************************************************************************************/
	printTCHeader("TEST CASE 10", "Do_Dup2 Check Positive Scenarios");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test10_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:TC10:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE 10 PASSED");


	/**************************************************************************************************
		TEST CASE 11 (Link)
	**************************************************************************************************/
	printTCHeader("TEST CASE 11", "Link Positive Scenarios");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test11_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:TC11:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE 11 PASSED");


	/**************************************************************************************************
		TEST CASE 12 (Unlink)
	**************************************************************************************************/
	printTCHeader("TEST CASE 12", "Unlink Check Positive Scenarios");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test12_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:TC12:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE 12 PASSED");


	/**************************************************************************************************
		TEST CASE 13 (Rmdir)
	**************************************************************************************************/
	printTCHeader("TEST CASE 13", "RmDir Check Positive Scnearios");

	proc = proc_create("VFS");
	thr = kthread_create(proc,test13_vfs,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:TC13:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE 13 PASSED");


	/**************************************************************************************************
		TEST CASE (Kshell Kill Test)
	**************************************************************************************************/
	printTCHeader("TEST CASE KSHELL", "Kshell kill test");

	proc = proc_create("Kshell");
	thr = kthread_create(proc,test_kshell,0,NULL);
	sched_make_runnable(thr);

	while( (child = do_waitpid(-1,0,&status)) > 0)
	{
        	dbg_print("Kernel1:KSHELL:\t initproc_run got child %d\n",child);
	}
        printHeader("TEST CASE KSHELL PASSED");

	/**********************************************************************************
		END OF TEST CASES
	***********************************************************************************/	
	
        return NULL;
	/*----Ends----*/
}

/**
 * Clears all interrupts and halts, meaning that we will never run
 * again.
 */
static void
hard_shutdown()
{
#ifdef __DRIVERS__
        vt_print_shutdown();
#endif
        __asm__ volatile("cli; hlt");
}
