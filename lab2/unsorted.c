/*
 * File: unsorted.c
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
} SET;

// defines search function
static int search(SET *sp, char *elt);

// creates set, returns pointer to set
// O(1)
SET *createSet(int maxElts) {
  SET *sp;
  sp = malloc(sizeof(SET));
  assert(sp != NULL);
  sp->count = 0;
  sp->length = maxElts;
  sp->data = malloc(sizeof(char *) * maxElts);
  assert(sp->data != NULL);
  return sp;
}

// deletes set and frees all memory
// O(n), where n is number of elements in set (sp->count)
void destroySet(SET *sp) {
  assert(sp != NULL);
  int i;
  for (i = 0; i < sp->count; i++) {
    free(sp->data[i]);
  }
  // free data pointer
  free(sp->data);
  // free set
  free(sp);
}

// returns the number of elements in set
// O(1)
int numElements(SET *sp) {
  assert(sp != NULL);
  return (sp->count);
}

// adds an element to the set and updates number of elements as sp->count
// Worst case (not considering search function): O(1), where n is number of elements in set (sp->count)
// Worst case (considering search function): O(n)
void addElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  if (search(sp, elt) >= 0) {
    return;
  }
  assert(sp->count < sp->length);
  sp->data[sp->count] = strdup(elt);
  sp->count++;
  return;
}

// searches for a duplicate data in set, returns position of first duplicate or -1
// Worst case: O(n), where n is number of elements in set (sp->count)
static int search(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  int i;
  if (sp->count == 0) {
    return -1;
  }
  // linear search
  for (i = 0; i < sp->count; i++) {
    if (strcmp(elt, sp->data[i]) == 0) {
      return i;
    }
  }
  return -1;
}

// removes an element from the set and updates count
// Worst case (not considering search funciton): O(1), where n is number of elements in set (sp->count)
// Worst case (considering search function): O(n)
void removeElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  int pos = search(sp, elt);
  if (pos < 0) {
    return;
  } else {
    free(sp->data[pos]);
    // move last element to empty slot
    sp->data[pos] = sp->data[sp->count - 1];
    sp->count--;
    return;
  }
}

// looks for the element in set, returns its contents if found or NULL otherwise
// O(1) (not considering search)
// O(n) (considering search), where n is number of elements in set (sp->count)
char *findElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  int pos = search(sp, elt);
  if (pos >= 0) {
    return (sp->data[pos]);
  } else {
    return NULL;
  }
}

// copies all elements from set to a new memory location
// O(1)
char **getElements(SET *sp) {
  assert(sp != NULL);
  char **a;
  a = malloc(sizeof(char *) * sp->count);
  assert(a != NULL);
  return (memcpy(a, sp->data, sizeof(char *) * sp->count));
}
