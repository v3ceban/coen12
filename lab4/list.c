/*
 * File: list.c
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Desription: This file contains functions defined in list.h
 * that are used in the main driver functions in radix.c and maze.c.
 *
 * See comments for the functions below for mode detailed desctiption for each
 * of them.
 *
 */

// include headers and libraries
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define struct for list
typedef struct list {
  int count;
  // list containst the head that points to other nodes in the list
  struct node *head;
  // list also has a compare function implemented in driver main function
  int (*compare)(void *, void *);
} LIST;

// define struct for node
typedef struct node {
  void *data;
  // each node points to next and previous nodes in the list
  struct node *next;
  struct node *prev;
} NODE;

// creates list pointer and returns a LIST pointer
// Big O = O(1)
LIST *createList(int (*compare)()) {
  // allocate memory for the list and check that it worked out
  LIST *list = (LIST *)malloc(sizeof(LIST));
  assert(list != NULL);

  // allocate memory for dummy node and make sure it allocated correctly
  NODE *head = (NODE *)malloc(sizeof(NODE));
  assert(head != NULL);

  // assign list's head to created dummy node and set it to point to itself
  // this helps to avoid empty head cases
  list->head = head;
  head->next = head->prev = head;

  // set compare to function pointer and initialize the count
  list->compare = compare;
  list->count = 0;

  return list;
}

// deletes the whole list and frees all memory
// Big O = O(lp->count)
void destroyList(LIST *lp) {
  assert(lp != NULL);

  NODE *current;

  // start at first node and cycle untill return to head
  for (current = lp->head->next; current != lp->head;
       current = lp->head->next) {
    // move head->next to save the node
    lp->head->next = current->next;
    free(current);
  }
  // free head as it was not freed in the loop
  free(lp->head);
  // free list to free all memory
  free(lp);
  return;
}

// return the number of nodes in the list
// Big O = O(1)
int numItems(LIST *lp) {
  assert(lp != NULL);
  return (lp->count);
}

// add a node as first element in the list
// Big O = O(1)
void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  // allocate memory for new node and check allocations
  NODE *newNode = (NODE *)malloc(sizeof(NODE));
  assert(newNode != NULL);
  // copy item to new node's data
  newNode->data = item;

  // shift pointers around to insert new node
  newNode->next = lp->head->next;
  newNode->prev = lp->head;
  lp->head->next->prev = newNode;
  lp->head->next = newNode;

  // update counter
  lp->count++;
  return;
}

// add a node as last element in the list
// Big O = O(1)
void addLast(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);

  // allocate memory for new node and check allocation
  NODE *newNode = (NODE *)malloc(sizeof(NODE));
  assert(newNode != NULL);

  // copy item to new node's data
  newNode->data = item;

  // shift pointers around to insert a new node
  newNode->next = lp->head;
  newNode->prev = lp->head->prev;
  lp->head->prev->next = newNode;
  lp->head->prev = newNode;

  // update counter
  lp->count++;
  return;
}

// delete first node in the list (not head, as it's a dummy node and doesn't
// count). Big O = O(1)
void *removeFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // first node is after head
  NODE *first = lp->head->next;
  // copy data to return it
  void *data = first->data;

  // shift pointers around to delete first node
  lp->head->next = first->next;
  first->next->prev = first->next;

  // update counter and free node
  lp->count--;
  free(first);
  return data;
}

// delete the last node in the list (also not head for the same reasons)
// Big O = O(1)
void *removeLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // last node is before head
  NODE *last = lp->head->prev;
  // copy data to return it
  void *data = last->data;

  // shift pointers around to delete last node
  lp->head->prev = last->prev;
  last->prev->next = last->next;

  // update counter and free node
  lp->count--;
  free(last);
  return data;
}

// find and return data from the first node in the list (not head as it's
// dummy) Big O = O(1)
void *getFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // as specified by instructions, list must not be empty
  // if it could've been empty, an if statement should've been used to
  // verify the condition and return NULL in case of empty list
  return (lp->head->next->data);
}

// find and return data from the last node in the list
// Big O = O(1)
void *getLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // as specified by instructions, list must not be empty
  // if it could've been empty, an if statement should've been used to
  // verify the condition and return NULL in case of empty list
  return (lp->head->prev->data);
}

// find and remove (free) a node with data matching the given item
// Big O = O(lp->count)
void removeItem(LIST *lp, void *item) {
  // check that lp and item are valid and that list is not empty
  // assert(lp != NULL && item != NULL && lp->count > 0);
  assert(lp != NULL);
  assert(item != NULL);
  assert(lp->count > 0);

  NODE *current;
  // start searching from the first node and look until we return to the head
  for (current = lp->head->next; current != lp->head; current = current->next) {
    if (lp->compare(item, current->data) == 0) { // if node's data matches item
      // shift pointers to delete node
      current->prev->next = current->next;
      current->next->prev = current->prev;

      // delete node and update counter
      free(current);
      lp->count--;
      return;
    }
  }
  return;
}

// find and return node's data that matches the given item
// Big O = O(lp->count)
void *findItem(LIST *lp, void *item) {
  // check that lp and item are valid and that list is not empty
  assert(lp != NULL && item != NULL && lp->count >= 0);
  NODE *current;
  // start searching from the first node and look until we return to the head
  for (current = lp->head->next; current != lp->head; current = current->next) {
    if (lp->compare(item, current->data) == 0) { // if node's data matches item
      return (current->data);
    }
  }
  return NULL;
}

// create and return an array of data from all nodes in the list
// Big O = O(lp->count)
void *getItems(LIST *lp) {
  // check that lp still exists
  assert(lp != NULL);

  // allocate memory for the array and check that it was successful
  void **items = malloc(lp->count * sizeof(void *));
  assert(items != NULL);

  NODE *current;
  int idx = 0;
  // loop through the array from first node untill we return back to head
  for (current = lp->head->next; current != lp->head;
       current = current->next, idx++) {
    // copy data to items array
    items[idx] = current->data;
  }

  return items;
}
