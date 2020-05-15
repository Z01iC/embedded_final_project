#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

/** Implement your structs here */

//Here is the enqueue delcaration so we can use it in lock.c
/**
 * This structure holds the process structure information
 */
struct process_state {
  //put another struct process
  unsigned int blocked; //0 if unblocked 1 if blocked
  unsigned int *sp;
	unsigned int *orig_sp;
  int stack_size;
  struct process_state *next;
};

/**
 * This defines the lock structure
 */
typedef struct lock_state {
	// This is the process that is currently taking up the lock right now
  int locked; //If 0 unlocked, if 1 is locked
  // Here are all the processes that have been removed from the process queue and are waiting for this lock
  struct process_state *next;
  //current process being locked
  struct process_state *curr;
} lock_t;

/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {
  struct process_state *next;
} cond_t;

void enqueue(struct process_state *proc);

#endif
