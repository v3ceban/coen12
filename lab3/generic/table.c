/*
 * File: table.c - generic
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
  // number of elements in the set
  int count;
  // max length of set
  int length;
  // string that set contains
  void **data;
  // flag to check if the data is deleted
  char *flag;
  // pointer to compare function
  int (*compare)(void *elt1, void *elt2);
  // pointer to hash function
  unsigned (*hash)(void *elt);
} SET;

// defines search function to be used later
static int search(SET *sp, void *elt, bool *found);

// creates set with max number of elements defined as maxElts, returns pointer
// to set, O(maxElts)
SET *createSet(int maxElts, int (*compare)(void *elt1, void *elt2),
               unsigned (*hash)(void *elt)) {
  SET *sp;
  int i;
  sp = malloc(sizeof(SET));
  assert(sp != NULL);
  sp->count = 0;
  sp->length = maxElts;
  sp->data = malloc(sizeof(void *) * maxElts);
  assert(sp->data != NULL);
  sp->flag = malloc(sizeof(char) * maxElts);
  assert(sp->flag != NULL);
  sp->compare = compare;
  assert(sp->compare != NULL);
  sp->hash = hash;
  assert(sp->hash != NULL);
  // set all flags to empty
  for (i = 0; i < maxElts; i++) {
    sp->flag[i] = 0;
  }
  return sp;
}

// deletes set and frees all memory
// O(n), where n is the lendgh of the set (sp->length)
void destroySet(SET *sp) {
  assert(sp != NULL);
  int i;
  for (i = 0; i < sp->length; i++) {
    // free data only if it's present and not already deleted
    if (sp->flag[i] == 2) {
      free(sp->data[i]);
    }
  }
  // free data pointer
  free(sp->data);
  // free set
  free(sp);
  return;
}

// returns the number of elements in set
// O(1)
int numElements(SET *sp) {
  assert(sp != NULL);
  return (sp->count);
}

// searches for a duplicate data in set, returns position of first duplicate or
// first empty spot in the set. Worst case: O(log(n)), where n is the length of
// set (sp->length). Expected case: O(1)
static int search(SET *sp, void *elt, bool *found) {
  // don't need to assert for found as it's defined and used locally
  assert(sp != NULL && elt != NULL);
  int i, pos, del;
  bool delF = false;

  for (i = 0; i < sp->length; i++) {
    // find position for the elt with hash function using linear hashing
    pos = ((*sp->hash)(elt) + i) % sp->length;
    switch (sp->flag[pos]) {
    // encountered a filled position
    case 2:
      // check if found
      if ((*sp->compare)(elt, sp->data[pos]) == 0) {
        *found = true;
        return pos;
      }
      break;

    // encountered a deleted position
    case 1:
      // check if it's first deleted and mark it
      if (!delF) {
        delF = true;
        del = pos;
      }
      break;

    // encountered an empty position
    default:
      // return not found
      *found = false;
      // if return del if there was a deleted element or pos if there was no
      // deleted elements encountered
      return (delF ? del : pos);
    }
  }
  // if something went wrong abort ffs (for fun's sake)
  abort();
}

// inserts elt into set if it's not found. Big O fully depends on search
// function. Worst case O(sp->length). Expected case O(1).
void addElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx = search(sp, elt, &found);
  // only add if there's no match
  if (!found) {
    assert(sp->count < sp->length);
    sp->data[idx] = elt;
    sp->flag[idx] = 2;
    sp->count++;
  }
  return;
}

// remove elt from set if it's found. Big O fully depends on search
// function. Worst case O(sp->length). Expected case O(1).
void removeElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx = search(sp, elt, &found);
  // delete only if found
  if (found) {
    sp->flag[idx] = 1;
    sp->count--;
  }
  return;
}

// find elt in set and return it or NULL pointer. Big O fully depends on search
// function. Worst case O(sp->length). Expected case O(1).
void *findElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int pos = search(sp, elt, &found);
  return (found ? sp->data[pos] : NULL);
}

// get all elements from set and return them as an array of strings. Since the
// function has to go through the whole set, Big O = O(sp->length)
void *getElements(SET *sp) {
  assert(sp != NULL);
  void **a;
  int i, j;
  a = malloc(sizeof(void *) * sp->count);
  assert(a != NULL);
  for (i = 0, j = 0; i < sp->length; i++) {
    if (sp->flag[i] == 2) {
      void *data = sp->data[i];
      a[j] = data;
      j++;
    }
  }
  return a;
}
