/* 

   Opgave 3

   pipe.c

 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "pipe.h"

/* create a pipe between two new processes, executing the programs 
   specified by filename1 and filename2 with the arguments in argv1
   and argv2 and wait for termination */
int pipecmd(char *filename1, char *argv1[], char *filename2, char *argv2[])
{
  int pfds[2];
  pipe(pfds);

  pid_t p1 = fork();
  if(p1 == 0){
    //child
    close(1);
    dup2(pfds[1], 1);
    close(pfds[0]);
    execvp(filename1, argv1);
  }

  close(0);
  dup2(pfds[0], 0);
  close(pfds[1]);
  execvp(filename2, argv2);

  return 0;
}

// vim: set ts=2 sw=2 et:
