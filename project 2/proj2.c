/*
Class: CPSC 346-02
Team Member 1: Connor Cooley
Team Member 2: N/A 
GU Username of project lead: ccooley
Pgm Name: proj2.c
Pgm Desc: Counts the words in a file
Usage: ./a.out
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE 80

char* get_inp();

int wc(char*);

int main()
{
 char* inp = get_inp();
 int count = wc(inp);
 printf("%d\n", count);
 free(inp);  //return dynamially allocated memory to the heap
 return 0;
}

char* get_inp()
{
 char* inp = (char*) malloc(MAX_LINE);
 char* start = inp;
 char c;

 printf("Enter text <= 80 characters in length\n");
 while ((c = getchar()) != '\n')
  *inp++ = c; //weird, yes? First add the character to the dereffed pointer
              //then go to the next position in dynamic memory 
 *inp = '\0'; 
 return start;
}

/*
pre:  inp is the address of an 80 byte block of memory 
post: returns the number of words stored in the block of memory.  A word
      is any sequence of characters that 
      1) are stored at the beginning of the block or 
      2) are stored at end of the block or
      3) are terminated by the end-of-line character or
      4) are preceded by and ended by one or more spaces
        
*/

int wc(char* inp)
{
  int i = 0;
  int wordCount = 0;
  while(inp[i] != '\0')
  {
    while(inp[i] == ' ')
	{
	  i++;
	}
	
    if(inp[i] >= 'A' && inp[i] <= 'Z' || inp[i] >= 'a' && inp[i] <= 'z' || inp[i] >= '0' && inp[i] <= '9')
	{
      wordCount++;
	  while(inp[i] >= 'A' && inp[i] <= 'Z' || inp[i] >= 'a' && inp[i] <= 'z' || inp[i] >= '0' && inp[i] <= '9')
	  {
	    i++;
	  }
	  while(inp[i] == ' ')
	  {
	    i++;
	  }
	}
  }
  return wordCount;
}
