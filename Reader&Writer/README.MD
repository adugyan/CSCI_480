The program will create several reader threads and several writer threads. Each thread loops for several
iterations for writing or reading. The numbers of threads and the number of iterations need to be passed 
in as command line arguments for the program -- So your program takes in three command line 
arguments: number of reader threads, number of writer threads, and number of iterations in a 
reader/writer thread, in that order. If no arguments are passed the defaults will be: readers = 1, 
writers = 1, iterations = 43. 

The readers read a shared string and print the contents to the standard output. The writers write to the 
same string. 

The code will initialize a shared string, which is a global variable shared by all threads. You can 
initialize the shared string in any way. For demonstration, I set it to “All work and no play 
makes Jack a dull boy.”

The main() function will do the initialization to initialize the semaphores (which are also shared by all 
threads). It also creates the separate reader and writer threads, with number of threads based on 
command line arguments.
