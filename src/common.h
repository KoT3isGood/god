
#include "time.h"

char *string_clone(const char *str, ...);

time_t get_modification_time(const char *filename);
int needs_rebuild(time_t, time_t);