#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "libgen.h"
#include "../../include/god/c.h"
#include "../../include/god/utils.h"
#include "../common.h"


char* clang_compile(char* file, struct project p, struct C_settings settings);
char* gcc_compile(char* file, struct project p, struct C_settings settings);
char* msvc_compile(char* file, struct project p, struct C_settings settings);

struct project C_compile(struct project p, struct C_settings settings) {

	char* compiler = p.compiler;
	struct project build = {};
	int i = 0;

	int num_files = 0;

	/* Basic checks for libraries */

	if (p.b==NULL) {
		printf(
			"C_compile: "
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			" build system wasn't specified\n"
			);
		return (struct project){};
	};
	if (p.files==NULL) {
		printf(
			"C_compile: "
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			" files weren't specified\n"
			);
		return (struct project){};
	};
	printf(TERMINAL_CYAN"Building %s"TERMINAL_RESET"\n",p.name);

	if (compiler==NULL) {
		compiler="clang";
	}

	do {
		i++;
	} while (p.files[i]);
	num_files=i;
	build.files=malloc(sizeof(char**)*(num_files+1));
	i=0;
	
	do {
		printf(TERMINAL_YELLOW"  [%i/%i] %s "TERMINAL_RESET,i+1, num_files, p.files[i]);
		if (!strcmp(compiler,"clang")) {
			char* file = clang_compile(p.files[i], p, settings);
			build.files[i]=file;
		}
		printf("\n");
		i++;
	} while (p.files[i]);

	build.b=p.b;
	build.name=p.name;


	return build;
}
