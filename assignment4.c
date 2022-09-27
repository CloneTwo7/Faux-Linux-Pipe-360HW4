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

#include <assert.h>

/**********************************************************************
./assignment4 <arg1> : <arg2>

    Where: <arg1> and <arg2> are optional parameters that specify the programs
    to be run. If <arg1> is specified but <arg2> is not, then <arg1> should be
    run as though there was not a colon. Same for if <arg2> is specified but
    <arg1> is not.
**********************************************************************/


int main(int argc, char *argv[]){
	int i, n;
	int fd[2];
	int rdr, wtr; 
	char buff[255];
	char *cmd_one[argc];
	char *cmd_two[argc];
	//Needs more extensive error checking
	if(pipe (fd) < 0) {
		exit(1);
	}
	rdr = fd[0]; wtr = fd[1];

	/*I do not like how messy and blocky the following code is
	 * but it does its job. It creates an array of strings for 
	 * each command argument including their flags*/
	int cmdFlag = 0;
	int switchIndex = 0;
	int cIndexOne = 0;
	int cIndexTwo = 0;
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], ":") == 0) {
			cmdFlag = 1;
			switchIndex = i;
		}
		if( i > switchIndex && cmdFlag == 1) {
			cmd_two[cIndexTwo] = calloc(1,strlen(argv[i])+1);
			strcpy(cmd_two[cIndexTwo], argv[i]);
			strcat(cmd_two[cIndexTwo], "\0");
			cIndexTwo++;
			cmd_two[cIndexTwo] = NULL;
		} else if( cmdFlag != 1) {
			cmd_one[cIndexOne] = calloc(1,strlen(argv[i])+1);
			strcpy(cmd_one[cIndexOne], argv[i]);
			strcat(cmd_one[cIndexOne], "\0");
			cIndexOne++;
			cmd_one[cIndexOne] = NULL;
		}
	}

	/* Parent process handles the right side of the command
	 * */
	if(fork()) {
		close(wtr);
		close(0); dup(rdr); close(rdr);
		char *cmd = calloc(1, strlen(cmd_two[0]) + 6);
		strcat(cmd, "/bin/");
		strcat(cmd, cmd_two[0]);
		strcat(cmd,"\0");
		wait(NULL);
		execvp(cmd, cmd_two);
		exit(1);
		
	} else { //Child
		close(rdr);
		close(1); dup(wtr); close(wtr);
		//allocate enough space for the cmd plus '/bin/' and null terminator
		char *cmd = calloc(1, strlen(cmd_one[0]) + 6); 
		strcat(cmd, "/bin/");
		strcat(cmd, cmd_one[0]);
		strcat(cmd, "\0");
		execvp(cmd, cmd_one);
		exit(0);
	}

	return 0;
}
