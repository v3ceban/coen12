/*
 * File: huffman.c
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Desription: This file contains functions that take an input file and compress
 * it in a different, smaller file with user-specified name using huffman's
 * algorythm.
 *
 * See comments for the functions below for mode detailed desctiption for each
 * of them.
 *
 */

#include "assert.h"
#include "ctype.h"
#include "pack.h"
#include "pqueue.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// Defines the max size of the array
#define SIZE 257

// Defines type for node struct
typedef struct node NODE;

// Defines the functions to be used later
static NODE *makeNode(int, NODE *, NODE *);
static int compare(NODE *, NODE *);
static int depth(NODE *);

// Creates a new node and sets parents for left and right nodes.
// Big O complexity: O(1),
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

// Compares left and right nodes, returns a negative number if left is larger,
// a positive number if right is larger, or 0 if they're equal.
// Big O complexity: O(1),
static int compare(NODE *left, NODE *right) {
  return left->count - right->count;
}

// Calculates and retruns the height of the tree.
// Big O complexity: O(h), where h is height of the tree.
static int depth(NODE *node) {
  int d = 0;
  while (node->parent != NULL) {
    d++;
    node = node->parent;
  }
  return d;
}

// Receives the name of input file and the name of desired output file.
// Compresses the input file and retruns it as output file. Big O complexity:
// O(n), where n is the number of unique characters in the input file.
int main(int argc, char *argv[]) {

  // Open input file for read, and check if it was successfull.
  FILE *inputFile = fopen(argv[1], "r");
  assert(inputFile != NULL);

  // Open output file for write, and check if it was successfull.
  FILE *outputFile = fopen(argv[2], "w");
  assert(outputFile != NULL);

  // contains the number of appearances for each character
  int counts[SIZE] = {0}; // set them all to 0 for now
  // contains all nodes that are present in the file
  NODE *nodes[SIZE] = {NULL}; // set them all to NULL for now
  int i;
  // priority queue that will help us create the tree
  PQ *q = createQueue(compare);

  // count the number of appearances for each character in input file
  while ((i = fgetc(inputFile)) != EOF) {
    counts[i]++;
  }

  // create a node for each character that appeared in the input file
  // add these node into the priority queue
  for (i = 0; i < SIZE; i++) {
    if (counts[i] > 0) {
      nodes[i] = makeNode(counts[i], NULL, NULL);
      addEntry(q, nodes[i]);
    }
  }
  // create an extra node of EOF char and add it to the queue
  nodes[SIZE - 1] = makeNode(counts[SIZE - 1], NULL, NULL);
  addEntry(q, nodes[SIZE - 1]);

  // create the tree out of the nodes by removing two of them and inserting the
  // sum of their counts back as their parent untill there's only one left.
  // this final node will be the root of the tree
  while (numEntries(q) >= 2) {
    NODE *right = removeEntry(q);
    NODE *left = removeEntry(q);
    NODE *parent = makeNode(right->count + left->count, left, right);
    addEntry(q, parent);
  }

  // prints out the number of appearances and saved size for each character
  for (i = 0; i < SIZE; i++) {
    NODE *node = nodes[i];
    if (node != NULL) {
      int height = depth(node);
      // if not printable, print the code of the char
      if (!isprint(i)) {
        printf("%03o: %d x %d bits = %d bits\n", i, node->count, height,
               node->count * height);
        // if printable, print the char itself
      } else {
        printf("'%c': %d x %d bits = %d bits\n", i, node->count, height,
               node->count * height);
      }
    }
  }

  // call the archivation funtion
  pack(argv[1], argv[2], nodes);

  // close the files
  fclose(inputFile);
  fclose(outputFile);

  return 0;
}
