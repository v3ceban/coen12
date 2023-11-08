#include "pqueue.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct pqueue {
  int count;
  int length;
  void **data;
  int (*compare)(void *, void *);
} PQ;

PQ *createQueue(int (*compare)()) {
  // me no create compare. me assert compare
  assert(compare != NULL);

  // me malloc q. me assert q
  PQ *q = malloc(sizeof(PQ));
  assert(q != NULL);

  // initialize empty queue with initial length
  q->count = 0;
  q->length = 10; // will grow if needed
  q->compare = compare;

  // me malloc array. me assert array
  q->data = malloc(q->length * sizeof(void *));
  assert(q->data != NULL);

  return q;
}

void destroyQueue(PQ *pq) {
  // me no create pq. me assert pq
  assert(pq != NULL);
  free(pq->data);
  free(pq);
}

int numEntries(PQ *pq) {
  // me no create pq. me assert pq
  assert(pq != NULL);
  return pq->count;
}

void addEntry(PQ *pq, void *entry) {
  // me no create pq and entry. me assert them
  assert(pq != NULL && entry != NULL);

  // double the size of array if needed
  if (pq->count == pq->length) {
    pq->length *= 2;
    pq->data = realloc(pq->data, pq->length * sizeof(void *));
    // me allocate. me assert
    assert(pq->data != NULL);
  }

  // add entry to the end of array and update count
  pq->data[pq->count] = entry;
  pq->count++;

  /*** HEAP UP ***/
  // get index of new entry and its parent
  int this = pq->count - 1;
  int parent = (this - 1) / 2;

  // swap new entry with its parent until either:
  // a. new entry is root
  // b. new entry is smaller than its parent
  while (this > 0 && pq->compare(pq->data[this], pq->data[parent]) < 0) {
    void *temp = pq->data[this];
    pq->data[this] = pq->data[parent];
    pq->data[parent] = temp;

    // update parent and child to swap
    this = parent;
    parent = (this - 1) / 2;
  }
  /*** END HEAP UP ***/
}

void *removeEntry(PQ *pq) {
  // me no create pq. me assert pq
  assert(pq != NULL && pq->count > 0);

  // save root node's data to return it in the end
  void *root = pq->data[0];

  // replace root's data with the last entry's data
  pq->data[0] = pq->data[pq->count - 1];
  // update number of entries in array
  pq->count--;

  /*** HEAP DOWN ***/
  // get indexes of root and its children
  int this = 0;
  int left = 2 * this + 1;
  int right = 2 * this + 2;

  // go through the whole queue
  while (left < pq->count) {

    // check which child's data is larger: left or right
    int max = left;
    if (right < pq->count && pq->compare(pq->data[right], pq->data[left]) < 0) {
      max = right;
    }

    // compare data in parent and child. swap them if child's data is larger
    if (pq->compare(pq->data[this], pq->data[max]) > 0) {

      void *temp = pq->data[this];
      pq->data[this] = pq->data[max];
      pq->data[max] = temp;

      // update parent and child nodes
      this = max;
      left = 2 * this + 1;
      right = 2 * this + 2;

      // if child's data is smaller the heap is fine
    } else {
      break;
    }
  }
  /*** END HEAP DOWN ***/

  return root;
}
