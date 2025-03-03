#include "god/build.h"
#include "getopt.h"
#include "stddef.h"
#include "string.h"
#include <bits/getopt_core.h>
#include "stdio.h"
int main(int argc, char **argv) {
        int build(struct build_data);
        char *string_clone(const char *str,...);

        struct build_data b = {};
        b.arch = "x86_64";
        b.os = "linux";
        /* Parse argumets. */
        struct option options[]={
                {"target",required_argument,0,0},
                {"help",no_argument,0,'h'},
                {0,0,0,0}
        };    
        int opt;
        while ((opt = getopt_long(argc, argv, "", options, NULL)) != -1) {
                if (opt == 0) {
                        if (strcmp("target", optarg) == 0) {
                                b.os=string_clone(optarg);
                        };
                        if (strcmp("arch", optarg) == 0) {
                                b.arch=string_clone(optarg);
                        };
                }
        }

        return build(b);
}