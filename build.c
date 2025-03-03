#include "include/god/build.h"
#include "include/god/c.h"
#include "include/god/ld.h"
#include "stdio.h"

int build(struct build_data b) {
        struct project god_exe={};
        god_exe.b=&b;
        god_exe.name="god";
        char* god_exe_files[] = {
                "src/runner/build.c",
                "src/runner/help.c",
                "src/runner/main.c",
                "src/common.c",
                NULL
        };
        god_exe.files=god_exe_files;
        struct C_settings settings = {};
        struct project god_obj = C_compile(god_exe,settings);

        struct link_settings sett = {};
        sett.type=LINK_TYPE_EXECUTABLE;
        char* link_libraries[] = {
                "c",
                NULL,
        };
        sett.libs=link_libraries;
        char* exe = ld_link_project(god_obj, sett);

        struct project god_lib={};
        god_lib.b=&b;
        god_lib.name="god";
        char* god_lib_files[] = {
                "src/library/c.c",
                "src/library/ld.c",
                "src/library/master.c",
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

        mv("bin/god_test",exe);
        mv("bin/libgod.a",lib);
        mv("bin/","include"); 

        return 0;
}