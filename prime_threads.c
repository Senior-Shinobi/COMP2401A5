// file is prime_threads.c
/********************************************************************/


/****************************************************************/
// Includes

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

/***************************************************************/
// Structures


// holds the thread information for each thread
typedef struct threadData { 
    pthread_t thread_id;			// system thread id
    int tid;                        // inernal id assigned by the program
} ThreadData; 


/******************************************************************/
// Define


#define NUM_THREADS 5


/******************************************************************/
// Function Prototypes

int numPrimeFactors(unsigned long number);

/***************************************************************/
// Global Variables
unsigned long totalNumPrimeFactors = 0;		// global value that holds the final result
unsigned long *recordsToProcess = NULL;     // array that will hold al the numbers to be processed
int numRecords = 0;                         // number of records in the array recordsToProcess
pthread_mutex_t lock;

/*************************************************************/
/*
Purpose compute the number of prime factors of a given number
input:
number - the number to be processed

output: 
None

Return:
the number of prime factors of number
*/
int numPrimeFactors(unsigned long number)
{
    unsigned long i;
    unsigned long quot;
    int numFactors = 0;

    // base case
    if (number == 1) return(0);
    if (number == 2 || number == 3) return(1);

    // check for the first factor until integer is less then number 
    for (i = 2; i <= number; i++) {
        if (i % 1000 == 0) {        
            if (number / i == 1) return(1);   // cannot have any more factrs other than self
        }
        if (number % i == 0) {
            quot = number / i;
            if (quot == 1) return(1);   // cannot have any more factrs other than self
            numFactors = 1 + numPrimeFactors(quot);
            return(numFactors);
        }
    }
    return(0);
}
/*************************************************************************/
//Purpose: the main function of the threads

void *threadMain(/*add parameters as needed */){
	int a = numRecords;
	int count;
	while (a >0) {
		pthread_mutex_lock(&lock);
		count = a-1;
		if (count > -1) {
			
			a-=1;
			pthread_mutex_unlock(&lock);
			totalNumPrimeFactors += numPrimeFactors(recordsToProcess[count]);
		}
	}



    // add code 
}

/*************************************************************************/
int main(int argc, char ** argv)
{
   pthread_t tid[NUM_THREADS];
	
    //creates file and opens
	FILE *f;
	
	f = fopen(argv[1],"r");
	//if no file is given tells you how to use the program
	if (argc < 3){
		printf("Usage: multiThread <fileName> index1 index2...");
		return -1;
	}
	//if file is not found
	if (f == NULL){
		printf("No file was found with the name %s, please enter a valid file name as a command line argument",argv[1]);
		return -1;
	}
    // check command line is correct - if not show how to use the program
    
    // init mutex 
	pthread_mutex_init(&lock,NULL);


	if (strcmp(argv[2],"-1")==0){
	int count;
	//finds the end of the file
	fseek(f,0,SEEK_END);
	//sets the size of the file to count
	count = ftell(f);
	fseek(f,0,SEEK_SET);
	numRecords = count/sizeof(long);
	recordsToProcess=malloc(numRecords*(sizeof(long)));
	fread(recordsToProcess, sizeof(long),numRecords,f);
	
	} else{
    numRecords = argc-2;

    recordsToProcess=malloc(numRecords*(sizeof(long)));
		for (int i = 0; i<numRecords; i++){
			fseek(f,sizeof(long)*(atoi(argv[i+2])),SEEK_SET);
			fread(&recordsToProcess[i], sizeof(long),1,f);
			
		}
	}
    // read the records from the file
	for (int j = 0;j<5;j++){ 
		pthread_create(&tid[j],NULL,threadMain,NULL);
	}
    // wait for the threads to terminate
    printf("May take a while\n");
	for (int j = 0; j<5;j++){
		pthread_join(tid[j],NULL);
	}
    // destroy the mutex 
    pthread_mutex_destroy(&lock);
    
    
    // print the numer of prime factors
	printf("Total is = %lu\n",totalNumPrimeFactors);
}




