/*
Class: CPSC 346
Team Member 1: Connor Cooley - Section 02
Team Member 2: Michael Newell - Section 01
GU Username of project lead: ccooley
Pgm Name: proj7.c
Pgm Desc: Readers-Writers Problem with Linux Pipes and Pthreads
Usage: ./a.out number
*/

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define NUM_THREADS 4

void* writeThread(void*);
void* readThread(void*);
bool checkPrime(int);

struct params{
	int pipe[2];
	int numInt;
	int reader;
};

typedef struct params param;

int main(int argc, char* argv[])
{
	int arr[2];
	param pipes[NUM_THREADS];
	int status, i, number;
	pthread_t threads[NUM_THREADS];
	
	srand(time(NULL));
	pipe(arr);
	
	if(argc != 2)
	{
		printf("error: program needs two arguments\n");
		return 1;
	}
	number = atoi(argv[1]);
	
	for(i = 0; i < NUM_THREADS; i++)
	{
		pipes[i].pipe[0] = arr[0];
		pipes[i].pipe[1] = arr[1];
		pipes[i].numInt = number;
	}
	
	status = pthread_create(&threads[0], NULL, writeThread, (void*)&pipes);
	sleep(1);
	
	for(i = 1; i < NUM_THREADS; i++)
	{
		pipes[i].reader = (void*)i;
		status = pthread_create(&threads[i], NULL, readThread, (void*)&pipes[i]);
	}
	
	for(i = 0; i < NUM_THREADS; i++)
	{
		pthread_join(threads[i], NULL); //wait for thread[i] to complete
	}
	return 0;
}

// Write function
void* writeThread(void* in)
{
	param* pipes = (param*)in;
	int numGen = 0;
	while(numGen < (int)pipes -> numInt)
	{
		int random = rand() % 10000000;
		write(pipes -> pipe[1], &random, sizeof(int));
		numGen++;
		printf("Generated: %d\n", random);
	}
	close(pipes -> pipe[1]);
}

// Read function
void* readThread(void* in)
{
	param* pipes = (param*)in;
	int num;
	
	while((read((int)pipes -> pipe[0], &num, sizeof(int))) > 0)
	{
		sleep(1);
		if(checkPrime(num))
		{
			printf("Reader %d, %d is prime\n", (int)pipes -> reader, num);
		}
	}
	pthread_exit(NULL);
}

bool checkPrime(int num)
{
	int i;
	bool isPrime = true;
	
	for(i = 2; i <= num / 2; i++)
	{
		if(num % i == 0)
		{
			isPrime = false;
			break;
		}
	}
	return isPrime;
}