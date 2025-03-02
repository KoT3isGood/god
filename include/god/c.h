#ifndef _GOD_C_H
#define _GOD_C_H
#include "build.h"
#include "stdint.h"

#define C_COMPILE_FLAGS_ANSI 0x1
#define C_COMPILE_FLAGS_C99 0x2
#define C_COMPILE_FLAGS_C11 0x4
#define C_COMPILE_FLAGS_C17 0x8
#define C_COMPILE_FLAGS_C23 0x10
#define C_COMPILE_FLAGS_LIBC 0x11

struct C_settings {
        char** include_dirs;
        uint32_t flags;
};

/*
 * C_compile produces new project that can be used by linker. Based on compiler it can build 
 * C, C++, Obj-C, Obj-C++, and whatever compiler can compile. 
 * All files are object files.
 */
struct project C_compile(struct project p, struct C_settings settings);

#endif
