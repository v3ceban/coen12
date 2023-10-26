/*
 * File: table.c - generic
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Desription: This file contains functions defined in list.h
 * to be used in the main driver functions. See functions for mode detailed
 * desctiption
 *
 */

// include headers and libraries
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list { // define struct list
  int count;
  // list containst the head that points to other nodes in the list
  struct node *head;
  // list also has a compare function implemented in driver main function
  int (*compare)(void *elt1, void *elt2);
} LIST;

typedef struct node { // define struct NODE
  void *data;
  // each node points to next and previous nodes in the list
  struct node *next;
  struct node *prev;
} NODE;

// creates list pointer and returns a LIST pointer
// Big O = O(1)
LIST *createList(int (*compare)(void *elt1, void *elt2)) {
  LIST *list = (LIST *)malloc(sizeof(LIST)); // allocate memory for the list
  assert(list != NULL);                      // check if it worked
  NODE *head = (NODE *)malloc(sizeof(NODE)); // allocate memory for dummy node
  assert(head != NULL);                      // make sure it allocated correctly
  list->head = head;              // assign list's head to created dummy node
  head->next = head->prev = head; // set head to point to itself
  list->compare = compare;        // assign compare pointer to function pointer
  list->count = 0;                // set number of nodes in list to zero
  return list;
}

// deletes the whole list and frees all memory
// Big O = O(lp->count)
void destroyList(LIST *lp) {
  assert(lp != NULL); // check that lp pointer was received correctly
  NODE *current = lp->head->next;   // start deleting from first node
  while (current != lp->head) {     // cycle untill return to head
    lp->head->next = current->next; // move head->next to save the node
    free(current);                  // free current node
    current = lp->head->next;       // restore the saved node
  }               // do while loop didn't work, but maybe I did something wrong
  free(lp->head); // free head
  free(lp);       // free list
}

// return the number of nodes in the list
// Big O = O(1)
int numItems(LIST *lp) {
  assert(lp != NULL); // check that lp is alright
  return (lp->count); // return it
}

// add a node as first element in the list
// Big O = O(1)
void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);           // check that we got everything
  NODE *newNode = (NODE *)malloc(sizeof(NODE)); // allocate memory for new node
  assert(newNode != NULL); // make sure allocation was successful
  newNode->data = item;    // copy item to new node's data

  // shift pointers around to insert new node
  newNode->next = lp->head->next;
  newNode->prev = lp->head;
  lp->head->next->prev = newNode;
  lp->head->next = newNode;
  lp->count++; // update counter
}

// add a node as last element in the list
// Big O = O(1)
void addLast(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);           // check variables
  NODE *newNode = (NODE *)malloc(sizeof(NODE)); // allocate memory for new node
  assert(newNode != NULL);                      // check allocation
  newNode->data = item;                         // copy item to new node's data

  // shift pointers around to insert a new node
  newNode->next = lp->head;
  newNode->prev = lp->head->prev;
  lp->head->prev->next = newNode;
  lp->head->prev = newNode;
  lp->count++; // update counter
}

// delete first node in the list (not head, as it's a dummy node and doesn't
// count). Big O = O(1)
void *removeFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0); // check if lp is alive and isn't empty
  NODE *first = lp->head->next;        // first node is after head
  void *data = first->data;            // copy data to return it

  // shift pointers around to delete first node
  lp->head->next = first->next;
  first->next->prev = first->next;

  lp->count--; // update counter
  free(first); // free node
  return data;
}

void *removeLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0); // check if lp is alive and isn't empty
  NODE *last = lp->head->prev;         // last node is before head
  void *data = last->data;             // copy data to return it

  // shift pointers around to delete last node
  lp->head->prev = last->prev;
  last->prev->next = last->next;

  lp->count--; // update counter
  free(last);  // free node
  return data;
}

void *getFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  return (lp->head->next->data);
}

void *getLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  return (lp->head->prev->data);
}

void removeItem(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  if (lp->count == 0) {
    return;
  }
  NODE *current = lp->head->next;
  while (current != NULL) {
    if (lp->compare(item, current->data) == 0) {
      current->prev = current->next;
      current->next->prev = current->prev;
      free(current);
      lp->count--;
      return;
    }
    current = current->next;
  }
}

void *findItem(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  void *data = NULL;
  if (lp->count == 0) {
    return data;
  }
  NODE *current = lp->head->next;
  while (current != NULL) {
    if (lp->compare(item, current->data) == 0) {
      data = current->data;
      break;
    }
    current = current->next;
  }
  return data;
}

void *getItems(LIST *lp) {
  assert(lp != NULL);
  void **items = malloc(lp->count * sizeof(void *));
  assert(items != NULL);
  NODE *current = lp->head->next;
  int i = 0;
  while (current != NULL) {
    items[i] = current->data;
    current = current->next;
    i++;
  }
  return items;
}
