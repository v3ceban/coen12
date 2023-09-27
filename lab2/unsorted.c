/*
 * File:        unsorted.c
 *
 * Copyright:	2023, Vladimir Ceban
 *
 * Description: This file contains all the functions used in main for testing strins in files data.
 * 
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include "set.h"
# include <assert.h>

typedef struct set
{
   int count;
   int length;
   char** data;
} SET;

static int search (SET *sp, char *elt);


SET* createSet(int maxElts) {
   SET *sp;
   sp = malloc(sizeof(SET));
   assert(sp != NULL);
   sp->count = 0;
   sp->length = maxElts;
   sp->data = malloc(sizeof(char*)*maxElts);
   assert(sp->data != NULL);
   // printf("Set created. Count = %d, Length = %d\n", sp->count, sp->length);
   return sp;
}

void destroySet(SET *sp) {
   assert(sp != NULL);
   int i;
   for (i = 0; i < sp->count; i++)
   {
      free(sp->data[i]);
   }
   free(sp->data);
   free(sp);
}

int numElements(SET *sp) {
   assert(sp != NULL);
   return(sp->count);
}

void addElement(SET *sp, char *elt) {
   assert(sp != NULL);
   if (search(sp, elt) >= 0) {
      return;
   }
   assert(sp->count < sp->length);
      sp->data[sp->count]=strdup(elt);
      sp->count++;
      // printf("Data = %s. New count = %d\n", sp->data[sp->count-1], sp->count);
      return;
}

static int search(SET *sp, char *elt) {
   assert(sp != NULL);
   int i;
   if (sp->count == 0) {
      return -1;
   }
   for (i = 0; i < sp->count; i++)
   {
      if (strcmp(elt, sp->data[i]) == 0) {
         return i;
      }
   }
   return -1;
}

void removeElement(SET *sp, char *elt) {
   assert(sp != NULL);
   int pos = search(sp, elt);
   if (pos < 0) {
      return;
   } else {
      //point current pointer to last pointer
      free(sp->data[pos]);
      sp->data[pos] = sp->data[sp->count-1];
      sp->count--;
      return;
   }
}

char *findElement(SET *sp, char *elt) {
   assert(sp != NULL);
   int pos = search(sp, elt);
   if (pos >= 0) {
      return(sp->data[pos]);
   } else {
      return NULL;
   }
}

char **getElements(SET *sp){
   assert(sp != NULL);
   char **a;
   a = malloc(sizeof(char *)*sp->count);
   assert(a!=NULL);
   return(memcpy(a, sp->data, sizeof(char*)*sp->count));
}
