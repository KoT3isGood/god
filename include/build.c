#include "god/build.h"
#include "getopt.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"



int main(int argc, char **argv) {
        int build(struct build_data);
        char *string_clone(const char *str,...);

        struct build_data b = {};
        b.arch = BUILD_ARCH_x86_64;
        b.kernel = BUILD_KERNEL_LINUX;
        /* Parse argumets. */
        struct option options[]={
                {"arch",required_argument,0,0},
                {"kernel",required_argument,0,0},
                {"help",no_argument,0,'h'},
                {0,0,0,0}
        };    
        int opt;
        int option_index;
        while ((opt = getopt_long(argc, argv, "h", options, &option_index)) != -1) {
                if (opt == 0) {
			const char* option = options[option_index].name;
                        if (strcmp("arch", option) == 0) {
				int a = 0;
                        	if (strcmp("x86_64", optarg) == 0) {
					b.arch=BUILD_ARCH_x86_64;
					continue;
				}
                        	if (strcmp("x86", optarg) == 0) {
					b.arch=BUILD_ARCH_x86;
					continue;;
				}
				printf("unrecognized arch: %s\n",optarg);
				continue;
                        };
                        if (strcmp("kernel", option) == 0) {
				if (strcmp("none", optarg) == 0) {
					b.kernel=BUILD_KERNEL_NONE;
					continue;
				}
                        	if (strcmp("linux", optarg) == 0) {
					b.kernel=BUILD_KERNEL_LINUX;
					continue;
				}
                        	if (strcmp("windows", optarg) == 0) {
					b.kernel=BUILD_KERNEL_WINDOWS;
					continue;
				}
				printf("unrecognized kernel: %s\n",optarg);
				continue;
                        };
                        if (strcmp("help", option) == 0) {
                        };
                }
        }

        return build(b);
}