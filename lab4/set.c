/*
 * File: set.c
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Description: This file is a copy of table.c from lab3, but it uses functions
 * from list.h instead. It contains all the functions used in main for testing
 * text files with various string functions
 * See function definitions for more info
 *
 */

#include "set.h"
#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct set {
  int count;
  int length;
  LIST **list;
  char *flag;
  int (*compare)(void *elt1, void *elt2);
  unsigned (*hash)(void *elt);
} SET;

SET *createSet(int maxElts, int (*compare)(void *elt1, void *elt2),
               unsigned (*hash)(void *elt)) {
  assert(hash != NULL && compare != NULL);
  SET *sp;
  sp = malloc(sizeof(SET));
  assert(sp != NULL);
  sp->count = 0;
  sp->length = maxElts / 20;
  sp->list = malloc(sizeof(LIST *) * maxElts);
  assert(sp->list != NULL);
  int i;
  for (i = 0; i < sp->length; i++) {
    sp->list[i] = createList(compare);
  }
  sp->flag = malloc(sizeof(char) * maxElts);
  assert(sp->flag != NULL);
  sp->compare = compare;
  sp->hash = hash;
  return sp;
}

void destroySet(SET *sp) {
  assert(sp != NULL);
  int i;
  for (i = 0; i < sp->length; i++) {
    destroyList(sp->list[i]);
  }
  free(sp->list);
  free(sp->flag);
  free(sp);
  return;
}

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
