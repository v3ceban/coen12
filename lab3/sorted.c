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
// Worst case (not considering search function): O(n), where n is number of elements in set (sp->count)
// Worst case (considering search function): O(n*log(n))
void addElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx, i;
  // set idx as position of first duplicate in the set
  idx = search(sp, elt, &found);
  if (found == true) {
    return;
  }
  assert(sp->count < sp->length);
  // shift all elements down starting at the end of the array up to idx
  for (i = sp->count; i > idx; i--) {
    sp->data[i] = sp->data[i - 1];
  }
  // copy elt to set and update counter
  sp->data[idx] = strdup(elt);
  sp->count++;
  return;
}

// searches for a duplicate data in set, returns position of first duplicate or first empty spot in the set
// Worst case: O(log(n)), where n is number of elements in set (sp->count)
static int search(SET *sp, char *elt, bool *found) {
  // don't need to assert for found as it's defined and used locally
  assert(sp != NULL && elt != NULL);
  int lo, hi, mid, flag;
  hi = sp->count - 1;
  lo = 0;
  //binary search
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
// Worst case (not considering search funciton): O(n), where n is number of elements in set (sp->count)
// Worst case (considering search function): O(n*log(n))
void removeElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
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

// looks for the element in set, returns its contents if found or NULL otherwise
// O(1) (not considering search)
// O(log(n)) (considering search)
char *findElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int pos = search(sp, elt, &found);
  return (found ? sp->data[pos] : NULL);
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
