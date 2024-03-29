#include "globals.h"
#include "errno.h"

#include "util/debug.h"

#include "proc/kthread.h"
#include "proc/kmutex.h"

/*
 * IMPORTANT: Mutexes can _NEVER_ be locked or unlocked from an
 * interrupt context. Mutexes are _ONLY_ lock or unlocked from a
 * thread context.
 */

void
kmutex_init(kmutex_t *mtx)
{
	/*----Kernel1:PROCS:kmutex_init:Begins---*/
	mtx->km_holder = NULL;
	sched_queue_init(&mtx->km_waitq);
	/*----Ends----*/
}

/*
 * This should block the current thread (by sleeping on the mutex's
 * wait queue) if the mutex is already taken.
 *
 * No thread should ever try to lock a mutex it already has locked.
 */
void
kmutex_lock(kmutex_t *mtx)
{
	/*----Kernel1:PROCS:kmutex_lock:Begins---*/
	KASSERT(mtx != NULL && "mutex does not exist");
	KASSERT(curthr && mtx->km_holder != curthr && "This thread has already locked the mutex");
	
	if(mtx->km_holder==NULL)	/* no holder case */
	{
		mtx->km_holder = curthr;
	}
	else				/* holder so wait case */
	{
		dbg_print("Kernel1: Current Thread waiting on mutex queue\n");
		sched_sleep_on(&mtx->km_waitq);
		mtx->km_holder = curthr;
	}
	KASSERT(mtx->km_holder == curthr && "thread didnt get mutex but returned anyway");
	/*----Ends----*/
}

/*
 * This should do the same as kmutex_lock, but use a cancellable sleep
 * instead.
 */
int
kmutex_lock_cancellable(kmutex_t *mtx)
{
	/*----Kernel1:PROCS:kmutex_lock_cancellable:Begins---*/
	KASSERT(mtx != NULL && "This mutex does not exist");
	KASSERT(curthr && mtx->km_holder != curthr && "This thread has already locked the mutex");

	/* no holder case */
	if(mtx->km_holder == NULL) 
	{
		mtx->km_holder = curthr;
		return 0;	/* not cancelled */
	}

	/* holder case so sleep, 0 if natural wake up, 1 if cancelled*/
	if ( sched_cancellable_sleep_on(&mtx->km_waitq) == -EINTR)
	{
		mtx->km_holder = NULL;
		return -EINTR; 
	}
	else
	{
		mtx->km_holder = curthr;
		return 0; 
	}
	/*----Ends----*/
}

/*
 * If there are any threads waiting to take a lock on the mutex, one
 * should be woken up and given the lock.
 *
 * Note: This should _NOT_ be a blocking operation!
 *
 * Note: Don't forget to add the new owner of the mutex back to the
 * run queue.
 *
 * Note: Make sure that the thread on the head of the mutex's wait
 * queue becomes the new owner of the mutex.
 *
 * @param mtx the mutex to unlock
 */
void
kmutex_unlock(kmutex_t *mtx)
{
	/*----Kernel1:PROCS:kmutex_unlock:Begins---*/
	KASSERT(mtx != NULL && "This mutex doesnt exist");
	KASSERT(curthr && mtx->km_holder == curthr && "This thread has not locked the mutex");

	/* handles both cases 1) empty queues, 2) someone waiting */
	mtx->km_holder = sched_wakeup_on(&mtx->km_waitq);
	KASSERT( curthr != mtx->km_holder && "Current thread was on waitq = BAD");
	/*----Ends----*/
}
