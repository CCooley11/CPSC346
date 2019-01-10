/*
Class: CPSC 346-02
Team Member 1: Connor Cooley
Team Member 2: Michael Newell
GU Username of project lead: ccooley
Pgm Name: proj4.c
Pgm Desc: Outline of a shell
Usage: ./a.out
*/

#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define TRUE 80

char** getInput();
char** parseInput(char*);
void dispOutput(char**);
void displayHistory(char***);
void execute(char** args);
char ***history;
int entry = 0;

int main(int argc, char* argv[])
{
  //A pointer to an array of pointers to char.  In essence, an array of 
  //arrays.  Each of the second level arrays is a c-string. These hold
  //the command line arguments entered by the user.
  //as exactly the same structure as char* argv[]
  char **args;
  history = (char***) malloc(1000);
  
  while(TRUE)
  {
    printf("myShell> ");
    fflush(stdout);
    args = getInput();
    if(strcmp(args[0], "quit") == 0)
	{
	  break;
	} else if(strcmp(args[0], "history") == 0)
	{
	  displayHistory(history);
	} else if (strcmp(args[0], "!!") == 0)
	{
	  int ind = entry % 10;
	  execute(history[ind - 1]);
	} else if (strncmp(args[0],"!",1) == 0)
	{
	  int n = entry % 10;
	  char* temp = args[0];
	  temp++;
	  int num;
	  num = atoi(temp);
	  num = num - 1;
	  if(entry <= 10)
	  {
	    if(num > entry)
		{
	      printf("N exceeds number of commands saved\n");
		}
		else
		{
		  execute(history[num]);
		}
	  }
	  else 
	  {
	    if(num > 10)
		{
	      printf("N exceeds number of commands saved\n");
		}
		else
		{
		  int indx = (num + n) % 10; 
		  execute(history[indx]);
		}
	  }
	} else {
	  execute(args);
	}
  }
  return 0;
}

/*
Reads input string from the key board.   
invokes parseInput and returns the parsed input to main
*/
char** getInput()
{
  char* input = (char*) malloc(MAX_LINE);
  char* start = input;
  char **output;
  char c;
  
  while((c = getchar()) != '\n')
  {
    *input++ = c;
  }
	*input = '\0';
  
  output = parseInput(start);
  return output;
}  

/*
inp is a cstring holding the keyboard input
returns an array of cstrings, each holding one of the arguments entered at
the keyboard. The structure is the same as that of argv
Here the user has entered three arguements:
myShell>cp x y
*/ 
char** parseInput(char* inp)
{	
  char **parsedInput = (char**) malloc(5);
  int argNumber = 0;
  char* nextArg = (char*) malloc(MAX_LINE);
  char c;
  int i = 0;
  int count;
  
  while(inp[i] != '\0')
  {
	count = i;
    while(inp[i] != ' ' && inp[i] != '\0')
	{
	  c = inp[i];
	  nextArg[i] = c;
	  i++;
	}
	i++;
	count = i - count;
	nextArg[i] = '\0';
	if(argNumber > 0)
	{
	  parsedInput[argNumber] = nextArg + i - count;
	}
	else
	{
      parsedInput[argNumber] = nextArg;
	}
	argNumber++;
  }
  //Check meta-commands...
  if(strcmp(parsedInput[0], "history") != 0 && 
  strncmp(parsedInput[0], "!", 1) != 0)
  {
	int ind = entry % 10;
    history[ind] = parsedInput;
    entry++;
  }
  return parsedInput;
} 
 
/*
Displays the arguments entered by the user and parsed by getInput
*/
void displayHistory(char*** args)
{
  int ind = entry % 10;
  int i = 0;
  int j = 0;
  
  if(entry < 10)
  {
    for(i = 0; i < ind; i++)
    {
	  printf("%d -> ", i + 1);
      while(args[i][j] != '\0')
      {
	    printf("%s ", args[i][j]);
	    j++;
      }
	  j = 0;
	  printf("\n");
    }
  }
  else if(entry == 10)
  {
    for(i = 0; i < 10; i++)
    {
	  printf("%d -> ", i + 1);
      while(args[i][j] != '\0')
      {
	    printf("%s ", args[i][j]);
	    j++;
      }
	  j = 0;
	  printf("\n");
    }
  }
  else
  {
	int n = entry % 10;
	int count = 1;
    for(i = n; i < 10; i++)
    {
	  printf("%d -> ", count++);
      while(args[i][j] != '\0')
      {
	    printf("%s ", args[i][j]);
	    j++;
      }
	  j = 0;
	  printf("\n");
    }
	for(i = 0; i < n; i++)
    {
	  printf("%d -> ", count++);
      while(args[i][j] != '\0')
      {
	    printf("%s ", args[i][j]);
	    j++;
      }
	  j = 0;
	  printf("\n");
    }
  }
}

void execute(char** args)
{
	pid_t pid;
    pid = fork();
    if (pid < 0)  //fork failed
    {
      fprintf(stderr, "Fork Error");
    } 
    else
      if (pid == 0)  //child process
        execvp(args[0],args);  //load args[0] into the child's address space 
      else           //parent process
      {
        wait(NULL);
      }
}