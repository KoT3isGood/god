#include "string.h"
#include "god/utils.h"
#include "god/common.h"
#include "libgen.h"

char* windres(char* file) {
	compilecounter++;
	struct run_project windres = run_new("x86_64-w64-mingw32-windres");
	run_add_arg(&windres, file);
	char* outputfile = string_clone(".god/windres/%i/%s.obj",compilecounter,file);
	char* outputdir = string_clone("%s",outputfile);
	dirname(outputdir); 
	fix_filename(outputfile);
	makedir(outputdir);
	run_add_arg(&windres, outputfile);
	run_run(&windres);
	return outputfile;
};