#include "shared_structs.h"

#ifndef __MatrixCol__
#define __MatrixCol__

matCol *createCol(char vals[8]); /*initializes a linked column list*/
void destroyCol(matCol  *matrixCol); /*Free the memory for a matrix column*/
matCol *shiftColIn(char vals[8], matCol *head); /*Shift in a column to a linked list of columns*/
#endif

