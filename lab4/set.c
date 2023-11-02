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
  int length;                             // max length of the list*
  LIST **list;                            // array of lists
  int (*compare)(void *elt1, void *elt2); // compare function provided by driver
  unsigned (*hash)(void *elt);            // hash function provided by driver
} SET;

// Create set and return a pointer to the set
// Big O = O(m) where m is the length of the set
SET *createSet(int maxElts, int (*compare)(void *elt1, void *elt2),
               unsigned (*hash)(void *elt)) {
  assert(hash != NULL && compare != NULL);

  SET *sp = malloc(sizeof(SET));
  assert(sp != NULL);

  // There are no elements when set is initialized
  sp->count = 0;
  // *Maximum maxElts/20 should be created
  sp->length = maxElts / 20;
  // Allocate memory for all lists
  sp->list = malloc(sizeof(LIST *) * sp->length);
  assert(sp->list != NULL);
  // Compare and hash pointers to driver functions
  sp->compare = compare;
  sp->hash = hash;

  int i;
  // *Create maxElts/20 lists to optimize the search time
  for (i = 0; i < sp->length; i++) {
    sp->list[i] = createList(compare);
  }

  return sp;
}

// Destroy all lists in the set, the set itself, and free memory
// Big O = O(n), where n is the sp->length
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

// Just return the number of elements in the set in all lists
// Big O = O(1)
int numElements(SET *sp) {
  assert(sp != NULL);

  return (sp->count);
}

// Add new element to the beginning of the list that has the
// hash of the added element. Big O = O(n) because of findItem,
// where n is the length of the list
void addElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);

  // Gets the index of the set that we need with external hash function
  int idx = sp->hash(elt) % sp->length;
  if (findItem(sp->list[idx], elt) == NULL) {
    addFirst(sp->list[idx], elt);
    sp->count++;
  }

  return;
}

// Remove an element from the list that has the hash
// of this item. Big O = O(n) because of removeItem, where
// n is the length of the list
void removeElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);

  // Gets the index of the set that we need with external hash function
  int idx = sp->hash(elt) % sp->length;
  // Finds and removes the element, no need to find first
  removeItem(sp->list[idx], elt);
  sp->count--;

  return;
}

// Find an element in the list that has the hash of this element
// Big O = O(n) because of findItem, where n is the length of the list
void *findElement(SET *sp, void *elt) {
  assert(sp != NULL && elt != NULL);

  // Gets the index of the set that we need with external hash function
  int idx = sp->hash(elt) % sp->length;

  return (findItem(sp->list[idx], elt));
}

// Gets and returns all data from all lists present in the set
// Big O = O(m), where m is the length of the set
void *getElements(SET *sp) {
  assert(sp != NULL);

  int i, index = 0;
  void **a = malloc(sizeof(void *) * sp->count);
  assert(a != NULL);

  // Position to insert the new items in the array
  for (i = 0; i < sp->length; i++) {
    void *items = getItems(sp->list[i]);
    memcpy(a + index, items, sizeof(void *) * numItems(sp->list[i]));
    // The new insertion happens at the end of previous
    index += numItems(sp->list[i]);
  }

  return a;
}
