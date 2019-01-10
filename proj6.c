/*
Class: CPSC 346
Team Member 1: Connor Cooley - Section 02
Team Member 2: Michael Newell - Section 01
GU Username of project lead: ccooley
Pgm Name: proj6.c
Pgm Desc: Producer/Consumer Problem with Linux Semaphores
Usage: ./a.out
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <wait.h>

#define SEMKEY 0
typedef struct sembuf sem_buf;

#define PROD 0
#define CONS 1

sem_buf sem_wait[1], sem_signal[1];

int semF, semE, semM;

void producer();
void consumer();
void criticalSection(int who);

int main()
{  
  //set the buffer values for wait.
  sem_wait[0].sem_num = 0;
  sem_wait[0].sem_op = -1; //decrement
  sem_wait[0].sem_flg = SEM_UNDO;   
  
  //set the buffer values for signal 
  sem_signal[0].sem_num = 0;
  sem_signal[0].sem_op = 1; //increment
  sem_signal[0].sem_flg = SEM_UNDO;
  
  short int in[1];
  
  // Full
  in[0] = 0;
  semF = semget(SEMKEY, 1, 0777 | IPC_CREAT);
  semctl(semF, 1, SETALL, in);
  
  // Empty
  in[0] = 100;
  semE = semget(SEMKEY + 1, 1, 0777 | IPC_CREAT);
  semctl(semE, 1, SETALL, in);
  
  // Mutex
  in[0] = 1;
  semM = semget(SEMKEY + 2, 1, 0777 | IPC_CREAT);
  semctl(semM, 1, SETALL, in);
  
  short int mutex[1], empty[1], full[1];
  semctl(semM, 1, GETALL, mutex);
  semctl(semE, 1, GETALL, empty);
  semctl(semF, 1, GETALL, full);
  
  printf("Initial semaphore values: mutex = %d, empty = %d, full = %d\n", mutex[0], empty[0], full[0]);
 
  int value;
  if((value = fork()) < 0)
    printf("Child could not be created.\n");
  else
  {
    if(value > 0)
      producer();
    else
      consumer();
  }
  
  if(value > 0)
    wait(NULL);
  else
  {
   semctl(semM, 1, GETALL, mutex);
   semctl(semE, 1, GETALL, empty);
   semctl(semF, 1, GETALL, full);
   printf("Final semaphore values: mutex = %d, empty = %d, full = %d\n", mutex[0], empty[0], full[0]);
   semctl(semF, 1, IPC_RMID, NULL);
   semctl(semE, 1, IPC_RMID, NULL);
   semctl(semM, 1, IPC_RMID, NULL);
  }
  
  return 0;
}
 
void producer()
{
  int i;
  for(i = 0; i < 5; i++)
  {
    semop(semE, sem_wait, 1);
    semop(semM, sem_wait, 1);
    criticalSection(PROD);
    semop(semM, sem_signal, 1);
    semop(semF, sem_signal, 1); 
	//sleep(1);
  }
  
}
 
void consumer()
{
  int i;
  for (i = 0; i < 5; i++)
  {
    semop(semF, sem_wait, 1);
    semop(semM, sem_wait, 1);
    criticalSection(CONS);
    semop(semM, sem_signal, 1);
    semop(semE, sem_signal, 1); 
  }
}

void criticalSection(int who)
{
  if (who == PROD)
    printf("Producer making an item\n");
  else
    printf("Consumer consuming an item\n");
 }
