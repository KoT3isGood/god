#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "libgen.h"
#include "../../include/god/c.h"
#include "../../include/god/utils.h"
#include "../common.h"
#include "sys/wait.h"


#define TERMINAL_RED "\x1B[31m"
#define TERMINAL_GREEN "\x1B[32m"
#define TERMINAL_YELLOW "\x1B[33m"
#define TERMINAL_BLUE "\x1B[34m"
#define TERMINAL_MAGENTA "\x1B[35m"
#define TERMINAL_CYAN "\x1B[36m"
#define TERMINAL_WHITE "\x1B[37m"
#define TERMINAL_RESET "\x1B[0m"



struct project C_compile(struct project p, struct C_settings settings) {

        char* compiler = p.compiler;
        char* compilecommand = NULL;
        char* includeargs = NULL;
        struct project build = {};
        int i = 0;
        int b = 0;
        int numfiles=0;
        int argsize=0;


        if (p.b==NULL) {
                printf(
                        "C_compile: "
                        TERMINAL_RED 
                        "error:"
                        TERMINAL_RESET
                        " build system wasn't specified\n"
                        );
                return (struct project){};
        };
        if (p.files==NULL) {

                printf(
                        "C_compile: "
                        TERMINAL_RED 
                        "error:"
                        TERMINAL_RESET
                        " files weren't specified\n"
                        );
                return (struct project){};
        };

        /* Includes*/
        i=0;
        if (settings.include_dirs==NULL) {
                includeargs="";
                goto skip_include;
        }

        do {
                argsize+=strlen(settings.include_dirs[i])+3;
                i++;
        } while (settings.include_dirs[i]);
        argsize+=1;
        includeargs = malloc(argsize);
        memset(includeargs,0,argsize);

        i=0;
        do {
               strcat(includeargs," -I"); 
               strcat(includeargs,settings.include_dirs[i]); 
               i++;
        } while (settings.include_dirs[i]);
        argsize=0;

skip_include:

        i=0;
        if (compiler==NULL) {
                compiler="gcc";
        };
        do {
                i++;
        } while (p.files[i]);
        numfiles=i;
        b=i+1;
        i=0;
        build.files=malloc(sizeof(char*)*b);
        memset(build.files,0,sizeof(char*)*b);

        printf(TERMINAL_CYAN"Building %s\n"TERMINAL_RESET,p.name);
        do {
                compilecounter++;
                char* outputfile = string_clone(".god/o/%s.o",p.files[i],compilecounter);

                /* dirname() modifies string instead of allocating new one. */
                char* outputfile2 = string_clone(outputfile);
                char* outputdir = dirname(outputfile2);

                /* Prevent useful execution time */
                build.files[i]=outputfile;
                time_t source_time=get_modification_time(p.files[i]);
                time_t script_time=get_modification_time(outputfile);


                /* TODO: replace with execve */
                char* command = string_clone("gcc -Wno-incompatible-pointer-types -g -fPIE -c %s%s -o %s", p.files[i], includeargs, outputfile);
                if (!(i%2)) 
                        printf(TERMINAL_YELLOW"  [%i/%i] %s"TERMINAL_RESET,i+1,numfiles,command);
                else
                        printf(TERMINAL_WHITE"  [%i/%i] %s"TERMINAL_RESET,i+1,numfiles,command);

                int needs = needs_rebuild(script_time, source_time);
                if (needs)
                        printf("\n");
                else
                        printf(TERMINAL_BLUE" (cached)"TERMINAL_RESET"\n");

                if (!needs) goto skip_build;

                /* linux won't show colors */
                char* term = getenv("TERM");
                char* cmd = string_clone("export TERM=%s && mkdir -p %s/ && %s",term,outputdir,command);
                system(cmd);
        skip_build:
                i++;
        } while (p.files[i]);

        build.b=p.b;
        build.name=p.name;


        return build;
}