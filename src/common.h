#ifndef _SRC_COMMON_H
#define _SRC_COMMON_H
#include "time.h"

/* Allocates new string and formats it. */
char *string_clone(const char *str, ...);

/* Returns file create time */
time_t get_modification_time(const char *filename);

/* Returns if file needs to be rebuilt. */
int needs_rebuild(time_t, time_t);

#endif