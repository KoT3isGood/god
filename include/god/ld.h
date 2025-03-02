#ifndef _GOD_LD_H
#define _GOD_LD_H
#include "build.h"

enum link_type {
        LINK_TYPE_STATIC,
        LINK_TYPE_DYNAMIC,
        LINK_TYPE_EXECUTABLE,
};

struct link_settings {
        enum link_type type;
        char** lib_dirs;
        char** libs;
};

char* ld_link_project(struct project p, struct link_settings settings);

#endif
