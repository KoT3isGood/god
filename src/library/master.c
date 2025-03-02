#include "../../include/god/utils.h"
#include "../common.h"
#include "stdlib.h"
#include "stdio.h"

int linkcounter = 0;
int compilecounter = 0;
void mv(char* dest, char* source) {
        char* command = string_clone("cp -r %s %s",source,dest);
        printf("command:%s\n",command);
        system(command);
};