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
  // number of elements in the set
  int count;
  // max length of set
  int length;
  // string that set contains
  char **data;
  // flag to check if the data is deleted
  char *flag;
} SET;

// defines search function to be used later
static int search(SET *sp, char *elt, bool *found);

// defines hash function to be used later
static unsigned strhash(char *s);

// hash function
static unsigned strhash(char *s) {
  unsigned hash = 0;
  while (*s != '\0') {
    hash = 31 * hash + *s++;
  }
  return hash;
}

// creates set with max number of elements defined as maxElts, returns pointer
// to set, O(maxElts)
SET *createSet(int maxElts) {
  SET *sp;
  int i;
  sp = malloc(sizeof(SET));
  assert(sp != NULL);
  sp->count = 0;
  sp->length = maxElts;
  sp->data = malloc(sizeof(char *) * maxElts);
  assert(sp->data != NULL);
  sp->flag = malloc(sizeof(char) * maxElts);
  assert(sp->flag != NULL);
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
static int search(SET *sp, char *elt, bool *found) {
  // don't need to assert for found as it's defined and used locally
  assert(sp != NULL && elt != NULL);
  int i;
  // find home pos for the elt with hash function
  int pos = strhash(elt) % sp->length;
  for (i = 0; i < sp->length; i++) {
    char flag = sp->flag[pos];
    // return when found an empty position, as elt is not in the set
    if (flag == 0) {
      *found = false;
      return pos;
      // return if position has element and it's the same string as elt
    } else if (flag == 2 && strcmp(elt, sp->data[pos]) == 0) {
      *found = true;
      return pos;
      // update position with linear hashing
    } else {
      pos = (i + strhash(elt)) % sp->length;
    }
  }
  // return in case something went wrong
  *found = false;
  return pos;
}

// inserts elt into set if it's not found. Big O fully depends on search
// function. Worst case O(sp->length). Expected case O(1).
void addElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx = search(sp, elt, &found);
  if (!found) {
    assert(sp->count < sp->length);
    sp->data[idx] = strdup(elt);
    sp->flag[idx] = 2;
    sp->count++;
    return;
  }
  return;
}

// remove elt from set if it's found. Big O fully depends on search
// function. Worst case O(sp->length). Expected case O(1).
void removeElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx = search(sp, elt, &found);
  if (!found) {
    return;
  }
  free(sp->data[idx]);
  sp->flag[idx] = 1;
  sp->count--;
  return;
}

char *findElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int pos = search(sp, elt, &found);
  return (found ? sp->data[pos] : NULL);
}

char **getElements(SET *sp) {
  assert(sp != NULL);
  char **a;
  int i, j;
  a = malloc(sizeof(char *) * sp->count);
  assert(a != NULL);
  for (i = 0, j = 0; i < sp->length; i++) {
    if (sp->flag[i] == 2) {
      char *str = strdup(sp->data[i]);
      a[j] = str;
      j++;
    }
  }
  return a;
}
