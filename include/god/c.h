
#ifndef _GOD_C_H
#define _GOD_C_H
#include "build.h"
#include "stdint.h"

/* Compile flags define how the object should be generated. */
#define C_COMPILE_FLAGS_NOSTDINC 0x1
#define C_COMPILE_FLAGS_FREESTANDING 0x2

struct C_settings {
        char** include_dirs;
        char** include_files;
	uint32_t compile_flags;
	uint32_t generation_flags;
};

/*
 * C_compile() - produces new project that can be used by linker. Based on 
 * compiler it can build C, C++, Obj-C, Obj-C++, and whatever compiler can 
 * compile. 
 * All files in a new project are object files.
 */
struct project C_compile(struct project p, struct C_settings settings);


/*
 * C_generate_lsp() - generates language servers configuration files for all 
 * files mentioned in C_compile();
 * 
 * It does not care about file timestamps as it updates whenever build system 
 * file gets updated. 
 */
void C_generate_lsp();

#endif
