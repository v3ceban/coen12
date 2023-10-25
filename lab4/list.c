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

#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list {
  int count;
  struct node *head;
  int (*compare)(void *elt1, void *elt2);
} LIST;

typedef struct node {
  void *data;
  struct node *next;
  struct node *prev;
} NODE;

LIST *createList(int (*compare)(void *elt1, void *elt2)) {
  LIST *list = (LIST *)malloc(sizeof(LIST));
  assert(list != NULL);
  NODE *head = (NODE *)malloc(sizeof(NODE));
  assert(head != NULL);
  list->head = head;
  head->next = head->prev = head;
  list->compare = compare;
  list->count = 0;
  return list;
}

void destroyList(LIST *lp) {
  assert(lp != NULL);
  NODE *current, *next;
  current = lp->head->next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  free(lp->head);
  free(lp);
}

int numItems(LIST *lp) {
  assert(lp != NULL);
  return (lp->count);
}

void addFirst(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  NODE *newNode = (NODE *)malloc(sizeof(NODE));
  assert(newNode != NULL);
  NODE *head = lp->head;
  newNode->data = item;

  newNode->next = head->next;
  newNode->prev = head;
  head->next->prev = newNode;
  head->next = newNode;
  lp->count++;
}

void addLast(LIST *lp, void *item) {
  assert(lp != NULL && item != NULL);
  NODE *newNode = (NODE *)malloc(sizeof(NODE));
  assert(newNode != NULL);
  NODE *head = lp->head;
  newNode->data = item;

  newNode->next = head;
  newNode->prev = head->prev;
  head->prev->next = newNode;
  head->prev = newNode;
  lp->count++;
}

void *removeFirst(LIST *lp) {
  assert(lp != NULL);
  if (lp->count == 0) {
    return NULL;
  }
  NODE *head = lp->head;
  NODE *first = head->next;
  void *data = first->data;

  head->next = first->next;
  first->next->prev = first->next;

  lp->count--;
  free(first);
  return data;
}

void *removeLast(LIST *lp) {
  assert(lp != NULL);
  if (lp->count == 0) {
    return NULL;
  }
  NODE *head = lp->head;
  NODE *last = head->prev;
  void *data = last->data;

  head->prev = last->prev;
  last->prev->next = last->next;

  lp->count--;
  free(last);
  return data;
}

void *getFirst(LIST *lp) {
  assert(lp != NULL);
  if (lp->count == 0) {
    return NULL;
  }
  return (lp->head->next->data);
}

void *getLast(LIST *lp) {
  assert(lp != NULL);
  if (lp->count == 0) {
    return NULL;
  }
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
