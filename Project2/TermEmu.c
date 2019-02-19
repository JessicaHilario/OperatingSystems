/*
 * Jessica Hilario 013944279
 * Joel Rodriguez 015222816
 * Assignment 2
 * Due: Feb 25, 2019
 * This program recreates a shell once Aprog is 
 * called and allowing the user to perform some 
 * commands such as listing the contents of the 
 * directory, copying a file to another destination
 * and kill a process.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[]) {
	
	while (true) {
		printf("/User/Desktop/Project2>>");
	}
	int link[2];

	dup2(link[1], STDOUT_FILENO);
	close(link[0]);
	close(link[1]);
	
	if

	return 0;
}

