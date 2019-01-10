/*
Class: CPSC 346-02
Team Member 1: Connor Cooley
Team Member 2: Michael Newell
GU Username of project lead: ccooley
Pgm Name: proj5.c
Pgm Desc: Peterson Algorithm
Usage: ./a.out
*/

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void parent(int time_crit_sect, int time_non_crit_sect, int* pr_0, int* pr_1, int* turn, int shmid, int shmid1, int shmid2);
void child(int time_crit_sect, int time_non_crit_sect, int* pr_0, int* pr_1, int* turn, int shmid, int shmid1, int shmid2);
void cs(char, int);
void non_crit_sect(int);

void main(int argc, char* argv[])
{
  int tm_child;
  int tm_child_noncs;
  int tm_parent;
  int tm_parent_noncs;
  
  // Create shared memory segment
  int shmid = shmget(0, 1, 0777 | IPC_CREAT);
  int shmid1 = shmget(0, 1, 0777 | IPC_CREAT);
  int shmid2 = shmget(0, 1, 0777 | IPC_CREAT);
  
  // Attach shared memory
  int* pr_0 = (int*) shmat(shmid, 0, 0);
  int* pr_1 = (int*) shmat(shmid1, 0, 0);
  int* turn = (int*) shmat(shmid2, 0, 0);
   
  //check for proper arguments
  if(!(argc == 1 || argc == 5))
  {
  	printf("Error: invalid number of arguments");
  } 
  // Default times
  else if(argc == 1)
  {
  	tm_child = 1;
  	tm_child_noncs = 2;
  	tm_parent = 2;
  	tm_parent_noncs = 1;
  }
  // User input times
  else
  {
    tm_child = atoi(argv[2]);
  	tm_child_noncs = atoi(argv[4]);
  	tm_parent = atoi(argv[1]);
  	tm_parent_noncs = atoi(argv[3]);
  }
  
  //fork here
  int value;
  
  if((value = fork()) < 0)
  {
  	printf("Forking Error");
  }
  else 
  	if (value == 0)
  	{
  	  child(tm_child, tm_child_noncs, pr_0, pr_1, turn, shmid, shmid1, shmid2);
  	}
  	else
  	{
  	  parent(tm_parent, tm_parent_noncs, pr_0, pr_1, turn, shmid, shmid1, shmid2);
  	}
}

void parent(int time_crit_sect, int time_non_crit_sect, int* pr_0, int* pr_1, int* turn, int shmid, int shmid1, int shmid2)
{
  for(int i = 0; i < 10; i++)
  {
  	//protect this
  	*pr_0 = 1;
  	*turn = 1;
  	while(*pr_1 && *turn);
  	cs('p', time_crit_sect);
  	*pr_0 = 0;
  	non_crit_sect(time_non_crit_sect); 
  }
  
  // Detach shared memory
  shmdt(pr_0);
  shmdt(pr_1);
  shmdt(turn);
  
  // Remove shared memory
  shmctl(shmid, IPC_RMID, 0);
  shmctl(shmid1, IPC_RMID, 0);
  shmctl(shmid2, IPC_RMID, 0);
}

void child(int time_crit_sect, int time_non_crit_sect, int* pr_0, int* pr_1, int* turn, int shmid, int shmid1, int shmid2)
{
  for(int i = 0; i < 10; i++)
  {
  	//protect this
  	*pr_1 = 1;
  	*turn = 0;
  	while(*pr_0 && !*turn);
  	cs('c', time_crit_sect);
    *pr_1 = 0;
  	non_crit_sect(time_non_crit_sect); 
  }
  
  // Detach shared memory
  shmdt(pr_0);
  shmdt(pr_1);
  shmdt(turn);
}

void cs(char process, int time_crit_sect)
{
  if (process == 'p')
  {
  	printf("parent in critical sction\n");
  	sleep(time_crit_sect);
  	printf("parent leaving critical section\n");
  }
  else
  {
  	printf("child in critical sction\n");
  	sleep(time_crit_sect);
  	printf("child leaving critical section\n");
  }
}

void non_crit_sect(int time_non_crit_sect)
{
  sleep(time_non_crit_sect);
}