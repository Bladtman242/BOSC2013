/* 

   Opgave 2

   redirect.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "redirect.h"

/* start the program specified by filename with the arguments in argv 
   in a new process that has its stdin redirected to infilename and
   wait for termination */
int redirect_stdincmd(char *filename, char *argv[], char *infilename)
{
  int fid = open(infilename, O_RDONLY);
    close(0);
    dup(fid);
  pid_t child = fork();
  if (child < 0){
    puts("could not obtain PID, fatal error");
    exit(10);
  }
  else if (child == 0){
    //filename is the file to search for and execute, argv contains the filename too, as is convention
     execvp(filename , argv);
  }
  wait(0);
  close(fid);
  return 0;
}

/* start the program specified by filename with the arguments in argv 
   in a new process that has its stdout redirected to outfilename and 
   wait for termination */
int redirect_stdoutcmd(char *filename, char *argv[], char *outfilename)
{
  int fid = open(outfilename, O_WRONLY);
  close(1);
  dup(fid);
  pid_t child = fork();
  if (child < 0) {
    puts("fatal err, could not reproduce");
  }
  else if (child == 0){
    execvp(filename, argv);
  }
  wait(0);
  close(fid);
  return 0;
}

// vim: set ts=2 sw=2 et:
