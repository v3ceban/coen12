#include "assert.h"
#include "pack.h"
#include "pqueue.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <ctype.h>

#define SIZE 257

typedef struct node NODE;

static NODE *makeNode(int, NODE *, NODE *);
static int compare(NODE *, NODE *);
static int depth(NODE *);

static NODE *makeNode(int count, NODE *left, NODE *right) {
  NODE *newNode = malloc(sizeof(NODE));
  assert(newNode != NULL);
  newNode->parent = NULL;
  newNode->count = count;
  if (left != NULL) {
    left->parent = newNode;
  }
  if (right != NULL) {
    right->parent = newNode;
  }
  return newNode;
}

static int compare(NODE *left, NODE *right) {
  return left->count - right->count;
}

static int depth(NODE *node) {
  int d = 0;
  while (node->parent != NULL) {
    d++;
    node = node->parent;
  }
  return d;
}

int main(int argc, char *argv[]) {

  FILE *inputFile = fopen(argv[1], "r");
  assert(inputFile != NULL);

  FILE *outputFile = fopen(argv[2], "w");
  assert(outputFile != NULL);

  int counts[SIZE] = {0};
  NODE *nodes[SIZE] = {NULL};
  int i;
  PQ *q = createQueue(compare);

  while ((i = fgetc(inputFile)) != EOF) {
    counts[i]++;
  }

  for (i = 0; i < SIZE; i++) {
    if (counts[i] > 0) {
      nodes[i] = makeNode(counts[i], NULL, NULL);
      addEntry(q, nodes[i]);
    }
  }
  nodes[SIZE - 1] = makeNode(counts[SIZE - 1], NULL, NULL);
  addEntry(q, nodes[SIZE - 1]);

  while (numEntries(q) >= 2) {
    NODE *right = removeEntry(q);
    NODE *left = removeEntry(q);
    NODE *parent = makeNode(right->count + left->count, left, right);
    addEntry(q, parent);
  }

  for (i = 0; i < SIZE; i++) {
    NODE *node = nodes[i];
    if (node != NULL) {
      int height = depth(node);
      if (!isprint(i)) {
        printf("%03o: %d x %d bits = %d bits\n", i, node->count, height,
               node->count * height);
      } else {
        printf("'%c': %d x %d bits = %d bits\n", i, node->count, height,
               node->count * height);
      }
    }
  }

  pack(argv[1], argv[2], nodes);

  fclose(inputFile);
  fclose(outputFile);

  return 0;
}
