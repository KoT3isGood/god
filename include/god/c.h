
#ifndef _GOD_C_H
#define _GOD_C_H
#include "build.h"
#include "stdint.h"
#include "stdio.h"

#define C_COMPILE_FLAGS_WALL 1
#define C_COMPILE_FLAGS_WERROR 2

#define C_GENERATION_FLAGS_NOSTDINC 1
#define C_GENERATION_FLAGS_FREESTANDING 2

struct C_settings {
        char** include_dirs;
        char** include_files;
	struct C_Macro {
		char* name;
		char* value;
	}* macros;
	uint32_t compile_flags;
	uint32_t generation_flags;
	enum C_version {
		C_VERSION_AUTO = 0,
		C_VERSION_C89,
		C_VERSION_C99,
		C_VERSION_C11,
		C_VERSION_C17,
		C_VERSION_C23,
		C_VERSION_C2Y,
	} version;
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
