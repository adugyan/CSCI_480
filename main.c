/********************************************************************
CSCI 480 - Assignment 2 - Spring 2021

Progammer: Adu-Gyan Kofi
Section:   
Prof:      Jie Zhou
Date Due:  2/15/2021

Purpose:  To familiarize myself with the use of fork()
and several other system callsto implement a microshell in C/C++,  
and practice FCFS CPU scheduling algorithm. 
*********************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>		// strtok
#include <unistd.h>		// fork, pipe
#include <wait.h>		// wait
#include <stdlib.h>		// malloc, realloc, free, exit

//Statement prototypes
void tokenize(char *input, char **args);
void take_command(char **args);
void ls_sys();
/*******************************************************************************
Function:	void tokenize(char *input, char **args, size_t *new_size)
Use:		Used to tokenize a string, using whitespaces, as delimiters to 
			prepare for use with execvp()
Arguments:	input - string to tokenize
            args  - array of character pointers to place arguments in
            new_size - size of args array
Returns:		Nothing
*******************************************************************************/
void tokenize(char *input, char **args)
{
	int size = 0;
	args[size] = strtok(input, " ");
	while(args[size++] != NULL)
	{	// While not at end of string, continue tokenizing
		args = realloc(args, (size + 1) * sizeof(args[0]));
		//printf("%s\n", args);
		args[size] = strtok(NULL, " ");
	}
	return;
}

/*******************************************************************************
Function:		void single_command(char **args)
Use:				Used to execute a command by itself
Arguments:	**args = A null terminated array of character pointers containing
			the command and arguments you want to run.
Returns:		Nothing
*******************************************************************************/
void take_command(char **args){
	pid_t child = fork();
	if(child < 0)
	{	
		execvp(args[0], args);	
		// Fork failed, print error
		fprintf(stderr, "Process failed to fork\n");
		exit(127);
	}
	if(child == 0)
	{	// Child Process
		if(execvp(args[0], args) < 0)
		{	// execvp failed, print error
			//fprintf(stderr, "couldn't execute: %s", args[0]);
      printf("\n");
			exit(127);
		}
		exit(0);
	}
	wait(0);
	return;
}

/*******************************************************************************
Function:		void totalWaitingTime(int processes[], int n, int bt[], int wt[])
Use:			Used to find the waiting time for all processes.
Arguments:	    int n = # of proccesses
				int processes[] =  processes
				int bt[] = burst time
				int wt[] = wait time. The first process doesn't need to wait so it 
						   will be 0.
Returns:		Nothing
*******************************************************************************/
void totalWaitingTime(int processes[], int n, int bt[], int wt[])
{
	//waiting time for the first process is 0 
	// turn around time - burst time
	wt[0] = 0;

	//calculating waiting time
	for (int i = 1; i < n; i++)
		wt[i] = bt[i - 1] + wt[i-1];
}

/*******************************************************************************
Function:		void avg_time(int processes[], int n, int bt[])
Use:			Used to find the waiting time for all processes.
Arguments:	    int processes[] =  processes
				int bt[] = burst time
				int wt[] = wait time. The first process doesn't need to wait so it 
						   will be 0.
Returns:		Nothing
*******************************************************************************/
void display_time(int processes[], int n, int bt[])
{
	int wt[n], total_wt = 0, total = 0;

	//Function to find waiting time for all processes
	totalWaitingTime(processes, n, bt, wt);

	//display burst times
	for(int i = 0; i < n; i++)
  {
    printf("CPU burst: %d ", bt[i]);
    printf("ms\n");
  }
	//calculating total waiting time
	for (int i=0; i<n; i++)
		total_wt = total_wt + wt[i];

	printf("Total waiting time in the ready queue: %d", total_wt);
  printf(" ms\n");

	total = total_wt / n;
	printf("Average waiting time in the ready queue: %d", total);
  printf(" ms\n");
}

int main(void)
{
    char buf[1024]; //buffer
    pid_t pid;
    int status;
    char *command[2];
    printf("myshell> "); //primes the read
    fgets(buf,1024,stdin); //reads the buffer or 1024 characters from stdin
    buf[strlen(buf)-1] = 0; //removes the last character

    /*Loop for the shell. Will stay open until user enters "quit" */
    while (strcmp(buf, "quit") != 0)  
    {
        char **child1_args = malloc(1 * sizeof(child1_args[0]));
		tokenize(command[0], child1_args);	// Extract 1st command
		if(strcmp(buf, "fcfs") == 0)
		{
			printf("FCFS CPU scheduling simulation with 5 processes.\n");
			//process id's  
      int processes[] = {1, 2, 3, 4, 5};  
      int n = sizeof processes / sizeof processes[0];  
    
      //Burst time of all processes  
      int  burst_time[] = {96, 9, 79, 26, 19};  
    
      display_time(processes, n,  burst_time);    
		}
    else if(strcmp(buf, "ls_sys") == 0)
    {
      ls_sys();
    }
		else if(strcmp(buf, "ls") == 0)
		{
      printf("\n");
      system(buf);
  	}
		else if(strcmp(buf, "ps") == 0)
		{
      printf("\n");
      system(buf);
		}
    else
    {
      printf("couldn't execute: %s", buf);
      printf("\n");
    }

    take_command(child1_args);
		command[0] = command[1] = NULL;
		free(child1_args); //deallocate the allocated memory
		
        // Ask user for new input
		printf("myshell> ");
		fgets(buf, 1024, stdin);
		buf[strlen(buf)-1] = 0;
	}
    return 0;
} 

/*******************************************************************************
Function:		void ls_sys()
Use:			Used to display the results of Assign1
Arguments:	    None
Returns:		Nothing
*******************************************************************************/
void ls_sys()
{
FILE *ostype, *hostname, *osrelease, *version;
    char buffer[BUFSIZ + 1] = {0};
    int chars_read;

    ostype = popen("cat /proc/sys/kernel/ostype", "r");
    hostname = popen("cat /proc/sys/kernel/hostname", "r");
    osrelease = popen("cat /proc/sys/kernel/osrelease", "r");
    version = popen("cat /proc/sys/kernel/version", "r");

    chars_read = fread(buffer, sizeof(char), BUFSIZ, ostype);
    printf("Part A: OS Information\n");
    printf("------------------------\n");
    printf("1. The type of turing OS is %s", buffer);

    chars_read = fread(buffer, sizeof(char), BUFSIZ, hostname);
    printf("2. The hostname is %s", buffer);

    chars_read = fread(buffer, sizeof(char), BUFSIZ, osrelease);
    printf("3. The osrelease is %s", buffer);

    chars_read = fread(buffer, sizeof(char), BUFSIZ, version);
    printf("4. The version number is %s", buffer);

/********************************************************************
Part C
---------------
1. Who is the vendor?
2. What is the model name?
3. What is its physical address size and virtual address size?
*********************************************************************/
    FILE *vendor, *model, *addrSiz;

    vendor = popen("sed -n 2p /proc/cpuinfo", "r"); //print the line containing vendor information
    chars_read = fread(buffer, sizeof(char), BUFSIZ, vendor);
    printf("\nPart C: Processor 0\n");
    printf("------------------------\n");
    printf("1. The vendor information for processor 0 is - %s ", buffer);

    model = popen("sed -n 5p /proc/cpuinfo", "r"); //print the line containing model information
    chars_read = fread(buffer, sizeof(char), BUFSIZ, model);
    printf("2. The model information for processor 0 is -%s ", buffer);

    addrSiz = popen("sed -n 25p /proc/cpuinfo", "r"); //print the line containing model information
    chars_read = fread(buffer, sizeof(char), BUFSIZ, addrSiz);
    printf("3. Address Size information - %s", buffer);

}
