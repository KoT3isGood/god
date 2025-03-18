#include "god/build.h"
#include "god/c.h"
#include "god/ld.h"
#include "god/utils.h"
#include "stddef.h"
#include "stdio.h"

int build(struct build_data b) {

	char* includedirs[] = {
		"include",
		NULL,
	};

        struct C_settings settings = {};
	settings.include_dirs=includedirs;

        struct project god_lib={};
        god_lib.b=&b;
        god_lib.name="god";
        char* god_lib_files[] = {
                "src/library/C/clang.c",
                "src/library/c.c",
                "src/library/ld.c",
                "src/library/master.c",
                "src/library/windres.c",
                "src/common.c",
                NULL,
        };
        god_lib.files=god_lib_files;
        struct project god_lib_obj = C_compile(god_lib,settings);

        struct link_settings lib_sett = {};
        lib_sett.type=LINK_TYPE_STATIC;
        char* lib_link_libraries[] = {
                "c",
                NULL,
        };
        lib_sett.libs=lib_link_libraries;
        char* lib = ld_link_project(god_lib_obj, lib_sett);

        mv("bin",lib);




        struct project god_exe={};
        god_exe.b=&b;
        god_exe.name="god_test";
        char* god_exe_files[] = {
                "src/runner/build.c",
                "src/runner/help.c",
                "src/runner/main.c",
                "src/common.c",
                NULL
        };
        god_exe.files=god_exe_files;
        struct project god_obj = C_compile(god_exe,settings);

        struct link_settings sett = {};
        sett.type=LINK_TYPE_EXECUTABLE;
        char* link_libraries[] = {
                "c",
		"god",
                NULL,
        };
        char* link_dirs[] = {
		"bin",
		NULL,
	};
        sett.libs=link_libraries;
        sett.lib_dirs=link_dirs;
        char* exe = ld_link_project(god_obj, sett);




        mv("bin",exe);
        mv("bin/","include"); 

        return 0;
}