#include "../../../include/god/utils.h"
#include "../../../include/god/c.h"
#include "../../../include/god/build.h"
#include "../../common.h"
#include "stdlib.h"
#include "stdio.h"
#include "libgen.h"
char* clang_compile(char* file, struct project p, struct C_settings settings) {

	int i = 0;
	struct run_project run;
	char* outputfile = NULL;
	char* outputdir = NULL;
	if(p.b->kernel==BUILD_KERNEL_LINUX) {
		outputfile = string_clone(".god/o/%s.o",file);
	}
	if(p.b->kernel==BUILD_KERNEL_WINDOWS) {
		outputfile = string_clone(".god/o/%s.obj",file);
	}
	fix_filename(outputfile);
	fix_filename(file);
	char rebuild = needs_rebuild(get_modification_time(outputfile),get_modification_time(file));
	if (!rebuild) {
		printf(TERMINAL_BLUE"(cached)"TERMINAL_RESET);
		return outputfile;
	}
	run = run_new("clang");

	run_add_arg(&run, "-g");
	run_add_arg(&run, "-c");
	run_add_arg(&run, "-fPIC");
	run_add_arg(&run, "-fPIE");



	outputdir=string_clone("%s",outputfile);
	dirname(outputdir);
	makedir(outputdir);
	run_add_arg(&run, "-o");
	run_add_arg(&run, outputfile);

	if (!settings.include_dirs) goto include_dirs;
	do {
		char* file=string_clone("-I%s",settings.include_dirs[i]);
		run_add_arg(&run, file);
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


	run_run(&run);
	return outputfile;
}