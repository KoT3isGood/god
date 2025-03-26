#include "stdio.h"

int modules(int argc, char **argv) {
	#define MODULE(x, s) "=== " #x " (" #s ") \n" 
	#define SUB(a, x) " " #x "\n"	


	char* modules = 
	#include "../../compiledmodules.txt"
	;
	printf("Compiled modules:\n%s",modules);
	return 0;
};