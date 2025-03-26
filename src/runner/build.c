#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/stat.h"
#ifdef __WIN32__
#include "windows.h"
#endif
#include "libgen.h"
#include "god/common.h"
#include "god/utils.h"
#include "god/build.h"
#include "god/c.h"
#include "god/ld.h"
#include "unistd.h"

int build(int argc, char **argv) {
	/* traverse cwd down the files until we find build.c */
	#ifdef __linux__
	char buildwd[1024];
	getcwd(buildwd,1024);
	while (strcmp("/",buildwd)) {
		chdir(buildwd);
		FILE* buildc = fopen("build.c", "r");
		if (buildc) {
			fclose(buildc);
			break;
		};
		dirname(buildwd);
	}
	#endif


	char appdir[1024];
	struct build_data b;
	#ifdef __linux__
	ssize_t len = readlink("/proc/self/exe", appdir, sizeof(appdir) - 1);
	if (len == -1) {
		return 1;
	}
	appdir[len] = '\0';
	b.arch = BUILD_ARCH_x86_64;
	b.kernel = BUILD_KERNEL_LINUX;
	#endif
	#ifdef __WIN32__
	WORD len = GetModuleFileName(NULL, appdir, MAX_PATH);
	if (!len) {
		return 1;
	}
	b.arch = BUILD_ARCH_x86_64;
	b.kernel = BUILD_KERNEL_WINDOWS;
	#endif
	compilecounter=-1;
	linkcounter=-1;

	struct project build_exe={};
	build_exe.b=&b;
	build_exe.name="build";
	char* build_exe_files[] = {
		"build.c",
		NULL
	};
	char* libdir=dirname(appdir);
        const char* include_file = string_clone("%s/include/build.c", libdir);
        const char* include_dir = string_clone("%s/include", libdir);
	char* build_dir = string_clone("%s",appdir);
	char* build_include_files[] = {
		include_file,
		NULL
	};
	char* build_includes[] = {
		include_dir,
		NULL
	};
	build_exe.files=build_exe_files;
	struct C_settings settings = {};
	settings.include_files=build_include_files;
	settings.include_dirs=build_includes;
	struct project build_obj = C_compile(build_exe,settings);

	struct link_settings sett = {};
	sett.type=LINK_TYPE_EXECUTABLE;
	char* link_libraries[] = {
		"c",
		"god",
		NULL,
	};
	char* link_dirs[] = {
		libdir,
		NULL,
	};
	sett.libs=link_libraries;
	sett.lib_dirs=link_dirs;
	char* buildfile = ld_link_project(build_obj, sett);

	int argsize = 0;

	for (int i = 0; i<argc; i++) {
		argsize+=strlen(argv[i])+1;
	}
	char* buildargs = malloc(argsize);
	for (int i = 0; i<argc; i++) {
		strcat(buildargs,argv[i]);
		strcat(buildargs," ");
	}
	buildargs[argsize]=0;

	#ifdef __linux__
	const char* command = string_clone(".god/build %s\n",buildargs);
	mv(".god/build",buildfile);
	#endif
	#ifdef __WIN64__
	const char* command = string_clone(".god\\build.exe %s\n",buildargs);
	mv(".god\\build.exe",buildfile);
	#endif

	system(command);

	
	return 0;
};
int rebuild(int argc, char** argv) {
	system("rm -rf .god");
	return build(argc,argv);
}