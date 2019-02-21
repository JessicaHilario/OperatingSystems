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

//Function Declarations for builtin shell commands:
 
int chd(char **args);
int exit_shell(char **args);

//List of builtin commands, followed by their functions.
 
char *builtin_str[] = {
	"cd",
	"exit"
};

int (*builtin_func[]) (char **) = {
	chd,
	exit_shell
};

int num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}


//Implementing built-in functions
//changing the directory
int chd(char **args)
{
//args[1] is the directory, checks if the directory is null
  if (args[1] == NULL) {
    fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	  
  } 
  else {
    //checks if chdir(args[1]) is the "cd" command
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
//returns 1 to continue executing the program
  return 1;
}

//exit built-in command
int exit_shell(char **args)
{
  //returns 0 to terminate execution
  return 0;
}

//launching a program and terminating it
int launch(char **args)
{
  pid_t pid, wpid;
  int status;
  //creating a fork
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } 
	
  else if (pid < 0) {
    // Error forking
    perror("lsh");
  } 
	
  else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
	  
  }
  //returning 1 to continue program execution
  return 1;
}

//executes command or launches a program
int execute_command(char **args)
{
  int i;

  //checking if they entered nothing in "cd"
  //returns 1 to continue running, 0 to terminate
  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return launch(args);
}

#define RL_BUFSIZE 1024

//reads a line of input and stores it in a pointer
char *read_line(void)
{
  int bufsize = RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"


//split a line into tokens
char **split_line(char *line)
{
  int bufsize = TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

//main
int main(int argc, char **argv)
{
  
  //running the problem
	char *line;
	char **args;
	int status;
	printf("Project Shell\n");
	printf("Loading the shell...\n\n");

	//continually prints out the shell while the program is running
	do {
		printf("/User/Desktop/Project2>>>");
		line = read_line();
		args = split_line(line);
		status = execute_command(args);

		free(line);
		free(args);
	} while (status);

  return EXIT_SUCCESS;
}
