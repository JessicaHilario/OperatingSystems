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
#include <sys/wait.h>
#include <sys/stat.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while(0);

#ifdef __unix__
	#define IS_POSIX 1
#else
	#define IS_POSIX 0
#endif

//Check the ls -l command
int check_ls(const char* cmd) {
	//Should put const in front of them since we aren't writing to them
	const char *target = "ls-l";
	char tmp[4];
	
	//i will be the index for cmd
	int i = 0;
	//j will be the index for tmp
       	int j = 0;
	
	//Puts all the letters from the command line into a temp array
	while (i < strlen(cmd)) {
		if(*(cmd + i) != ' ') {
			tmp[j] = *(cmd + i);
			j += 1;
		}

		if(j == 4) {
			break;
		}
		i += 1;
	}
	
	return memcmp(tmp, target, 4) == 0;
}

//Checks the exit command
int check_exit(const char* cmd) {
	const char *target = "exit";
	char tmp[4];
	
	//i will be the index for cmd
	int i = 0;
	//j will be the index for tmp
       	int j = 0;
	
	//Puts all the letters from the command line into a temp array
	while (i < strlen(cmd)) {
		if(*(cmd + i) != ' ') {
			tmp[j] = *(cmd + i);
			j += 1;
		}

		if(j == 4) {
			break;
		}
		i += 1;
	}
	
	return memcmp(tmp, target, 4) == 0;
}

//Checks the cp command
int check_cp(const char* cmd) {
	const char *target = "cp";
	char tmp[2];
	
	//i will be the index for cmd
	int i = 0;
	//j will be the index for tmp
       	int j = 0;
	
	//Puts all the letters from the command line into a temp array
	while (i < strlen(cmd)) {
		if(*(cmd + i) != ' ') {
			tmp[j] = *(cmd + i);
			j += 1;
		}

		if(j == 2) {
			break;
		}
		i += 1;
	}
	
	return memcmp(tmp, target, 2) == 0;
}

//Checks the kill command
int check_kill(const char* cmd) {
	const char *target = "kill";
	char tmp[4];
	
	//i will be the index for cmd
	int i = 0;
	//j will be the index for tmp
       	int j = 0;
	
	//Puts all the letters from the command line into a temp array
	while (i < strlen(cmd)) {
		if(*(cmd + i) != ' ') {
			tmp[j] = *(cmd + i);
			j += 1;
		}

		if(j == 4) {
			break;
		}
		i += 1;
	}
	
	return memcmp(tmp, target, 4) == 0;
}

//Checks the cd command
int check_cd(const char* cmd) {
	const char *target = "cd";
	char tmp[2];
	
	//i will be the index for cmd
	int i = 0;
	//j will be the index for tmp
       	int j = 0;
	
	//Puts all the letters from the command line into a temp array
	while (i < strlen(cmd)) {
		if(*(cmd + i) != ' ') {
			tmp[j] = *(cmd + i);
			j += 1;
		}

		if(j == 2) {
			break;
		}
		i += 1;
	}
	
	return memcmp(tmp, target, 2) == 0;
}

char ** split(const char* cmd, char c, int *n) {
	int strLen = strlen(cmd);	
	char ** tmp = (char**) malloc(sizeof(char*)*(strLen/2)+1); //Creates the size needed in bytes
	int counter = 0;
	
	//Points to the start of the string
	for (int start = 0; start < strLen; start ++) {

		if (cmd[start] != c) {

			//Points to the end of the string
			for (int end = start; end < strLen; end ++) {

				//If the string contains c, allocate memory and copy the memory size
				if (cmd[end] == c || end == strLen - 1) {
					int size = end - start + 1;//Getting the size of the string
					char* add = (char*) malloc(size);//Allocating memory for the string
					memcpy(add, cmd+start, size);//Copying the memory address
					add[size-1] = '\0';//Make the last charatcer of the string a null character
					tmp[counter++] = add;
					start = end;
					break;
				}
			}
		}
	}

	*n = counter;
	tmp = realloc(tmp, sizeof(char*)*counter); //Resizes the block of memory to the new size of memory
	return tmp;
}

//Clear up the allocated memory
void free_string_array(char **array, int len)
{
	for (int i = 0; i < len; i++) {
		free(array[i]);
	}
	free(array);
}

//Validates and executes the commands
void execute_command(char cmdin[]) {
	int link[2];
	pid_t pid;
	struct stat sb;

	//Checked in the parent process to actually exit and change the directory
	if (check_exit(cmdin)) {
		exit(0);
	}

	if (check_cd(cmdin)) {
		int argc;
		char** argv = split(cmdin,' ', &argc);

		if (chdir(argv[1]) != 0) {
			printf("No such directory\n");
		}

		free_string_array(argv, argc);

		return;
	}

	if(pipe(link)==-1) {
		die("pipe");
	}

	if((pid=fork()) == -1) {
		die("fork");
	}

	if(pid == 0) {
		if (check_ls(cmdin)) {
			int argc; 
			char** argv = split(cmdin,' ', &argc);
			
			execl("/bin/ls", argv[0], argv[1], (char *)0);
			die("execl");
			free_string_array(argv, argc);

			return;
		}
		if (check_cp(cmdin)) {
			int argc; 
			char** argv = split(cmdin,' ', &argc);

			if (stat(argv[1], &sb) == 0) {
				execl("/bin/cp", argv[0], argv[1], argv[2], (char *) 0);
				die("execl");
			}
			else { 
				perror("stat");
				exit(EXIT_FAILURE);
			}
			free_string_array(argv, argc);

			return;
		}
		if (check_kill(cmdin)) {
			int argc;
			char** argv = split(cmdin,' ', &argc);
			
			execl("/bin/kill", argv[0], argv[1], (char *)0);
			free_string_array(argv, argc);

			return;
		}
		else {
			printf("Not a valid command\n");
		}
		exit(0);
	}	

}

int main(int argc, char *argv[]) {


	char cmd[1000];
	while(strcmp(cmd,"exit")!=0) {
		if (IS_POSIX == 1) {
			char *dir = getcwd(NULL, 0);
			printf("%s>>> ", dir);
			free(dir);
			fgets(cmd, 1000, stdin);
			execute_command(cmd);
			wait(NULL);
		}
	}
	return 0;
}
