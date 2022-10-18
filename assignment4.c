/***********************************************************************
name: Travis M. Peterson
	assignment4 -- acts as a pipe using ":" to seperate programs.
description:	
	See CS 360 Processes and Exec/Pipes lecture for helpful tips.
***********************************************************************/

/* Includes and definitions */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
/**********************************************************************
./assignment4 <arg1> : <arg2>

    Where: <arg1> and <arg2> are optional parameters that specify the programs
    to be run. If <arg1> is specified but <arg2> is not, then <arg1> should be
    run as though there was not a colon. Same for if <arg2> is specified but
    <arg1> is not.
**********************************************************************/


int main(int argc, char *argv[]){
	int i;
	int fd[2];
	int rdr, wtr; 
	char **cmd_one = NULL;
	char **cmd_two = NULL;

	if(pipe (fd) < 0) { //Ensure pipe didn't break
		perror("Pipe Broken: ");
	}
	rdr = fd[0]; wtr = fd[1];

	/*Parses user input by vinding the ':' and creating two
	 *null-terminated arrays */
	int cmdFlag = 0;
	cmd_one = &argv[1];
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], ":") == 0) {
			argv[i] = '\0';
			cmdFlag = 1;
			if(i + 1 < argc) {
				cmd_two = &argv[i+1];
			}
			break;
		}
	}

	/* Parent process handles the right side of the command*/
	if(fork()) {
		/*Parent handling this side ensures the program doesn't
		 *end before the child process finishes executing */
		if( cmd_two == NULL) {
			wait(NULL);
			exit(1);
		}
		close(wtr);
		close(0); dup(rdr); close(rdr);
		if(execvp(cmd_two[0], cmd_two) == -1) {
			perror("");
		}
		exit(1);
	} 
	/* Child process handles the left side of the command*/
	else {
		close(rdr) == -1;
		
		if(cmd_two != NULL) {
			close(1); dup(wtr); close(wtr);
		}
		if( execvp(cmd_one[0], cmd_one) == -1) {
			perror("");
		}
		exit(0);
	}
	return 0;
}
