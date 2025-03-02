#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "common.h"
#include "sys/stat.h"

char *string_clone(const char *str,...) {
        va_list args;
        va_list args_copy;
        int lenght;
        char* new;
        if (!str) return 0;
        va_start(args, str);
        va_copy(args_copy, args);
        lenght = vsnprintf(0,0,str,args_copy);
        new = malloc(lenght+1);
        vsnprintf(new, lenght+1, str, args);
        va_end(args);
        va_end(args_copy);
        return new;
};

time_t get_modification_time(const char *filename) {
        struct stat stat_buf;
        if (stat(filename, &stat_buf) != 0) {
                return (time_t)-1;
        }
        return stat_buf.st_mtime;
};
int needs_rebuild(time_t target, time_t dependency) {

    if (target == (time_t)-1 || dependency == (time_t)-1) {
        return 1; 
    }

    return dependency > target;
};