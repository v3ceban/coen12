#include "list.h"
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

// For data I need to know:
// 1. Length of the array, which is a constant
// 2. Current position of head
// 3. Number of items in the array

// Nodes should have next and prev pointers for easy access to last and first
// node from head

typedef struct node {
  void **data;
  int start;
  int count;
  int length;
  struct node *next;
  struct node *prev;
} NODE;

// No need for tail pointer as head points in both directions
// Head is sentinel

typedef struct list {
  int count;
  NODE *head;
} LIST;

LIST *createList(void) {
  LIST *lp = malloc(sizeof(LIST));
  assert(lp != NULL);
  lp->head = malloc(sizeof(NODE));
  assert(lp->head != NULL);
  lp->head->next = lp->head;
  lp->head->prev = lp->head;
  lp->head->length = lp->head->count = 0;
  lp->count = 0;
  return lp;
}

void destroyList(LIST *lp) {
  assert(lp != NULL);
  NODE *current = lp->head;
  while (current != NULL) {
    NODE *next = current->next;
    free(current->data);
    free(current);
    current = next;
  }
  free(lp);
}

int numItems(LIST *lp) {
  assert(lp != NULL);
  return lp->count;
}

void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  NODE *first = lp->head->next;

  if (first->count == first->length) {
    // create new node
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    assert(newNode != NULL);
    // create the array in the node
    newNode->data = (void **)malloc(lp->head->next->length * sizeof(void *));
    assert(newNode->data != NULL);
    newNode->count = 0;
    newNode->length = 10;
    newNode->start = 0;
    newNode->data[newNode->start] = item;
    newNode->next = first;
    newNode->prev = lp->head;
    first->prev = newNode;
    first = newNode;
  } else {
    int idx = (first->count - 1) + first->start % first->length;
    first->data[idx] = item;
  }

  lp->count++;
  return;
}

// copied
// void addLast(LIST *lp, void *item) {
//   assert(lp != NULL && item != NULL);
//   NODE *newNode = (NODE *)malloc(sizeof(NODE));
//   assert(newNode != NULL);
//   newNode->data = item;
//   newNode->next = lp->head;
//   newNode->prev = lp->head->prev;
//   lp->head->prev->next = newNode;
//   lp->head->prev = newNode;
//   lp->count++;
//   return;
// }

// copied
// void *removeFirst(LIST *lp) {
//   assert(lp != NULL && lp->count > 0);
//   NODE *first = lp->head->next;
//   void *data = first->data;
//   lp->head->next = first->next;
//   first->next->prev = first->next;
//   lp->count--;
//   free(first);
//   return data;
// }

// copied
// void *removeLast(LIST *lp) {
//   assert(lp != NULL && lp->count > 0);
//   NODE *last = lp->head->prev;
//   void *data = last->data;
//   lp->head->prev = last->prev;
//   last->prev->next = last->next;
//   lp->count--;
//   free(last);
//   return data;
// }

// copied
// void *getFirst(LIST *lp) {
//   assert(lp != NULL && lp->count > 0);
//   return (lp->head->next->data);
// }

// copied
// void *getLast(LIST *lp) {
//   assert(lp != NULL && lp->count > 0);
//   return (lp->head->prev->data);
// }

// copied
// void *getItem(LIST *lp, int index) {
//   assert(lp != NULL);
//   return lp->head->data[index];
// }

// void setItem(LIST *lp, int index, void *item);
