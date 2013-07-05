#include "kernel.h"
#include "config.h"
#include "globals.h"
#include "errno.h"

#include "util/debug.h"
#include "util/list.h"
#include "util/string.h"
#include "util/printf.h"

#include "proc/kthread.h"
#include "proc/proc.h"
#include "proc/sched.h"
#include "proc/proc.h"

#include "mm/slab.h"
#include "mm/page.h"
#include "mm/mmobj.h"
#include "mm/mm.h"
#include "mm/mman.h"

#include "vm/vmmap.h"

#include "fs/vfs.h"
#include "fs/vfs_syscall.h"
#include "fs/vnode.h"
#include "fs/file.h"

proc_t *curproc = NULL; /* global */
static slab_allocator_t *proc_allocator = NULL;

static list_t _proc_list;
static proc_t *proc_initproc = NULL; /* Pointer to the init process (PID 1) */

void
proc_init()
{
	/*----Kernel1:PROCS:proc_init:Begins---*/
        list_init(&_proc_list);
        proc_allocator = slab_allocator_create("proc", sizeof(proc_t));
        KASSERT(proc_allocator != NULL);
	/*---Ends---*/
}

static pid_t next_pid = 0;

/**
 * Returns the next available PID.
 *
 * Note: Where n is the number of running processes, this algorithm is
 * worst case O(n^2). As long as PIDs never wrap around it is O(n).
 *
 * @return the next available PID
 */
static int
_proc_getid()
{
        proc_t *p;
        pid_t pid = next_pid;
        while (1) {
failed:
                list_iterate_begin(&_proc_list, p, proc_t, p_list_link) {
	/*dbg_print("\nKernel1: iterate %d\n",p->p_pid);*/
                        if (p->p_pid == pid) {
                                if ((pid = (pid + 1) % PROC_MAX_COUNT) == next_pid) {
                                        return -1;
                                } else {
                                        goto failed;
                                }
                        }
                } list_iterate_end();
                next_pid = (pid + 1) % PROC_MAX_COUNT;
                return pid;
        }
}

/*
 * The new process, although it isn't really running since it has no
 * threads, should be in the PROC_RUNNING state.
 *
 * Don't forget to set proc_initproc when you create the init
 * process. You will need to be able to reference the init process
 * when reparenting processes to the init process.
 */
proc_t *
proc_create(char *name)
{
	/*----Kernel1:PROCS:proc_create:Begins---*/
        proc_t* process = (proc_t*)slab_obj_alloc(proc_allocator);
	process->p_comm[256] = *name;
	process->p_pproc= curproc; 
	process->p_state = PROC_RUNNING;

	/* get pid and kassert from grading guidlines*/
	process->p_pid = _proc_getid();
	KASSERT(PID_IDLE != process->p_pid || list_empty(&_proc_list));
	KASSERT(PID_INIT != process->p_pid || PID_IDLE == curproc->p_pid);

        /*this is pointing to the pointing to the init  process	for reparrenting*/	
	if(process->p_pid == PID_INIT)
	{
		proc_initproc = process; 
	}
	
	/*set the page dir for this process*/
	process->p_pagedir = pt_create_pagedir(); 

	/*init process lists*/
	list_link_init(&process->p_list_link);
	list_link_init(&process->p_child_link);
	list_init(&(process->p_threads));
	list_init(&(process->p_children));

	/*add process onto kernel procs list*/
	list_insert_head(&_proc_list, &process->p_list_link);
	KASSERT(list_empty(&_proc_list) != 1 && "list NOT empty check");

	/* Insert process to parent process list if list is not null. */
	if(curproc != NULL)
	{
		list_insert_tail(&curproc->p_children, &process->p_child_link);
	}

	process->p_status = 0;

	/*Items to be added for PROCS*/
	sched_queue_init(&process->p_wait);

	/*Items for VFS*/
	process->p_cwd = vfs_root_vn;

	if (process->p_cwd)
	{
		vref(process->p_cwd);
	}

	int fd;
        for (fd = 0; fd < NFILES; fd++) {
		process->p_files[fd] = NULL;
        }


	/*Items to be inited for later projects*/
	process->p_brk = NULL;
	process->p_start_brk = NULL;
	process->p_vmmap = NULL;

	return process;
	/*---Ends---*/
}

/**
 * Cleans up as much as the process as can be done from within the
 * process. This involves:
 *    - Closing all open files (VFS)
 *    - Cleaning up VM mappings (VM)
 *    - Waking up its parent if it is waiting
 *    - Reparenting any children to the init process
 *    - Setting its status and state appropriately
 *
 * The parent will finish destroying the process within do_waitpid (make
 * sure you understand why it cannot be done here). Until the parent
 * finishes destroying it, the process is informally called a 'zombie'
 * process.
 *
 * This is also where any children of the current process should be
 * reparented to the init process (unless, of course, the current
 * process is the init process. However, the init process should not
 * have any children at the time it exits).
 *
 * Note: You do _NOT_ have to special case the idle process. It should
 * never exit this way.
 *
 * @param status the status to exit the process with
 */
void
proc_cleanup(int status)
{
	/*----Kernel1:PROCS:proc_cleanup:Begins---*/
        proc_t *proc;
	int fd = 0;

	/* Close all open file (VFS) */
	vput(curproc->p_cwd);

	for (fd = 0; fd < NFILES; fd++)
	{
		/*Check if file has already been closed by a dup fd*/
		if (curproc->p_files[fd] && curproc->p_files[fd]->f_refcount > 0)
		{	
			do_close(fd);
		}
        }

	
	/* Clean up VM mappings (VM */

	/* set state/status */
	curproc->p_state = PROC_DEAD;
	curproc->p_status = 0;
	KASSERT(1 <= curproc->p_pid);
	dbg_print("Kernel1:CleanUp: proc_cleanup() checking list empty\n");
	KASSERT(NULL != proc_initproc);
	KASSERT(NULL != curproc->p_pproc); 
	/*reparent all the processes children*/
	if (!list_empty(&curproc->p_children))
	{
		dbg_print("Kernel1:CleanUp: proc_cleanup() reparenting children of PID %d\n",curproc->p_pid);
        	list_iterate_begin(&curproc->p_children, proc ,proc_t,p_child_link){
   		dbg_print("Kernel1:CleanUp: proc_cleanup() reparenting child PID %d to PID %d\n",proc->p_pid,proc_initproc->p_pid);
		proc->p_pproc = proc_initproc;
                list_insert_tail(&proc_initproc->p_children, &proc->p_child_link);
        	}list_iterate_end();
	}

	/*wakeup waiting parrent*/
	dbg_print("Kernel1:CleanUp: proc_cleanup() waking up parent\n");
	sched_wakeup_on(&curproc->p_pproc->p_wait);
	sched_switch();
	/*---Ends---*/
}

static void*
function_to_force_context(int arg1, void *arg2)
{
	kthread_exit(0);
        return NULL;
}

/*
 * This has nothing to do with signals and kill(1).
 *
 * Calling this on the current process is equivalent to calling
 * do_exit().
 *
 * In Weenix, this is only called from proc_kill_all.
 */
void
proc_kill(proc_t *p, int status)
{
	/*----Kernel1:PROCS:proc_kill:Begins---*/
	kthread_t *thread;
	if(curproc == p)
	{
		do_exit(status);
	}
	else
	{
		/* bad ass forcibly remove threads from their waitq*/
		list_iterate_begin(&p->p_threads, thread ,kthread_t,kt_plink){
			kthread_cancel(thread,0);
        	}list_iterate_end();
	}
	/*----Ends---*/
}

/*
 * Remember, proc_kill on the current process will _NOT_ return.
 * Don't kill direct children of the idle process.
 *
 * In Weenix, this is only called by sys_halt.
 */
void
proc_kill_all()
{
	/*----Kernel1:PROCS:proc_kill_all:Begins---*/
	proc_t *proc;
	kthread_t *thread;
	list_iterate_begin(&_proc_list,proc,proc_t,p_list_link){
	if(proc->p_pid!=PID_IDLE && proc->p_pid != PID_INIT && proc->p_pid != 2 && proc != curproc)
	{
		dbg_print("Kernel1:CleanUp: calling proc_kill from PID %d on PID %d\n",curproc->p_pid,proc->p_pid);
		proc_kill(proc,0);
	}
	}list_iterate_end();
	
	/*kill the current proc after everybody except 0,1,2 are dead*/
	if (curproc->p_pid != PID_INIT)
	{
		do_exit(0);
	}
	/*----Ends---*/
}

proc_t *
proc_lookup(int pid)
{
        proc_t *p;
        list_iterate_begin(&_proc_list, p, proc_t, p_list_link) {
                if (p->p_pid == pid) {
                        return p;
                }
        } list_iterate_end();
        return NULL;
}

list_t *
proc_list()
{
        return &_proc_list;
}

/*
 * This function is only called from kthread_exit.
 *
 * Unless you are implementing MTP, this just means that the process
 * needs to be cleaned up and a new thread needs to be scheduled to
 * run. If you are implementing MTP, a single thread exiting does not
 * necessarily mean that the process should be exited.
 */
void
proc_thread_exited(void *retval)
{
	/*----Kernel1:PROCS:proc_thead_exited:Begins---*/
		dbg_print("Kernel1:CleanUp: proc_thread_exited()\n");
		proc_cleanup(0);
	/*----Ends---*/
}

/* If pid is -1 dispose of one of the exited children of the current
 * process and return its exit status in the status argument, or if
 * all children of this process are still running, then this function
 * blocks on its own p_wait queue until one exits.
 *
 * If pid is greater than 0 and the given pid is a child of the
 * current process then wait for the given pid to exit and dispose
 * of it.
 *
 * If the current process has no children, or the given pid is not
 * a child of the current process return -ECHILD.
 *
 * Pids other than -1 and positive numbers are not supported.
 * Options other than 0 are not supported.
 */
pid_t
do_waitpid(pid_t pid, int options, int *status)
{
	/*----Kernel1:PROCS:do_waitpid:Begins---*/
	proc_t *p;
	kthread_t *thr;

	/*If no children, error case*/
	if(list_empty(&curproc->p_children) == 1)
	{
		dbg_print("Kernel1:SysMsg:  do_waitpid() list empty\n");
		return -ECHILD;
	}

	/*Wait for ANY ONE of child processes to finish*/
	if(pid == -1)
	{
	
		while( 1 )
		{
			list_iterate_begin(&curproc->p_children, p,proc_t,p_child_link){
			
			KASSERT(-1 == pid || p->p_pid == pid);
			KASSERT(NULL != p && "the process should not be NULL");
			if(p->p_state==PROC_DEAD)
			{
				KASSERT(NULL != p->p_pagedir);
				*status = p->p_status;
				pid = p->p_pid;
				/*destory all process threads*/
			       dbg_print("Kernel1:SysMsg:  do_waitpid executing -1 kthread_destroy on %d\n",p->p_pid);
				list_iterate_begin(&p->p_threads,thr,kthread_t,kt_plink){
					kthread_destroy(thr);
					KASSERT(KT_EXITED == thr->kt_state);
				}list_iterate_end();
				list_remove(&p->p_child_link);
				list_remove(&p->p_list_link);
				return pid;
			}
			}list_iterate_end();
		
			dbg_print("Kernel1:SysMsg:  do_waitpid going to sleep \n");
			sched_sleep_on(&curproc->p_wait);
			dbg_print("Kernel1:SysMsg:  do_waitpid awakes\n");
		
		} 
		
	} else {

		/*PID > 0 specific process*/
		dbg_print("Kernel1:SysMsg:  BEGIN looking for PID %d\n",pid);
		list_iterate_begin(&curproc->p_children, p , proc_t, p_child_link){
		KASSERT(NULL != p && "the process should not be NULL");
		dbg_print("Kernel1:SysMsg:  looking for PID %d, found %d\n",pid,p->p_pid);
		if(p->p_pid == pid)
		{
			KASSERT(-1 == pid || p->p_pid == pid);
			KASSERT(NULL != p->p_pagedir);
			sched_sleep_on(&curproc->p_wait);
			*status = p->p_status;
			pid = p->p_pid;
			/*destory all process threads*/
			dbg_print("Kernel1:SysMsg:  do_waitpid executing PID kthread_destroy on %d\n",p->p_pid);
			list_iterate_begin(&p->p_threads,thr,kthread_t,kt_plink){
				kthread_destroy(thr);
				KASSERT(KT_EXITED == thr->kt_state);
			}list_iterate_end();
			list_remove(&p->p_child_link);
			list_remove(&p->p_list_link);
			return pid;
		}
		}list_iterate_end();
	}
	
	/*PID not found in child list*/
        return -ECHILD;
	/*----Ends---*/
}

/*
 * Cancel all threads, join with them, and exit from the current
 * thread.
 *
 * @param status the exit status of the process
 */
void
do_exit(int status)
{
	/*----Kernel1:PROCS:do_exit:Begins---*/
	kthread_cancel(curthr, 0);
	/*----Ends---*/
}

size_t
proc_info(const void *arg, char *buf, size_t osize)
{
        const proc_t *p = (proc_t *) arg;
        size_t size = osize;
        proc_t *child;

        KASSERT(NULL != p);
        KASSERT(NULL != buf);

        iprintf(&buf, &size, "pid:          %i\n", p->p_pid);
        iprintf(&buf, &size, "name:         %s\n", p->p_comm);
        if (NULL != p->p_pproc) {
                iprintf(&buf, &size, "parent:       %i (%s)\n",
                        p->p_pproc->p_pid, p->p_pproc->p_comm);
        } else {
                iprintf(&buf, &size, "parent:       -\n");
        }

#ifdef __MTP__
        int count = 0;
        kthread_t *kthr;
        list_iterate_begin(&p->p_threads, kthr, kthread_t, kt_plink) {
                ++count;
        } list_iterate_end();
        iprintf(&buf, &size, "thread count: %i\n", count);
#endif

        if (list_empty(&p->p_children)) {
                iprintf(&buf, &size, "children:     -\n");
        } else {
                iprintf(&buf, &size, "children:\n");
        }
        list_iterate_begin(&p->p_children, child, proc_t, p_child_link) {
                iprintf(&buf, &size, "     %i (%s)\n", child->p_pid, child->p_comm);
        } list_iterate_end();

        iprintf(&buf, &size, "status:       %i\n", p->p_status);
        iprintf(&buf, &size, "state:        %i\n", p->p_state);

#ifdef __VFS__
#ifdef __GETCWD__
        if (NULL != p->p_cwd) {
                char cwd[256];
                lookup_dirpath(p->p_cwd, cwd, sizeof(cwd));
                iprintf(&buf, &size, "cwd:          %-s\n", cwd);
        } else {
                iprintf(&buf, &size, "cwd:          -\n");
        }
#endif /* __GETCWD__ */
#endif

#ifdef __VM__
        iprintf(&buf, &size, "start brk:    0x%p\n", p->p_start_brk);
        iprintf(&buf, &size, "brk:          0x%p\n", p->p_brk);
#endif

        return size;
}

size_t
proc_list_info(const void *arg, char *buf, size_t osize)
{
        size_t size = osize;
        proc_t *p;

        KASSERT(NULL == arg);
        KASSERT(NULL != buf);

#if defined(__VFS__) && defined(__GETCWD__)
        iprintf(&buf, &size, "%5s %-13s %-18s %-s\n", "PID", "NAME", "PARENT", "CWD");
#else
        iprintf(&buf, &size, "%5s %-13s %-s\n", "PID", "NAME", "PARENT");
#endif

        list_iterate_begin(&_proc_list, p, proc_t, p_list_link) {
                char parent[64];
                if (NULL != p->p_pproc) {
                        snprintf(parent, sizeof(parent),
                                 "%3i (%s)", p->p_pproc->p_pid, p->p_pproc->p_comm);
                } else {
                        snprintf(parent, sizeof(parent), "  -");
                }

#if defined(__VFS__) && defined(__GETCWD__)
                if (NULL != p->p_cwd) {
                        char cwd[256];
                        lookup_dirpath(p->p_cwd, cwd, sizeof(cwd));
                        iprintf(&buf, &size, " %3i  %-13s %-18s %-s\n",
                                p->p_pid, p->p_comm, parent, cwd);
                } else {
                        iprintf(&buf, &size, " %3i  %-13s %-18s -\n",
                                p->p_pid, p->p_comm, parent);
                }
#else
                iprintf(&buf, &size, " %3i  %-13s %-s\n",
                        p->p_pid, p->p_comm, parent);
#endif
        } list_iterate_end();
        return size;
}
#include "kernel.h"
#include "config.h"
