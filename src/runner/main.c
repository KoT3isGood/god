#include "stdio.h"
#include "getopt.h"
#include "god/common.h"
#include "string.h"
#include "commands.h"


struct command_lookup_t {
	char* name;
	int (*command)(int argc, char **argv);
};

struct command_lookup_t commands[] = {
	{"build",build},
	{"rebuild",rebuild}
};


int main(int argc, char **argv) {
	/* Parse arguments. */
	if (argc==1) {
		
		return 0;
	}
	for (int i = 0; i < sizeof(commands)/sizeof(struct command_lookup_t);i++) 
	{
		if (!strcmp(commands[i].name,argv[1])) {
			return commands[i].command(argc-2,argv+2);
		}
	}
	
	return 0;
}