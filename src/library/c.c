#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "libgen.h"
#include "god/c.h"
#include "god/utils.h"
#include "god/common.h"


char* clang_compile(char* file, struct project p, struct C_settings settings);
char* gcc_compile(char* file, struct project p, struct C_settings settings);
char* msvc_compile(char* file, struct project p, struct C_settings settings);

const char* C_buildcargs[] = {
	"clang",
	"build.c",
	"--include",
	NULL,
	NULL,
};
FILE* cdb;
struct project C_compile(struct project p, struct C_settings settings) {
	char* compiler = p.compiler;
	struct project build = {};
	int i = 0;

	int num_files = 0;
	compilecounter++;

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
		if (i%2)
			printf(TERMINAL_YELLOW"  CC       %s\n"TERMINAL_RESET, p.files[i]);
		else
			printf(TERMINAL_WHITE"  CC       %s\n"TERMINAL_RESET, p.files[i]);
		if (!strcmp(compiler,"clang")) {
			char* file = clang_compile(p.files[i], p, settings);
			build.files[i]=file;
		}
		#ifndef minimalcc
		if (!strcmp(compiler,"gcc")) {
			char* file = gcc_compile(p.files[i], p, settings);
			build.files[i]=file;
		}
		#endif
		i++;
	} while (p.files[i]);

	build.b=p.b;
	build.name=p.name;
	build.files[num_files] = 0;
	


	return build;
}
