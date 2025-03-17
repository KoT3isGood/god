
#ifndef _GOD_C_H
#define _GOD_C_H
#include "build.h"
#include "stdint.h"
#include "stdio.h"

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

/* holy shit triple star */
extern FILE* cdb;

#endif
