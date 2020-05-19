#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__


/**
 * This defines the conditional variable structure
 */
typedef struct cond_var {
  struct process_state *next;
} cond_t;

/*
* This defines a linked likst of columns that represents
*   each column data in the LED matrix
*/
typedef struct columnFrame {
  char col[8]; //Values of the Columns in the matrix
  struct columnFrame *next; //Next Column in the matrix
} matCol;

/*
*Using a Linked List for the accelerometer data
*/
typedef struct accelData {
  float min;
  float max; //points to next thing in linked list
  float avg;
  int number;
} accelData;

#endif
