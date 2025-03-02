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
#define TERMIMAL_CYAN "\x1B[36m"
#define TERMIMAL_WHITE "\x1B[37m"
#define TERMINAL_RESET "\x1B[0m"


char* ld_link_project(struct project p, struct link_settings settings) {

        linkcounter++;
        int i = 0;
        int b = 0;
        int argsize = 0;
        char* args=NULL;

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

        if (settings.type==LINK_TYPE_EXECUTABLE) {
                char* outputfile = string_clone(".god/bin/%s_%i",p.name,linkcounter);
                char* command=string_clone(
                        "mkdir -p .god/bin && "
                        "ld "
                        "$(gcc -print-file-name=crt1.o) "
                        "$(gcc -print-file-name=crti.o) "
                        "%s"
                        "$(gcc -print-file-name=crtn.o) "
                        " -dynamic-linker /lib64/ld-linux-x86-64.so.2 -lc -m elf_x86_64 "
                        "-o %s\n",args,outputfile);
                system(command);
                return outputfile;
        }
        if (settings.type==LINK_TYPE_STATIC) {
                char* outputfile = string_clone(".god/lib/lib%s_%i.a",p.name,linkcounter);
                char* command=string_clone(
                        "mkdir -p .god/lib &&"
                        "ar rcs %s %s",outputfile,args
                );
                system(command);
                return outputfile;
        }
        return NULL;
};