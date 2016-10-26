#include <stdio.h>
#include "sich/sich.h"


int main(int argc, char* argv[]){
	(void)argc;
	(void)argv;
	
	printf("Benchmarking Sich version '%s' ...\n", sichGetVersion());
	
	return 0;
}

