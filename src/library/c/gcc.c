
#include "god/utils.h"
#include "god/c.h"
#include "god/build.h"
#include "god/common.h"
#include "stdlib.h"
#include "stdio.h"
#include "libgen.h"
#include "unistd.h"
extern FILE* cdb;
char* gcc_compile(char* file, struct project p, struct C_settings settings) {

	int i = 0;
	struct run_project run;
	char* outputfile = NULL;
	char* outputdir = NULL;
	char* target = NULL;
	if(p.b->kernel==BUILD_KERNEL_LINUX) {
		outputfile = string_clone(".god/o/%i/%s.o",compilecounter,file);
		run = run_new("gcc");
	}
	if(p.b->kernel==BUILD_KERNEL_WINDOWS) {
		outputfile = string_clone(".god/o/%i/%s.obj",compilecounter,file);
		run = run_new("x86_64-w64-mingw32-gcc");
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

	run_add_arg(&run, "-g");
	run_add_arg(&run, "-c");
	run_add_arg(&run, "-fPIC");
	run_add_arg(&run, "-fPIE");
	run_add_arg(&run, "-Wno-incompatible-pointer-types");

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