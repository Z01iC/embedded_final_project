#include "lock.h"

/**
 * Initialises the lock structure
 *
 * @param l pointer to lock to be initialised
 */
void l_init(lock_t *l)
{
  l->locked = 0;
  l->next = NULL;
	l->curr = NULL;
	
}

//Find the last process in the blocked queue in our lock.
//Add process "t" to that process' next_blocked field
//Maintains "next_blocked" field for a process_t
//Maintains "next" linked list for a lock
void lockQueueEnd(process_t *t, process_t *head)
{
  while (head->next != NULL)
  {
    head = head->next;
  }
  head->next = t;
	t->next= NULL; //should be null initialized, but for redundancy
}

/**
 * Grab the lock or block the process until lock is available
 *
 * @param l pointer to lock to be grabbed
 */
void l_lock(lock_t *l)
{
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq(); //temporarily disable interrupts
  //if this lock is already locked
  if (l->locked==1)
  {
    //update the process that is being placed at the end of the lock queue
    current_process->next = NULL;
		current_process->blocked = 1;
    //if there is no next process, updates next to be current process
    if (l->next == NULL)
    {
      l->next = current_process;
    }
    //if there is a next process already, go into the process and add
    //current process to the end of the blocked queue
    else
    {
      lockQueueEnd(current_process, l->next);
    }
		process_blocked();
  }
  else
  {
		current_process->blocked = 0; //for protection
    l->locked = 1;
		l->next = NULL;
    l->curr = current_process;
  }
	__set_PRIMASK(m);
}

/**
 * Release the lock along with the first process that may be waiting on
 * the lock. This ensures fairness wrt lock acquisition.
 *
 * @param l pointer to lock to be unlocked
 */
void l_unlock(lock_t *l)
{
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq(); //temporarily disable interrupts
  if (l->next == NULL)
  {                                
    //update the lock to be unlocked
    l->locked = 0;
    l->curr = NULL;
  }
  else if (l->next != NULL)
  {
    //update the current lock being blocked to be the next in the queue
    l->curr = l->next;
    //update the next in the queue, to be what is after the current block - can be NULL
    l->next = l->curr->next;
		l->curr->next = NULL;
		l->curr->blocked = 0;
		enqueue(l->curr);
  }
	__set_PRIMASK(m);
}
