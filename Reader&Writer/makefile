#
# PROGRAM:    4
# PROGRAMMER: Kofi Adu-Gyan
# LOGON ID:   z1723260
# DATE DUE:   03/20/21
#

# Compiler variables
CXXFLAGS = -Werror -Wall -g -std=c++14 -pthread

# Rule to link object code files to create executable file
z1723260_project4: main.o
	gcc -pthread -o z1723260_project4 main.o 

# Rules to compile source code files to object code
main.o: main.c
	gcc -pthread -c main.c

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o z1723260_project4

