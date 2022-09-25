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
	char *cmd_one, *cmd_one_flags[argc];
	char *cmd_two, *cmd_two_flags[argc];
	//Needs more extensive error checking
	if(pipe (fd) < 0) {
		exit(1);
	}
	rdr = fd[0]; wtr = fd[1];


	/*I do not like how messy and blocky the following code is
	 * but it does its job. It creates a string for each command
	 * as well as an array of strings for each command including
	 * their flags*/
	int cmdFlag = 0;
	int switchIndex = 0;
	int cIndexOne = 0;
	int cIndexTwo = 0;
	for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i], ":") == 0) {
			cmdFlag = 1;
			switchIndex = i;
		}
		if(i == 1) {
			cmd_one = calloc(1, strlen(argv[i])+1);
			strcpy(cmd_one, argv[i]);
			strcat(cmd_one, "\0");
		} else if(i == switchIndex+1 && cmdFlag == 1) {
			cmd_two = calloc(1, strlen(argv[i])+1);
			strcpy(cmd_two, argv[i]);
			strcat(cmd_one, "\0");
		} if( i > switchIndex && cmdFlag == 1) {
			cmd_two_flags[cIndexTwo] = calloc(1,strlen(argv[i])+1);
			strcpy(cmd_two_flags[cIndexTwo], argv[i]);
			strcat(cmd_two_flags[cIndexTwo], "\0");
			cIndexTwo++;
			cmd_two_flags[cIndexTwo] = NULL;
		} else if( cmdFlag != 1) {
			cmd_one_flags[cIndexOne] = calloc(1,strlen(argv[i])+1);
			strcpy(cmd_one_flags[cIndexOne], argv[i]);
			strcat(cmd_one_flags[cIndexOne], "\0");
			cIndexOne++;
			cmd_one_flags[cIndexOne] = NULL;
		}
	}

	if(cmdFlag != 1) {
		printf("wrong format, please include <arg> : <arg>\n");
	}
	printf("command #1: %s\n", cmd_one);
	for(int i = 0; i < cIndexOne; i++) {
		printf("- %s\n", cmd_one_flags[i]);
	}
	printf("command #2: %s\n", cmd_two);
	for(int i = 0; i < cIndexTwo; i++) {
		printf("- %s\n", cmd_two_flags[i]);
	}


	if(fork()) {

	} else {

	}








	return 0;
}
