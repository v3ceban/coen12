/*
 * File: set.c
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Description: This file is basically a copy of table.c from lab3, but it uses
 * functions from list.h instead and stores the lists it creates in the set. It
 * contains all the functions used in main driver for testing text files with
 * various string functions. See function definitions for more info
 *
 */

#include "set.h"
#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct set {
  int count;                              // number of elements in the set
  int length;                             // max length of the list
  LIST **list;                            // array of lists
  int (*compare)(void *elt1, void *elt2); // compare function provided by driver
  unsigned (*hash)(void *elt);            // hash function provided by driver
} SET;

// create set and return a pointer to it
SET *createSet(int maxElts, int (*compare)(void *elt1, void *elt2),
               unsigned (*hash)(void *elt)) {
  assert(hash != NULL && compare != NULL);
  SET *sp = malloc(sizeof(SET));
  assert(sp != NULL);
  // there are no elements when set is initialized
  sp->count = 0;
  // maximum maxElts/20 should be created
  sp->length = maxElts / 20;
  // allocate memory for all lists
  sp->list = malloc(sizeof(LIST *) * sp->length);
  assert(sp->list != NULL);
  int i;
  // create maxElts/20 lists to optimize the search time
  for (i = 0; i < sp->length; i++) {
    sp->list[i] = createList(compare);
  }
  // compare and hash pointers to driver functions
  sp->compare = compare;
  sp->hash = hash;
  return sp;
}

// destroy all lists in the set, the set itself, and free memory
void destroySet(SET *sp) {
  assert(sp != NULL);
  int i;
  for (i = 0; i < sp->length; i++) {
    destroyList(sp->list[i]);
  }
  free(sp->list);
  free(sp);
  return;
}

// just return the number of elements in the set in all lists
int numElements(SET *sp) {
  assert(sp != NULL);
  return (sp->count);
}

void addElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
  int idx = sp->hash(elt) % sp->length;
  if (findItem(sp->list[idx], elt) == NULL) {
    addFirst(sp->list[idx], elt);
    sp->count++;
  }
}

void removeElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
  int idx = sp->hash(elt) % sp->length;
  if (findItem(sp->list[idx], elt) != NULL) {
    removeItem(sp->list[idx], elt);
    sp->count--;
  }
}

void *findElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);
  int idx = sp->hash(elt) % sp->length;
  return (findItem(sp->list[idx], elt));
}

void *getElements(SET *sp) {
  assert(sp != NULL);
  void **a;
  int i;
  a = malloc(sizeof(void *) * sp->count);
  assert(a != NULL);
  int index = 0;
  for (i = 0; i < sp->length; i++) {
    void **items = getItems(sp->list[i]);
    memcpy(a + index, items, sizeof(void *) * numItems(sp->list[i]));
    index += numItems(sp->list[i]);
  }
  return a;
}
