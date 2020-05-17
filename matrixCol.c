#include "matrixCol.h"
#include <stdlib.h>
#include <string.h>

/*initializes a linked column list*/
matCol *createCol(char vals[8]){
    matCol *newCol = malloc(sizeof(matCol));
    /*Malloc Failed*/
    if(newCol == NULL){
        return NULL;
    }
    strcpy(newCol->col, vals); /*Copy values from desired char array*/
    newCol->next=NULL;
    return newCol;
}

/*Destorys columns at the end of the linked list*/
void destroyCol(matCol	*matrixCol){
    /*If nothing is passed in*/
	if(matrixCol == NULL)	{
			return;
	}
    /*If this is the only thing in the linked list*/
	if(matrixCol->next == NULL){
			free(matrixCol); //Free the memory
			return;
	}
    /*Remove the last one and set the previous one's next to NULL*/
    matCol *curr = matrixCol;
    matCol *prev = NULL;
    while(curr->next!=NULL){
        prev = curr;
        curr = curr->next;
    }
    prev->next=NULL;
    free(curr); //free the memory
}

/*Shift in a column to a linked list of columns*/
matCol *shiftColIn(char vals [8], matCol *head){
    //creates a new col based on vals
    matCol *newCol = createCol(vals);
    //If Malloc Failed
    if(newCol == NULL){
        return NULL;
    }
    //sets the previous head to be the next in linked list 
    newCol->next=head;
    return newCol;
}