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

// Include headers and libraries
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define struct for list
typedef struct list {
  // Number of items in the list
  int count;
  // List containst the head that points to other nodes in the list
  struct node *head;
  // List also has a compare function implemented in driver main function
  int (*compare)(void *, void *);
} LIST;

// Define struct for node
typedef struct node {
  void *data;
  // Each node points to next and previous nodes in the list
  struct node *next;
  struct node *prev;
} NODE;

// Creates list pointer and returns a LIST pointer
// Big O = O(1)
LIST *createList(int (*compare)()) {
  // The assignment tells to not assert for compare here

  // Allocate memory for the list and check that it worked out
  LIST *list = (LIST *)malloc(sizeof(LIST));
  assert(list != NULL);

  // Allocate memory for dummy node and make sure it allocated correctly
  NODE *head = (NODE *)malloc(sizeof(NODE));
  assert(head != NULL);

  // Assign list's head to created dummy node and set it to point to itself
  // This helps to avoid empty head cases
  list->head = head;
  head->next = head->prev = head;

  // Set compare to function pointer and initialize the count
  list->compare = compare;
  list->count = 0;

  return list;
}

// Deletes the whole list and frees all memory
// Big O = O(lp->count)
void destroyList(LIST *lp) {
  assert(lp != NULL);

  NODE *current;
  // Start at first node and cycle untill return to head
  for (current = lp->head->next; current != lp->head;
       current = lp->head->next) {
    // Move head->next to save the node
    lp->head->next = current->next;
    free(current);
  }

  // Free head, as it was not freed in the loop, and free list
  free(lp->head);
  free(lp);

  return;
}

// Return the number of nodes in the list
// Big O = O(1)
int numItems(LIST *lp) {
  assert(lp != NULL);
  return (lp->count);
}

// Add a node as first element in the list
// Big O = O(1)
void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);

  // Allocate memory for new node and check allocations
  NODE *newNode = (NODE *)malloc(sizeof(NODE));
  assert(newNode != NULL);

  // Copy item to new node's data
  newNode->data = item;

  // Shift pointers around to insert new node
  newNode->next = lp->head->next;
  newNode->prev = lp->head;
  lp->head->next->prev = newNode;
  lp->head->next = newNode;

  // Update counter
  lp->count++;
  return;
}

// Add a node as last element in the list
// Big O = O(1)
void addLast(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);

  // Allocate memory for new node and check allocation
  NODE *newNode = (NODE *)malloc(sizeof(NODE));
  assert(newNode != NULL);

  // Copy item to new node's data
  newNode->data = item;

  // Shift pointers around to insert a new node
  newNode->next = lp->head;
  newNode->prev = lp->head->prev;
  lp->head->prev->next = newNode;
  lp->head->prev = newNode;

  // Update counter
  lp->count++;

  return;
}

// Delete first node in the list (not head, as it's a dummy node and doesn't
// count). Big O = O(1)
void *removeFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);

  // First node is after head
  NODE *first = lp->head->next;
  // Copy data to return it later
  void *data = first->data;

  // Shift pointers around to delete first node
  lp->head->next = first->next;
  first->next->prev = first->next;

  // Update counter and free node
  lp->count--;
  free(first);

  return data;
}

// Delete the last node in the list (also not head for the same reasons)
// Big O = O(1)
void *removeLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);

  // Last node is before head
  NODE *last = lp->head->prev;
  // Copy data to return it
  void *data = last->data;

  // Shift pointers around to delete last node
  lp->head->prev = last->prev;
  last->prev->next = last->next;

  // Update counter and free node
  lp->count--;
  free(last);

  return data;
}

// Find and return data from the first node in the list (not head as it's
// dummy) Big O = O(1)
void *getFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // As specified by instructions, list must not be empty
  // if it could've been empty, an if statement should've been used to
  // verify the condition and return NULL in case of empty list
  return (lp->head->next->data);
}

// Find and return data from the last node in the list
// Big O = O(1)
void *getLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // As specified by instructions, list must not be empty
  // if it could've been empty, an if statement should've been used to
  // verify the condition and return NULL in case of empty list
  return (lp->head->prev->data);
}

// Find and remove (free) a node with data matching the given item
// Big O = O(lp->count)
void removeItem(LIST *lp, void *item) {
  // Check that lp and item are valid and that list is not empty
  assert(lp != NULL && item != NULL && lp->count > 0);

  NODE *current;
  // Start searching from the first node and look until we return to the head
  for (current = lp->head->next; current != lp->head; current = current->next) {
    if (lp->compare(item, current->data) == 0) { // if node's data matches item
      // Shift pointers to delete node
      current->prev->next = current->next;
      current->next->prev = current->prev;

      // Delete node and update counter
      free(current);
      lp->count--;
      return;
    }
  }

  return;
}

// Find and return node's data that matches the given item
// Big O = O(lp->count)
void *findItem(LIST *lp, void *item) {
  // Check that lp and item are valid and that list is not empty
  assert(lp != NULL && item != NULL && lp->count >= 0);

  NODE *current;
  // Start searching from the first node and look until we return to the head
  for (current = lp->head->next; current != lp->head; current = current->next) {
    if (lp->compare(item, current->data) == 0) { // if node's data matches item
      return (current->data);
    }
  }

  return NULL;
}

// Create and return an array of data from all nodes in the list
// Big O = O(lp->count)
void *getItems(LIST *lp) {
  // Check that lp still exists
  assert(lp != NULL);

  // Allocate memory for the array and check that it was successful
  void **items = malloc(lp->count * sizeof(void *));
  assert(items != NULL);

  NODE *current;
  int idx = 0;
  // Loop through the array from first node untill we return back to head
  for (current = lp->head->next; current != lp->head;
       current = current->next, idx++) {
    // Copy data to items array
    items[idx] = current->data;
  }

  return items;
}
