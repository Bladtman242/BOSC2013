/*

   Opgave 1

   forback.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include "forback.h"

/* start the program specified by filename with the arguments in argv 
   in a new process and wait for termination */
int foregroundcmd(char *filename, char *argv[])
{
	pid_t child = fork();
	if (child < 0){
		puts("could not obtain PID, fatal error");
		exit(10);
	}
	else if (child == 0){
		//filename is the file to search for and execute, argv contains the filename too, as is convention
		execvp(filename , argv);
	}
	else{
		wait(0);
	}
	return 0;
}

/* start the program specified by filename with the arguments in argv
   in a new process */
int backgroundcmd(char *filename, char *argv[])
{
	pid_t child = fork();
	if (child < 0){
		puts("could not obtain PID, fatal error");
		exit(10);
	}
	else if (child == 0){
		//filename is the file to search for and execute, argv contains the filename too, as is convention
		puts("");
		execvp(filename , argv);
		puts("");
	}
	return 0;
}

