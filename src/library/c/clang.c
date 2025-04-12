#include "god/utils.h"
#include "god/c.h"
#include "god/build.h"
#include "god/common.h"
#include "stdlib.h"
#include "stdio.h"
#include "libgen.h"
#include "unistd.h"
#include "string.h"
extern FILE* cdb;
char* clang_compile(char* file, struct project p, struct C_settings settings) {

	int i = 0;
	struct run_project run;
	char* outputfile = NULL;
	char* outputdir = NULL;
	char* target = NULL;
	if(p.b->kernel==BUILD_KERNEL_LINUX) {
		outputfile = string_clone(".god/o/%i/%s.o",compilecounter,file);
		target=string_clone("x86_64-pc-linux-gnu");
	}
	if(p.b->kernel==BUILD_KERNEL_WINDOWS) {
		outputfile = string_clone(".god/o/%i/%s.obj",compilecounter,file);
		target=string_clone("x86_64-pc-win32-gnu");
	}
	fix_filename(outputfile);
	fix_filename(file);
	char rebuild = needs_rebuild(get_modification_time(outputfile),get_modification_time(file));
	if (!rebuild) {
		printf(TERMINAL_BLUE"(cached)"TERMINAL_RESET);
		if (cdb) {
			goto build;
		};
		return outputfile;
	}
	build:
	run = run_new("clang");

	run_add_arg(&run, "-g");
	if (p.b->kernel==BUILD_KERNEL_WINDOWS) {
		run_add_arg(&run, "-gdwarf-2");
	}
	run_add_arg(&run, "-c");
	if (settings.generation_flags&C_GENERATION_FLAGS_PIC) {
		run_add_arg(&run, "-fPIC");
	}
	if (settings.generation_flags&C_GENERATION_FLAGS_PIE) {
		run_add_arg(&run, "-fPIE");
	}
	run_add_arg(&run, "-target");
	run_add_arg(&run, target);

	const char* extension = get_file_extension(file);
	if (!stricmp(extension,"c"))
		goto c_version_select;
	else if (!stricmp(extension,"cpp"))
		goto cpp_version_select;
	else if (!stricmp(extension,"c++"))
		goto cpp_version_select;
	else {
		printf(
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			"%s is not supported\n", 
			extension
			);
		return outputfile;
	}

c_version_select:
	switch (settings.cversion) {
	case C_VERSION_C89:
		run_add_arg(&run, "-std=c89");
		break;
	case C_VERSION_C99:
		run_add_arg(&run, "-std=c99");
		break;
	case C_VERSION_C11:
		run_add_arg(&run, "-std=c11");
		break;
	case C_VERSION_C17:
		run_add_arg(&run, "-std=c17");
		break;
	case C_VERSION_C23:
		run_add_arg(&run, "-std=c23");
		break;
	case C_VERSION_C2Y:
		run_add_arg(&run, "-std=c2y");
		break;
	default:
		break;
	}
	goto version_selected;

cpp_version_select:
	switch (settings.cppversion) {
	case CPP_VERSION_CPP98:
		run_add_arg(&run, "-std=c++98");
		break;
	case CPP_VERSION_CPP11:
		run_add_arg(&run, "-std=c++11");
		break;
	case CPP_VERSION_CPP14:
		run_add_arg(&run, "-std=c++14");
		break;
	case CPP_VERSION_CPP17:
		run_add_arg(&run, "-std=c++17");
		break;
	case CPP_VERSION_CPP20:
		run_add_arg(&run, "-std=c++20");
		break;
	case CPP_VERSION_CPP23:
		run_add_arg(&run, "-std=c++23");
		break;
	case CPP_VERSION_CPP2C:
		run_add_arg(&run, "-std=c++2c");
		break;
	default:
		break;
	}
	goto version_selected;
version_selected:

	if (settings.compile_flags&C_COMPILE_FLAGS_WERROR) {
		run_add_arg(&run, "-Werror");
	}
	if (settings.compile_flags&C_COMPILE_FLAGS_WALL) {
		run_add_arg(&run, "-Wall");
	}


	outputdir=string_clone("%s",outputfile);
	dirname(outputdir);
	makedir(outputdir);
	run_add_arg(&run, "-o");
	run_add_arg(&run, outputfile);

	if (!settings.include_dirs) goto include_dirs;
	do {
		char* dir=string_clone("-I%s",settings.include_dirs[i]);
		run_add_arg(&run, dir);
		i++;
	} while (settings.include_dirs[i]);
include_dirs:
	i=0;
	if (!settings.include_files) goto includes_files;
	do {
		run_add_arg(&run, "--include");
		run_add_arg(&run, settings.include_files[i]);
		i++;
	} while (settings.include_files[i]);
	i=0;
includes_files:
	if (!settings.macros) goto defines;
	do {
		run_add_arg(&run, "-D");
		if (settings.macros->value) {
			char* m = string_clone("%s=%s",settings.macros[i].name,settings.macros[i].value);
			run_add_arg(&run, m);
		} else {
			run_add_arg(&run, settings.macros[i].name);
		}
		i++;
	} while (settings.macros[i].name);
	i=0;
defines:

	run_add_arg(&run, file);
	if (rebuild) {
		run_run(&run);
	}

	if (cdb) {
		char cwd[1024];
    		getcwd(cwd, sizeof(cwd));
		fprintf(cdb,
			",\n"
			"\t{\n"
			"\t\t\"arguments\":[\n"
		);
		int h = 0;
		do {
			fprintf(cdb,"\t\t\t\"%s\"",run.args[h]);
			h++;
			if(run.args[h]) {
				fprintf(cdb,",\n");
			} else {
				fprintf(cdb,"\n");
			};
		} while (run.args[h]);

		fprintf(cdb,
			"\t\t],\n"
			"\t\t\"file\":\"%s\",\n"
			"\t\t\"directory\":\"%s\"\n"
			"\t}",file,cwd);
	}

	return outputfile;
}
