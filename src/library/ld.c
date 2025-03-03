#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "libgen.h"
#include "../../include/god/ld.h"
#include "../../include/god/utils.h"
#include "../common.h"


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
        printf("%s\n",libdirs);

skip_libdirs:


        if (settings.libs==NULL) {
                libs="";
                goto skip_libs;
        };
        argsize=0;
        i=0;
        do {
                argsize+=strlen(settings.libs[i])+3;
                i++;
        } while (settings.libs[i]);
        i = 0;
        argsize+=1;
        libs = malloc(argsize);
        memset(libs,0,argsize);

        do {
                strcat(libs," -l"); 
                strcat(libs,settings.libs[i]); 
                i++;
        } while (settings.libs[i]);

skip_libs:
        printf(TERMINAL_CYAN"Linking %s"TERMINAL_RESET": ",p.name);
        if (settings.type==LINK_TYPE_EXECUTABLE) {
                char* outputfile = string_clone(".god/bin/%s_%i",p.name,linkcounter);
                char* command = NULL;
                if (settings.linker==NULL) goto ld_default_link;
                if (!strcmp(settings.linker,"g++")) {
                        command=string_clone(
                                "mkdir -p .god/bin && "
                                "g++ " 
                                "%s"
                                "%s%s "
                                "-o %s",args,libdirs,libs,outputfile);
                        printf("%s\n",command);
                        system(command);
                        goto ld_done_link;
                }
ld_default_link:
                command=string_clone(
                        "mkdir -p .god/bin && "
                        "ld "
                        "$(gcc -print-file-name=crt1.o) "
                        "$(gcc -print-file-name=crti.o) "
                        "%s"
                        "$(gcc -print-file-name=crtn.o) "
                        " -dynamic-linker /lib64/ld-linux-x86-64.so.2 -m elf_x86_64%s%s "
                        "-o %s",args,libdirs,libs,outputfile);
                printf("%s\n",command);
                system(command);

ld_done_link:
                return outputfile;
        }
        if (settings.type==LINK_TYPE_STATIC) {
                char* outputfile = string_clone(".god/lib/lib%s_%i.a",p.name,linkcounter);
                char* command=string_clone(
                        "mkdir -p .god/lib && "
                        "ar rcs %s %s",outputfile,args
                );
                printf("%s\n",command);
                system(command);
                return outputfile;
        }
        return NULL;
};