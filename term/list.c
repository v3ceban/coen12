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

// creates list with sentinel head node
// head holds length of the array in next created node
// Big O complexity: O(1)
LIST *createList(void) {
  LIST *lp = malloc(sizeof(LIST));
  assert(lp != NULL);

  NODE *head = malloc(sizeof(NODE));
  assert(head != NULL);

  lp->head = head;
  head->next = head->prev = lp->head;
  head->data = NULL;
  // this makes head always full, important for insertion functions
  head->count = head->start = head->length = 10;
  lp->count = 0;

  return lp;
}

// destroys list and frees all memory
// Big O complexity: O(n), where n is number of nodes
void destroyList(LIST *lp) {
  assert(lp != NULL);
  NODE *this = lp->head->next;
  while (this != lp->head) {
    NODE *next = this->next;
    free(this->data);
    free(this);
    this = next;
  }
  free(lp->head->data);
  free(lp->head);
  free(lp);
}

// returns the total number of items in list
// Big O complexity: O(1)
int numItems(LIST *lp) {
  assert(lp != NULL);
  return lp->count;
}

// creates a new node based on heads length and returns a pointer to it
// is used in functions that create nodes
// Big O complexity: O(1)
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

// adds an item in the beginning of the list
// BigO complexity: O(1)
void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  // setup some pointers to make code cleaner
  // technically this requires more memory, but not too much to worry about
  // (i.e. memory used by a pointer is negligeble in this use context)
  // the same principle applies in later functions
  NODE *head = lp->head;
  NODE *first = head->next;
  // if it's the very first node in the list or if the first node is full
  // head is always full, so very first node will condition triggers
  if (first->count == first->length) {
    NODE *new = createNode(head);

    // shift pointers around
    new->next = first;
    new->prev = head;
    head->next = first->prev = new;

    // update the first pointer to use later in the function
    first = new;
  }

  // shift start position in the node one left to insert the new item in the
  // very beginning of the node
  first->start = (first->start - 1 + first->length) % first->length;
  first->data[first->start] = item;
  first->count++;
  lp->count++;

  return;
}

// adds a new item in the very end of the list
// BigO complexity: O(1)
void addLast(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  // setup some pointers to make code cleaner
  NODE *head = lp->head;
  NODE *last = head->prev;
  // if it's the very first node in the list or if the last node is full
  if (last->count == last->length) {
    NODE *new = createNode(head);

    // shift pounters around
    new->prev = last;
    new->next = head;
    head->prev = last->next = new;

    // update the first pointer to use later in the function
    last = new;
  }

  last->data[(last->start + last->count) % last->length] = item;
  last->count++;
  lp->count++;

  return;
}

// deletes the first item in the list
// BigO complexity: O(1)
void *removeFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // pointers for clean code
  NODE *head = lp->head;
  NODE *first = head->next;
  // no need to keep the empty node in memory
  if (first->count == 0) {
    head->next = first->next;
    first->next->prev = head;
    free(first->data);
    free(first);
    first = head->next;
  }

  void *data = first->data[first->start];
  first->start = (first->start + 1) % first->length;
  first->count--;
  lp->count--;

  return data;
}

// removes the last item in the list
// BigO complexity: O(1)
void *removeLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // pointers for clean code
  NODE *head = lp->head;
  NODE *last = head->prev;
  // no need to keep the empty node in memory
  if (last->count == 0) {
    head->prev = last->prev;
    last->prev->next = head;
    free(last->data);
    free(last);
    last = head->prev;
  }

  last->count--;
  void *data = last->data[(last->start + last->count) % last->length];
  lp->count--;

  return data;
}

// returns the first item in the list
// BigO complexity: O(1)
void *getFirst(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // pointers for clean code
  NODE *first = lp->head->next;
  void *data = first->data[first->start];

  return data;
}

// returns the last item in the list
// BigO complexity: O(1)
void *getLast(LIST *lp) {
  assert(lp != NULL && lp->count > 0);
  // pointers for clean code
  NODE *last = lp->head->prev;
  void *data = last->data[(last->start + last->count - 1) % last->length];

  return data;
}

// returns the item at given index
// BigO complexity: O(n), where n is the number of nodes
void *getItem(LIST *lp, int index) {
  assert(lp != NULL && index >= 0 && index < lp->count);

  NODE *this = lp->head->next;
  while (index >= this->count) {
    index -= this->count;
    this = this->next;
  }
  void *data = this->data[(this->start + index) % this->length];

  return data;
}

// changes the item at a given index
// BigO complexity: O(n), where n is the number of nodes
void setItem(LIST *lp, int index, void *item) {
  assert(lp != NULL && index >= 0 && item != NULL);

  NODE *this = lp->head->next;
  while (index >= this->count) {
    index -= this->count;
    this = this->next;
  }
  this->data[(this->start + index) % this->length] = item;
}
