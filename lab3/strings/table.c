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

// Defines SET structure
typedef struct set {
  // Number of elements in the set
  int count;
  // Max length of set
  int length;
  // String that set contains
  char **data;
  // Flag to check if the data is deleted
  char *flag;
} SET;

// Defines search function to be used later
static int search(SET *sp, char *elt, bool *found);

// Defines hash function to be used later
static unsigned strhash(char *s);

// Hash function that calculates and returns the home position of the string in
// the set. The Big-O runtime of this function is O(n), where n is the length of
// the input string.
static unsigned strhash(char *s) {
  unsigned hash = 0;
  while (*s != '\0') {
    hash = 31 * hash + *s++;
  }
  return hash;
}

// Creates set with max number of elements defined as maxElts, returns pointer
// to set. The Big-O runtime of this function is O(n), where n is the lenthg of
// the set, as it iterates through the loop for a maximum of maxElts times.
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

// Deletes set and frees all memory. The Big-O runtime of this function is O(n),
// where n is the length of the set. This is because the function iterates
// through each element in the set and performs constant time operations
// (freeing memory) on each element.
void destroySet(SET *sp) {
  assert(sp != NULL);
  int i;
  for (i = 0; i < sp->length; i++) {
    // Free data only if it's present and not already deleted
    if (sp->flag[i] == 2) {
      free(sp->data[i]);
    }
  }
  // Free data pointer
  free(sp->data);
  // Free flags
  free(sp->flag);
  // Free set
  free(sp);
  return;
}

// Returns the number of elements in set. The Big-O runtime for this function is
// O(1) because it has a constant runtime regardless of the size of the set.
int numElements(SET *sp) {
  assert(sp != NULL);
  return (sp->count);
}

// Searches for data that is the same as elt in set, returns position of first
// match or the position of the first empty (or deleted) spot in the set if
// there is no match. Also changes the value of found in parent function. The
// Big-O runtime of this function is O(n), where n is the length of the set.
// Expected O(1).
static int search(SET *sp, char *elt, bool *found) {
  // Don't need to assert for found as it's defined and used locally
  assert(sp != NULL && elt != NULL);
  int i, pos, del;
  bool delF = false;
  for (i = 0; i < sp->length; i++) {
    // Find position for the elt with hash function using linear hashing
    pos = (strhash(elt) + i) % sp->length;
    switch (sp->flag[pos]) {
    // Encountered a filled position
    case 2:
      // Check if found
      if (strcmp(elt, sp->data[pos]) == 0) {
        *found = true;
        return pos;
      }
      break;
    // Encountered a deleted position
    case 1:
      // Check if it's first deleted and mark it
      if (!delF) {
        delF = true;
        del = pos;
      }
      break;
    // Encountered an empty position
    default:
      // Return not found
      *found = false;
      // If return del if there was a deleted element or pos if there was no
      // deleted elements encountered
      return (delF ? del : pos);
    }
  }
  // If something went wrong abort ffs (for fun's sake)
  abort();
}

// Inserts elt into set if it's not found. The Big-O runtime of this function is
// O(n), where n is the lenthg of the set. This is because the function calls
// the search function. Expected O(1).
void addElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx = search(sp, elt, &found);
  // Only add if there's no match
  if (!found) {
    assert(sp->count < sp->length);
    sp->data[idx] = strdup(elt);
    sp->flag[idx] = 2;
    sp->count++;
  }
  return;
}

// Remove elt from set if it's found. The Big-O runtime of this function is
// O(n), where n is the length of the set. Expected O(1).
void removeElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int idx = search(sp, elt, &found);
  // Delete only if found
  if (found) {
    free(sp->data[idx]);
    sp->flag[idx] = 1;
    sp->count--;
  }
  return;
}

// Find elt in set and return it or NULL pointer. The Big-O runtime of this
// function is O(log n), where n is the lenthg of the set. Expected O(1).
char *findElement(SET *sp, char *elt) {
  assert(sp != NULL && elt != NULL);
  bool found;
  int pos = search(sp, elt, &found);
  return (found ? sp->data[pos] : NULL);
}

// Get all elements from set and return them as an array of strings. The Big-O
// runtime of this function is O(n), where n is the length of the set.
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
