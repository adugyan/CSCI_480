/***********************************************************************
CSCI 480 - Assignment 4 - Spring 2021
Progammer: Adu-Gyan Kofi
Section:   
Prof:      Jie Zhou
Date Due:  3/20/2021
Purpose:   To solve the Reader-Writer problem using the PThreadslibrary 
as  well as practice using semaphores in the PThreadslibrary.
*************************************************************************/
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//buffersize
#define BUFFER_SIZE 10

//variables number of readers, writers, and iterations
int numReader;
int numWriter;
int iterations;

//initialize string for readers and writers
char quote[] = "All work and no play makes Jack a dull boy";

//semaphore and mutex init
sem_t  rw_sem;//used by both readers and writers
sem_t cs_sem;//used for protecting critical sections of readers
pthread_mutex_t mutex; //mutex lock
int readerFlag;

/*******************************************************************************
Function:	void *writer(void *param)
Use:		Will write to the shared string
Arguments:	*param
Returns:	Nothing
*******************************************************************************/
void *writer(void *param)
{
    
    //pthread_mutex_lock(&mutex); //acquire the mutex lock
    //loop for NUM_ITERATIONS
    for (size_t  i = 0; i < iterations; i++)
    {
        sem_wait(&rw_sem);
        //writing (chopping the last character of the string
        int size = strlen(quote);
        
        quote[size] = quote[size-1] = '\0'; //strings end in \0. Will remove last char
        
        //print out a message saying that it is writing
        printf("Writer is writing\n");

        sem_post(&rw_sem);
        
        //sleep for 1 second
        sleep(1);
    }

    //pthread_mutex_unlock(&mutex);
    //exit
    pthread_exit(NULL);

}

/*******************************************************************************
Function:	void *reader(void *param)
Use:		Will read the shared string and print it's contents to
            the standard output.
Arguments:	*param
Returns:	Nothing
*******************************************************************************/
void *reader(void *param)
{
    int value; 
    // Reader acquire the lock before modifying numreader
    //pthread_mutex_lock(&mutex);
    //readerFlag++;
    /*if(readerFlag == 1) 
    {
       sem_wait(&rw_sem);
    }*/
    pthread_mutex_unlock(&mutex);
    // Reading Section
    for (size_t  i = 0; i < iterations; i++)
    {
    readerFlag++;
    pthread_mutex_unlock(&mutex);
    if(readerFlag == 1) 
    {
       sem_wait(&rw_sem);
    }
    printf("%s\n", quote);
    fflush(stdout); 
    sleep(1);

    // Reader acquire the lock before modifying numReader
    pthread_mutex_lock(&mutex);
    readerFlag--;
    if(readerFlag == 0) {
        sem_getvalue(&rw_sem, &value);
        printf("The initial value of the semaphore is %d\n", value);
        
        sem_post(&rw_sem); // If this is the last reader, it will wake up the writer.
        sleep(1);
        sem_getvalue(&rw_sem, &value);
        printf("The value of the semaphore after the post is %d\n", value);
        sleep(1);
    }
    //pthread_mutex_unlock(&mutex);

    }
    pthread_mutex_unlock(&mutex);

    //exit
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Get command line arguments
    if(argc<=1) {
        //Default if no arguments are entered.
        numReader = 1;
        numWriter = 1;
        iterations = 43;
        
    }   
    else
    {
        //argv[0] is the program name 
        //atoi = ascii to int
        numReader = atoi(argv[1]); 
        numWriter = atoi(argv[2]);  
        iterations = atoi(argv[3]); 

        //block to test command line functionality
        printf("The # of command line args is: %d\n", argc - 1);
        printf("Number of readers '%s'\n", argv[1]);
        printf("Number of writers '%s'\n", argv[2]);
        printf("Number of iterations '%s'\n", argv[3]);
    
    }
    
    
    
    //Initialize semaphores to 1
    sem_init(&rw_sem, 0,1);  
    sem_init(&cs_sem, 0, 1);
    
    //Initialize mutex and thread arrays
    pthread_t readers[numReader],writers[numWriter];
    pthread_mutex_init(&mutex, NULL);
    
    //Create reader and writer threads
    // Iterate and create producers
    for (size_t i = 0; i < numWriter; i++) 
    {
        if(pthread_create(&writers[i], NULL, writer, (void*) i) < 0)
        {
            //printf("Something went wrong, now exiting...\n");
            exit(-1);
        }
    }

    // Iterate and create consumers
    for (size_t i = 0; i < numReader; i++) 
    {
        if(pthread_create(&readers[i], NULL, reader, (void*) i) < 0)
        {
            //printf("Something went wrong, now exiting...\n");
            exit(-1);
        }
    }
  
    /*Clean up and exit*/
    // Terminate writer threads
    for (size_t i = 0; i < numWriter; i++) 
    {
        if(pthread_join(writers[i], NULL) < 0)
        {
            //printf("Something went wrong, now exiting...\n");
            exit(-1);
        }
    }

    // Terminate reader threads
    for (size_t i = 0; i < numReader; i++) 
    {
        if(pthread_join(readers[i], NULL) < 0)
        {
            //printf("Something went wrong, now exiting...\n");
            exit(-1);
        }
  }

    // Kill all semaphores and mutexes
    if(sem_destroy(&rw_sem) < 0) 
    {
        //printf("Something went wrong, now exiting...\n");
        exit(-1);
    }

    if(sem_destroy(&cs_sem) < 0) 
    {
        //printf("Something went wrong, now exiting...\n");
        exit(-1);
    }

    if(pthread_mutex_destroy(&mutex) != 0)
    {
        //printf("Something went wrong, now exiting...\n");
        exit(-1);
    }

    pthread_exit(NULL);
    return 0;
}