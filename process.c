/***************************************************************************
 *
 *  Copyright (c) 2015 Cornell University
 *  Computer Systems Laboratory
 *  Cornell University, Ithaca, NY 14853
 *  All Rights Reservedw
 *
 *  $Id$
 *
 **************************************************************************
 */
#include <stdlib.h>
#include "3140_concur.h"
#include "lock.h"
/*
  State layout:
  .-----------------.
  |     xPSR        | <--- status register
  |-----------------|
  |      PC         | <--- starting point of the process's function
  |-----------------|
  |      LR         | <--- process_terminated
  |-----------------|
  |      R12        |
  |-----------------|
  |    R3 - R0      |
  |-----------------|
  |   0xFFFFFFF9    | <--- exception return value
  |-----------------|
  |    R4 - R11     |
  |-----------------|
  |    PIT State    |
  |-----------------|
  State requires 18 slots on the stack.
 */

//############################################################
//#########################MY CODE############################
//############################################################

//define Process queue and current process
process_t *process_queue = NULL;   //initially holds nothing, will populate later
process_t *current_process = NULL; //initially holds nothing, should hold running process later
/*------------------------------------------------------------------------
 *
 *  enqueue --
 *
 *   updates the queue to have input paramter (process_t *proc)
 *   at the head of the global process_queue
 *   Returns: Nothing
 *
 *------------------------------------------------------------------------
 */
void enqueue(process_t *proc)
{
  /*If the queue is empty, simply change process queue to be
 the inputted process*/
  if (process_queue == NULL)
  {
    process_queue = proc;
  }
  //case where there is already a process in the queue
  else
  {
    /*iterate through linked list where curr_proc represents
  the process we are examining at each iteration*/
    process_t *curr_proc = process_queue;
    while (curr_proc->next != NULL)
    {
      curr_proc = curr_proc->next;
    }
    /*update the last process in the queue to point to the
  enqueued process, and ensure the enqueued process
  is the tail and points to NULL*/
    curr_proc->next = proc;
    proc->next = NULL;
  }
}
/*------------------------------------------------------------------------
 *
 *  dequeue --
 *
 *   Removes the head of the global queue (process_queue)
 *   Returns: the head of the queue
 *
 *------------------------------------------------------------------------
 */
process_t *dequeue()
{
  //if there is nothing in the queue, return NULL
  if (process_queue == NULL)
  {
    return NULL;
  }
  //update process queue to be second element in list
  //return the head of the queue
  else
  {
    process_t *temp = process_queue;
    process_queue = process_queue->next;
    return temp;
  }
}
/*------------------------------------------------------------------------
 *
 *  process_create --
 *
 *   creates a new process that exectutes input function (*f) of size (int n)
 *   enqueues that new process to the global queue process_queue
 *   Returns: -1 if there was a failure to create the process, 0 otherwise
 *
 *------------------------------------------------------------------------
 */
int process_create(void (*f)(void), int n)
{
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq(); //temporarily disable interrupts
	
  //uses built in function to init space in stack for f
  unsigned int *stackPointer = process_stack_init(f, n);
  if (stackPointer == NULL)
  {
		__set_PRIMASK(m);
    return -1;
  }

  //create new process and free up stack space for it
  process_t *newProcess = malloc(sizeof(process_t));
	if (newProcess == NULL)
	{
		__set_PRIMASK(m);
		return -1;
	}
  //initialize fields of new struct
	newProcess->blocked = 0;
	newProcess->sp = stackPointer;
  newProcess->stack_size = n;
  newProcess->next = NULL;
	newProcess->orig_sp = stackPointer;
  //add new process to the end of the queue
  enqueue(newProcess);
	__set_PRIMASK(m);
  return 0;
}

/*------------------------------------------------------------------------
 *
 *  process_start --
 *
 *   Set up period to be generated using PIT[0] iinterrupt
 *   Ends with a call to process_begin() to start the first process
 *   Returns: Nothing
 *
 *------------------------------------------------------------------------
 */
void process_start(void)
{
  //set up PIT timer but DON'T start it
  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 2) */
  SIM->SCGC6 = SIM_SCGC6_PIT_MASK;
  //load a reasonable timing value for the interrupts
  PIT->CHANNEL[0].LDVAL = 0x01FFF;
  PIT->MCR = 0x0;
  PIT->CHANNEL[0].TCTRL |= (1 << 1);

  // call to process begin
  process_begin();
}
/*------------------------------------------------------------------------
 *
 *  process_select --
 *
 *   Selects the next process in the global process_queue to run
 *   Updates the previous process sp to be input (unsigned int *cursp)
 *   enqueues the previous process to the end of the global process_queue
 *   Returns: The stack pointer of the next process to run, or NULL
 *
 *------------------------------------------------------------------------
 */
unsigned int *process_select(unsigned int *cursp)
{
  //process_t *returner = process_queue;
  //update the sp in process control block
  //add to the queue
  //grab something else from the queue
	uint32_t m;
	m = __get_PRIMASK();
	__disable_irq(); //temporarily disable interrupts
	//We don't want our program to get interrupted while selecting a prcoess!
	
  //Case where current process has finished running!
  if (cursp == NULL)
  {
    //if there is a current process, then delete that process by clearing the stack
    if (current_process != NULL)
    {
      free(current_process);
      process_stack_free(current_process->orig_sp, current_process->stack_size);
    }
    //if there are no processes left in the queue, return null
    if (process_queue == NULL)
    {
			__set_PRIMASK(m);
      return NULL;
    }
    //if there are processes left in the queue, update current process
    //but do NOT enqueue, because the previous process finished
    else
    {
      //checks the next process in the queue, and only returns if the process
      //is not blocked
      current_process = dequeue();
			__set_PRIMASK(m);
      return current_process->sp;
    }
  }

  //case where cursp!=NULL, or the current process still needs to run
  else
  {
    //update current process sp to where it left off
    current_process->sp = cursp;
    //enqueue current process to the end of the queue
    current_process->next = NULL;
		if(!current_process->blocked){ //if this thing got blocked then don't put it back in the process_queue
			enqueue(current_process);
		}

    //dequeue from the queue, if the process is blocked then continue
    //dequeue until the process is not blocked! then return sp
    current_process = dequeue();
		__set_PRIMASK(m);
    return current_process->sp;
  }
}
