/*
 * File: table.c - strings
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Description: This file contains all the functions used in main for testing
 * text files with various string functions See function definitions for more
 * info
 *
 */

#include "set.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// defines SET structure
typedef struct set {
  int count;
  int length;
  char **data;
  char *flag;
} SET;

// defines search function to be used later
static int search(SET *sp, char *elt, bool *found);

SET *createSet(int maxElts){
  return(printf("\n\nCREATE SET\n\n"));
}

void destroySet(SET *sp){
  return(printf("\n\nDESTROY SET\n\n"));
}

int numElements(SET *sp){
  return(printf("\n\nNUM ELEMENTS"));
}

void addElement(SET *sp, char *elt){
  return(printf("\n\nADD ELEMENT\n\n"));
}

void removeElement(SET *sp, char *elt){
  return(printf("\n\nREMOVE ELEMENT\n\n"));
}

char *findElement(SET *sp, char *elt) {
  return(printf("\n\nFIND ELEMENT\n\n"));
}

char **getElements(SET *sp) {
  return (printf("\n\nGET ELEMENTS\n\n"));
}