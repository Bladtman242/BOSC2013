#include "redirect.h"
#include <stdio.h>

int main (int argc, char** argv){
  return redirect_stdoutcmd(argv[1], argv+2, argv[2]);
}

// vim: set ts=2 sw=2 et:
