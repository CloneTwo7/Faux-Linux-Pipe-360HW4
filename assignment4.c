/***********************************************************************
name:
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
	char **cmd_one;
	char **cmd_two;
	//Needs more extensive error checking
	if(pipe (fd) < 0) {
		perror("Pipe Broken: ");
	}
	rdr = fd[0]; wtr = fd[1];

	int cmdFlag = 0;
	cmd_one = &argv[1];
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], ":") == 0) {
			argv[i] = '\0';
			cmdFlag = 1;
			cmd_two = &argv[i+1];
			break;
		}
	}

	/* Parent process handles the right side of the command*/
	if(fork()) {
		if( cmdFlag != 1) {
			wait(NULL);
			exit(1);
		}
		close(wtr);
		close(0); dup(rdr); close(rdr);
		if(execvp(cmd_two[0], cmd_two) == -1) {
			exit(0);
		}
		exit(1);
	} 
	/* Child process handles the left side of the command*/
	else {
		if (close(rdr) == -1) {
			perror("Cannot close file descriptor");
		}
		if(cmdFlag == 1) {
			close(1); dup(wtr); close(wtr);
		}
		if( execvp(cmd_one[0], cmd_one) == -1) {
			exit(0);
		}
		exit(0);
	}
	return 0;
}
