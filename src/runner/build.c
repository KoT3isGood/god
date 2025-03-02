#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/stat.h"
#include <sys/wait.h>
#include <libgen.h>
#include "../common.h"

int build(int argc, char **argv) {

        char appdir[1024];
        char *libdir;
        if (!realpath(argv[-2],appdir)) {
                return 1;
        }

        libdir=dirname(appdir);
        const char *link_dir = string_clone("-L%s", libdir);
        const char *include_dir = string_clone("-I%s/include", libdir);
        const char *build_dir = string_clone("%s/include/build.c", libdir);
        const char *cwd = getcwd(NULL,0);
        const char *script_dir = string_clone("%s/.god/build",cwd);
        char *env[] = {"PATH=/usr/bin"};

        time_t source_time=get_modification_time("build.c");
        time_t script_time=get_modification_time(".god/build");

        mkdir(".god",0777);

        int needs = needs_rebuild(script_time, source_time);

        if (needs) {
        remove(script_dir);

        char *args[] = {"/usr/bin/gcc","build.c","-o",".god/build",link_dir, include_dir,"-lgod","--include",build_dir,NULL};

        pid_t gcc = fork();
        if (gcc==-1) return 1; 

        if (gcc==0) {

                if(execve("/usr/bin/gcc", args, env)==-1) {
                        return 1;
                };
        }
        wait(NULL);

        }




        pid_t build_script = fork();
        if (build_script==-1) return 1;
        
        char *script_args[] = {script_dir,NULL};

        if (build_script==0) {
                if(execve(script_dir, script_args, env)!=0) {
                        return 1;
                }
        }
        wait(NULL);

        
        return 0;
};