#include "forback.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	int i = 0;
	while (i != argc) {
		printf("%s ", argv[i++]);
	}
	puts("\n");
	int stat = backgroundcmd(argv[1], argv +1);
	return stat;
}

