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
	
	char *cmd = "";
	int i = 0;
	
	printf("Project Shell\n");
	printf("Loading the shell...");

	while(strcmp(cmd,"exit") != 0) {
		printf("/User/Desktop/Project2>>>");
		printf("\n");
		i++;

		if (i == 5) {
			pass;
		}

	}
	return 0;
}
