#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "libgen.h"
#include "../../include/god/ld.h"
#include "../../include/god/utils.h"
#include "../common.h"
#include <complex.h>


#define TERMINAL_RED "\x1B[31m"
#define TERMINAL_GREENN "\x1B[32m"
#define TERMINAL_YELLOW "\x1B[33m"
#define TERMINAL_BLUE "\x1B[34m"
#define TERMINAL_MAGENTA "\x1B[35m"
#define TERMINAL_CYAN "\x1B[36m"
#define TERMINAL_WHITE "\x1B[37m"
#define TERMINAL_RESET "\x1B[0m"


char* ld_link_project(struct project p, struct link_settings settings) {

	linkcounter++;
	int i = 0;
	int b = 0;
	int argsize = 0;
	char* args=NULL;
	char* libdirs=NULL;
	char* libs=NULL;

	if (p.b==NULL) {
		printf(
			"ld_link_project: "
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			" build system wasn't specified\n"
			);
		return NULL;
	};
	if (p.files==NULL) {
		printf(
			"ld_link_project: "
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			" files weren't specified\n"
			);
		return NULL;
	}
	if (p.name==NULL) {
		printf(
			"ld_link_project: "
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			" project name wasn't specified\n"
			);
		return NULL;
	}
	printf(TERMINAL_CYAN"Linking %s"TERMINAL_RESET"\n",p.name);

	if (settings.type==LINK_TYPE_EXECUTABLE||settings.type==LINK_TYPE_DYNAMIC) {
		char* outputfile=NULL;
		char* linker;
		switch (p.b->kernel) {
		case BUILD_KERNEL_LINUX:
			outputfile=string_clone(".god/bin/%i/%s",linkcounter,p.name);
			linker="g++";
			break;
		case BUILD_KERNEL_WINDOWS:
			outputfile=string_clone(".god/bin/%i/%s.exe",linkcounter,p.name);
			linker="x86_64-w64-mingw32-g++";
			break;
		default:
			break;
		}
		struct run_project run = run_new(linker);
		run_add_arg(&run, "-o");
		run_add_arg(&run, outputfile);
		char* outputdir=string_clone(".god/bin/%i",linkcounter);
		makedir(outputdir);
		if (settings.type==LINK_TYPE_DYNAMIC) {
			run_add_arg(&run, "-shared");
		}
		int i = 0;
		do {
			run_add_arg(&run, p.files[i]);
			i++;
		} while (p.files[i]);
		i=0;
		if (!settings.libs) goto libs;
		do {
			if (settings.libs[i]==(void*)1) {i++;continue;};
			if (!strcmp(settings.libs[i],"c")) {
				if (settings.type==LINK_TYPE_EXECUTABLE &&
					p.b->kernel==BUILD_KERNEL_WINDOWS
				) {
					run_add_arg(&run, "-static");
					run_add_arg(&run, "-static-libgcc");
					run_add_arg(&run, "-static-libstdc++");
				}
				i++;
				continue;
			}
			char* file=string_clone("-l%s",settings.libs[i]);
			run_add_arg(&run, file);
			i++;
		} while (settings.libs[i]);
		libs:
		i=0;	
		if (!settings.lib_dirs) goto lib_dirs;
		do {
			if (settings.libs[i]==(void*)1) {i++;continue;};
			char* file=string_clone("-L%s",settings.lib_dirs[i]);
			run_add_arg(&run, file);
			i++;
		} while (settings.lib_dirs[i]);
		lib_dirs:

		run_run(&run);

		return outputfile;
	}
	if (settings.type==LINK_TYPE_STATIC) {
		struct run_project run = run_new("ar");
		char* outputfile=NULL;
		run_add_arg(&run,"rcs");
		outputfile=string_clone(".god/lib/%i/lib%s.a",linkcounter,p.name);
		char* outputdir = string_clone(".god/lib/%i",linkcounter);
		makedir(outputdir);
		run_add_arg(&run, outputfile);
		int i = 0;
		do {
			run_add_arg(&run, p.files[i]);
			i++;
		} while (p.files[i]);
		run_run(&run);
		
		return outputfile;
	}
	return NULL;
};