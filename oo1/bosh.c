/* 

   bosh.c : BOSC shell 

 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "parser.h"
#include "print.h"
#include <signal.h>

/* --- symbolic constants --- */
#define HOSTNAMEMAX 100
#define PROMPT "$-> "

/* --- trap sigint --- */
void siginttrap(int signal){
  printf("\nI gots sigint %d\n", signal);
  //exit (0);
}

/* --- use the /proc filesystem to obtain the hostname --- */
char* gethostname(char *hostname){
  FILE* f = fopen("/proc/sys/kernel/hostname", "r");
  char* stat = fgets(hostname, HOSTNAMEMAX, f);
  fclose(f);
  hostname[strlen(hostname)-1]=0;
  return stat;
}

/* --- execute a shell command --- */
//executecmd starts from the last cmd in the cmdline, and moves backwards,
//+hence, the previous command in the cmdline is the next cmd to be processed
//+by executecmd, and vice versa.
//Comments in the function refers to the cmdline when using the terms "next"
//+and previous.
int executecmd (Cmd* cmd, int std_in, int std_out, int bg){
  //if there are no more commands,
  //+close the pipe and return
  //if stdin is set, copy it to stdout before closing
  //+doing it this way is basically shit, I might look into it later.
  if(cmd == NULL){
    if(std_in != 0){
      puts("copying");
      void* buf[1024];
      int size=0;
      do {
        size = read(std_in, buf, 1024);
        write(std_out, buf, size);
        printf("did %d bytes, errno is: %d\n", size, errno);
      } while (size > 0);
      close (std_in);
    }
    close (std_out);
    return 0;
  }

  //make pipe, to bind the previous cmd's stdout to this one's stdin
  int pfds[2];
  pipe(pfds);

  //setup the previous cmds recursively
  //+bg is set to 0, as only the last cmd in the cmdline (first in terms of this function)
  //+should be affected by backgrounding
  executecmd(cmd->next, std_in, pfds[1], 0);

  //fork and exec current cmd
  pid_t child = fork();
  if(child==0){
    //copy the pipe's read part to cmd stdin,
    //+next cmd's
    //+then close the write part of pipe, we don't need it
    signal(SIGINT, siginttrap);
    dup2(pfds[0], 0);
    dup2(std_out, 1);
    close(pfds[1]);
    if(execvp(cmd->cmd[0], cmd->cmd)){
      printf("Error in: %s\n", cmd->cmd[0]);
      if(errno == 2){
        printf("%s not found\n", cmd->cmd[0]);
      }else {
        //there must be a better way of interpreting errnos?
        printf("errno set to: %d", errno);
      }
      exit (errno);
    }
  }

  //if this is the last cmd (indicated by stdout being 1, this should be done
  //+differently) and bg isn't set, wait for the this cmd
  if(std_out == 1 && !bg){
    waitpid(child, NULL, NULL);
  }
  //only close the stdout fd if it isn't the stdout of the program,
  //+otherwise the REPL (yes, I called it REPL) would die
  //+neglecting to close stdout will prevent the reading cmd from terminating,
  //causing the dreaded "ls | cat sort of works, but ls | wc does not" problem to occur
  else if (std_out != 1){
     close(std_out);
  }
  return 0;
}

int executeshellcmd (Shellcmd *shellcmd){
  int std_in = 0;
  if(shellcmd->rd_stdin != NULL){
    std_in = open(shellcmd->rd_stdin, O_RDONLY);
  }

  int std_out = 1;
  if(shellcmd->rd_stdout != NULL){
    std_out = open(shellcmd->rd_stdout, O_WRONLY|O_CREAT);
  }

  executecmd(shellcmd->the_cmds, std_in, std_out, shellcmd->background);
  printshellcmd(shellcmd);
  return 0;
}

/* --- main loop of the simple shell --- */
int main(int argc, char* argv[]) {
  /* initialize the shell */
  char *cmdline;
  char hostname[HOSTNAMEMAX];
  int terminate = 0;
  Shellcmd shellcmd;

  if (gethostname(hostname)) {

    /* parse commands until exit or ctrl-c */
    while (!terminate) {
      printf("%s", hostname);
      if (cmdline = readline(PROMPT)) {
        if(*cmdline) {
          add_history(cmdline);
          if (!strcmp(cmdline, "exit")){
            terminate=1;
          } else if (parsecommand(cmdline, &shellcmd)) {
            terminate = executeshellcmd(&shellcmd);
          }
        }
        free(cmdline);
      } else terminate = 1;
    }
    printf("Exiting bosh.\n");
  }

  return EXIT_SUCCESS;
}

// vim: set ts=2 sw=2 et:
