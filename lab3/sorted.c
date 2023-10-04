/*
 * File: sorted.c
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
static int search(SET *sp, char *elt, bool *found);

// creates set, returns pointer to set
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

// frees all set memory
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
int numElements(SET *sp) {
  assert(sp != NULL);
  return (sp->count);
}

// adds an element to the set and updates number of elements as count
void addElement(SET *sp, char *elt) {
  assert(sp != NULL);
  bool found;
  int idx, i;
  idx = search(sp, elt, &found);
  if (found == true) {
    return;
  }
  assert(sp->count < sp->length);
  for (i = sp->count; i > idx; i--) {
    sp->data[i] = sp->data[i - 1];
  }
  sp->data[idx] = strdup(elt);
  sp->count++;
  return;
}

// searches for a duplicate data in set, returns position of first duplicate or
// -1
static int search(SET *sp, char *elt, bool *found) {
  // assert(sp != NULL);
  int lo, hi, mid, flag;
  hi = sp->count - 1;
  lo = 0;
  while (lo <= hi) {
    mid = (hi + lo) / 2;
    flag = strcmp(elt, sp->data[mid]);
    if (flag > 0) {
      hi = mid - 1;
    } else if (flag < 0) {
      lo = mid + 1;
    } else {
      *found = true;
      return mid;
    }
  }
  *found = false;
  return lo;
}

// removes an element from the set and updates count
void removeElement(SET *sp, char *elt) {
  assert(sp != NULL);
  bool found;
  int i, idx;
  idx = search(sp, elt, &found);
  if (found == false) {
    return;
  } else {
    free(sp->data[idx]);
    // shift all elements up from idx
    for (i = idx + 1; i < sp->count; i++) {
      sp->data[i - 1] = sp->data[i];
    }
    sp->count--;
    return;
  }
}

// looks for the element in set, returns its conetns if found or NULL otherwise
char *findElement(SET *sp, char *elt) {
  assert(sp != NULL);
  bool found;
  int pos = search(sp, elt, &found);
  return (found ? sp->data[pos] : NULL);
}

// copies all elements from set to a new memory location
char **getElements(SET *sp) {
  assert(sp != NULL);
  char **a;
  a = malloc(sizeof(char *) * sp->count);
  assert(a != NULL);
  return (memcpy(a, sp->data, sizeof(char *) * sp->count));
}
