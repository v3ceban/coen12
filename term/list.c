#include "list.h"
#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

// defines node
typedef struct node {
  void **data;       // array with data
  int start;         // index of first position in array
  int count;         // num of elements in array
  int length;        // max length of array
  struct node *next; // points to next node
  struct node *prev; // points to previous nodev
} NODE;

// defines list
typedef struct list {
  int count;  // num of items in list (summ of all node->counts)
  NODE *head; // points to sentinel head node
} LIST;

// defines local function to create nodes
static NODE *createNode(NODE *);

LIST *createList(void) {
  LIST *lp = malloc(sizeof(LIST));
  assert(lp != NULL);
  NODE *head = malloc(sizeof(NODE));
  assert(head != NULL);
  lp->head = head;
  head->next = head->prev = lp->head;
  head->data = NULL;
  head->count = head->start = 0;
  head->length = 10;
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

// creates a new node based on heads length and returns a pointer to it
static NODE *createNode(NODE *head) {
  NODE *new = malloc(sizeof(NODE));
  assert(new != NULL);
  // create empty node
  new->count = new->start = 0;
  new->length = head->length;
  new->data = malloc(sizeof(void *) * new->length);
  assert(new->data != NULL);

  // double length for next node
  head->length *= 2;

  return (new);
}

void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  // setup some pointers to make code cleaner
  NODE *head = lp->head;
  NODE *first = head->next;
  // if it's the very first node in the list or if the first node is full
  if (first == head || first->count == first->length) {
    NODE *new = createNode(head);

    // shift pounters around
    new->next = first;
    new->prev = head;
    head->next = first->prev = new;

    // update the first pointer to use later in the function
    first = new;
  }

  first->data[first->start + (first->count % first->length)] = item;
  first->count++;
  lp->count++;

  return;
}

void addLast(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  // setup some pointers to make code cleaner
  NODE *head = lp->head;
  NODE *last = head->prev;
  // if it's the very first node in the list or if the last node is full
  if (last == head || last->count == last->length) {
    NODE *new = createNode(head);

    // shift pounters around
    new->prev = last;
    new->next = head;
    head->prev = last->next = new;

    // update the first pointer to use later in the function
    last = new;
  }

  last->data[last->start + (last->count % last->length)] = item;
  last->count++;
  lp->count++;

  return;
}

// copied
void *removeFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  NODE *first = lp->head->next;
  void *data = first->data;
  lp->head->next = first->next;
  first->next->prev = first->next;
  lp->count--;
  free(first);
  return data;
}

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
