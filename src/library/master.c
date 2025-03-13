#include "../../include/god/utils.h"
#include "../common.h"
#include "stdlib.h"
#include "stdio.h"

int linkcounter = 0;
int compilecounter = 0;
char trace = 1;
void mv(char* dest, char* source) {
	#ifdef __WIN64__

	char* command = string_clone("xcopy /E /I %s %s",source,dest);
	#endif
	#ifdef __linux__
	char* command = string_clone("cp -r %s %s",source,dest);
	#endif
	system(command);
};