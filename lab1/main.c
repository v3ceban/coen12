/*

Lab 1
Fall 2023
Name: Vladimir Ceban

*/

//Includes
#include <stdio.h>
#include <stdlib.h>

//Defines
#define MAX_WORD_LENGTH 30
int countFileWords(char *);
int main(int, char *[MAX_WORD_LENGTH]);

//Functions
int countFileWords(char *fileName){
   FILE *file;
   int x = 0;
   char word[MAX_WORD_LENGTH];
   if ((file = fopen(fileName, "r")) == NULL) {
      printf("File not found\n");
      return -1;
   }
   else {
      while (fscanf(file, "%s", word) == 1)
      {
         x++;
      }
      printf("%d", x);
      return x;
   }
}

int main (int argc, char **argv) {
   if (argc < 2) {
      printf("Please specify a file to read. (i.e. ./main ./scrathc/Macbeth.txt)\n");
      return 1;
   }
   if (countFileWords(argv[1])>=0) {
      printf (" total words\n");
      return 0;
   } else {
      printf("Your file couldn't be opened for some reason.\n");
      return 1;
   }
}