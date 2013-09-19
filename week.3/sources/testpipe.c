#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "pipe.h"

int main(int argc, char** argv){
  char* arg1[] = { "ls", NULL };
  char* arg2[] = { "rev", NULL };

  return pipecmd(arg1[0], arg1, arg2[0], arg2);
}

// vim: set ts=2 tw=2 et:
