#include "stdio.h"
#include "stdlib.h"
#include <sys/wait.h>

//morph function to create morphed programs
int morph(char *number){
	unsigned long num = atoi(number);
	
	return execvp("./numPrimeFactors",num);

}

//main method to read a file and copy the first 10 ints to an array
int main(int argc, char *argv[]) {
	//status variable used for wait
	int status = 0;
	//creates file and opens
	FILE *f;
	f = fopen(argv[1],"r");
	//if no file is given tells you how to use the program
	if (argc < 2){
		printf("Usage: multiFactor fileName");
		return -1;
	}
	//if file is not found
	if (f == NULL){
		printf("No file was found with the name %s, please enter a valid file name as a command line argument",argv[1]);
		return -1;
	}
	//counter for array size
	int count;
	//finds the end of the file
	fseek(f,0,SEEK_END);
	//sets the size of the file to count
	count = ftell(f);
	
	
	
	count = count/sizeof(int);
	//creates the array of the amount of ints in the file
	unsigned int arr[count];

	int total = 0;
	int pid;
	fread(arr, sizeof(int),count,f);
	fclose(f);
	for(int i=0;i<count;i++){
		pid = fork();
			if(pid == 0){
				//child
				morph(&arr[i]);
			}else{
				//parent
				wait(&status);
				if(WIFEXITED(status)){
        				total += WEXITSTATUS(status);
				}
  			}
	}

	printf("Total = %d\n",total);
	return 0;
}
