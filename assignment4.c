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
	int fd[2];
	int rd = fd[0], wrt = fd[1];
	if(pipe(fd)) {
		perror("ERROR");
		exit(1);
	}

	/*Builds two argument strings */
	char arg1[255], arg2[255];
	int prsIndex = 1;
	while(strcmp(":", argv[prsIndex])) {
		strcat(arg1, argv[prsIndex]);
		prsIndex++;
	}
	prsIndex++;
	while(prsIndex < argc) {
		strcat(arg2, argv[prsIndex]);
		prsIndex++;
	}

	return 0;
}
