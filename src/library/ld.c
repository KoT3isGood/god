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
	char* libccancel = "";

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
	char libcpresent=1;
	if (p.b->kernel==BUILD_KERNEL_WINDOWS) {
		libcpresent=0;
	}

	/* Get number of files. */
	do {
		argsize+=strlen(p.files[i])+1;
		i++;
	} while (p.files[i]);
	b=i;
	i = 0;
	argsize+=1;
	args = malloc(argsize);
	memset(args,0,argsize);

	do {
	       strcat(args,p.files[i]); 
	       strcat(args," "); 
	       i++;
	} while (p.files[i]);
	

	if (settings.lib_dirs==NULL) {
		libdirs="";
		goto skip_libdirs;
	};
	i=0;
	argsize=0;
	do {
		argsize+=strlen(settings.lib_dirs[i])+3;
		i++;
	} while (settings.lib_dirs[i]);
	i = 0;
	argsize+=1;
	libdirs = malloc(argsize);
	memset(libdirs,0,argsize);

	do {
		strcat(libdirs," -L"); 
		strcat(libdirs,settings.lib_dirs[i]); 
		i++;
	} while (settings.lib_dirs[i]);

skip_libdirs:


	if (settings.libs==NULL) {
		libs="";
		goto skip_libs;
	};
	argsize=0;
	i=0;
	do {
		if (settings.libs[i]==(void*)1) {i++;continue;};
		if (p.b->kernel==BUILD_KERNEL_WINDOWS) {
			if (!strcmp(settings.libs[i],"c"))  {
				libcpresent=1;
				i++;
				continue;
			};
		}
		argsize+=strlen(settings.libs[i])+3;
		i++;
	} while (settings.libs[i]);
	i = 0;
	argsize+=1;
	libs = malloc(argsize);
	memset(libs,0,argsize);

	do {
		if (settings.libs[i]==(void*)1) {i++;continue;};
		if (p.b->kernel==BUILD_KERNEL_WINDOWS) {
			if (!strcmp(settings.libs[i],"c")) {
				i++;
				continue;
			};
		}
		strcat(libs," -l"); 
		strcat(libs,settings.libs[i]); 
		i++;
	} while (settings.libs[i]);

skip_libs:
	if (trace) {
		printf(TERMINAL_CYAN"Linking %s"TERMINAL_RESET": ",p.name);
	}

	if (!libcpresent) {
		libccancel="-nostdlib";
	}
	if (settings.type==LINK_TYPE_EXECUTABLE) {
		char* outputfile;
		
		switch (p.b->kernel) {
		case BUILD_KERNEL_LINUX:
			outputfile=string_clone(".god/bin/%i/%s",linkcounter,p.name);
			break;
		case BUILD_KERNEL_WINDOWS:
			outputfile=string_clone(".god/bin/%i/%s.exe",linkcounter,p.name);
			break;
		}
		fix_filename(outputfile);
		char* command = NULL;
		if (settings.linker==NULL) goto ld_default_link;

		if (!strcmp(settings.linker,"g++")) {
			ld_default_link:
			switch (p.b->kernel) {
			case BUILD_KERNEL_WINDOWS:
				command=string_clone(
					"x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ -static " 
					"%s"
					"%s%s "
					"-o %s",args,libdirs,libs,outputfile);
				break;
			case BUILD_KERNEL_LINUX:
				command=string_clone(
					"g++ " 
					"%s"
					"%s%s "
					"-o %s",args,libdirs,libs,outputfile);
				break;
			default:
				printf("not implemented4\n");
			}
			goto ld_link;
		}
ld_link:
		if (trace) {
		      printf("%s\n",command);
		}
		#ifdef __WIN64__
		char* realcommand = string_clone("if not exist .god\\lib\\%i mkdir .god\\lib\\%i\n%s",linkcounter,linkcounter,command);
		#endif
		#ifdef __linux__
		char* realcommand = string_clone("mkdir -p .god/bin/%i && %s",linkcounter,command);
		#endif
		system(realcommand);

		return outputfile;
	}
	if (settings.type==LINK_TYPE_STATIC) {
		char* outputfile;
		switch (p.b->kernel) {
		case BUILD_KERNEL_LINUX:
			outputfile = string_clone(".god/lib/%i/lib%s.a",linkcounter,p.name);
			break;
		case BUILD_KERNEL_WINDOWS:
			outputfile = string_clone(".god/lib/%i/%s.lib",linkcounter,p.name);
			break;
		default:
			printf("not implemented2\n");
		}
		fix_filename(outputfile);
		char* command=string_clone(
			"ar rcs %s %s",outputfile,args
		);
		printf("%s\n",command);
		#ifdef __WIN64__
		char* realcommand = string_clone("if not exist .god\\lib\\%i mkdir .god\\lib\\%i\n%s",linkcounter,linkcounter,command);
		#endif
		#ifdef __linux__
		char* realcommand = string_clone("mkdir -p .god/lib/%i && %s",linkcounter,command);
		#endif
		system(realcommand);
		return outputfile;
	}
	if (settings.type==LINK_TYPE_DYNAMIC) {
		char* outputfile;
		
		switch (p.b->kernel) {
		case BUILD_KERNEL_LINUX:
			outputfile=string_clone(".god/lib/%i/%s.so",linkcounter,p.name);
			break;
		case BUILD_KERNEL_WINDOWS:
			outputfile=string_clone(".god/lib/%i/%s.dll",linkcounter,p.name);
			break;
		}
		fix_filename(outputfile);
		char* command = NULL;
		if (settings.linker==NULL) goto ld_default_link_so;

		if (!strcmp(settings.linker,"g++")) {
			switch (p.b->kernel) {
			case BUILD_KERNEL_LINUX:
				command=string_clone(
					"x86_64-w64-mingw32-g++ -shared " 
					"%s"
					"%s%s "
					"-o %s",args,libdirs,libs,outputfile);
				break;
			case BUILD_KERNEL_WINDOWS:
				command=string_clone(
					"g++ -shared " 
					"%s"
					"%s%s "
					"-o %s",args,libdirs,libs,outputfile);
				break;
			default:
				printf("not implemented4\n");
			}
			goto ld_link;
		}
ld_default_link_so:
		switch (p.b->kernel) {
		case BUILD_KERNEL_LINUX:
			command=string_clone(
				"g++ -shared " 
				"%s"
				"%s%s "
				"-o %s",args,libdirs,libs,outputfile);
			break;
		case BUILD_KERNEL_WINDOWS:
			command=string_clone(
				"x86_64-w64-mingw32-g++ -shared " 
				"%s"
				"%s%s %s "
				"-o %s",args,libdirs,libs,libccancel,outputfile);
			break;
		default:
			printf("not implemented3\n");
		}
ld_link_so:
		if (trace) {
		      printf("%s\n",command);
		}
		#ifdef __WIN64__
		char* realcommand = string_clone("if not exist .god\\lib\\%i mkdir .god\\lib\\%i\n%s",linkcounter,linkcounter,command);
		#endif
		#ifdef __linux__
		char* realcommand = string_clone("mkdir -p .god/lib/%i && %s",linkcounter,command);
		#endif
		system(realcommand);

		return outputfile;
	}
	return NULL;
};