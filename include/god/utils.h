#ifndef _GOD_UTILS_H
#define _GOD_UTILS_H


extern int linkcounter;
extern int compilecounter;



/* Copies file from source to destination */
void mv(char* dest, char* source);

/* We have to use _ to fix compiler errors with libc */
/* Actually moves file */
void _mv2(char* dest, char* source);
void _mkdir(char* folder);
void _rm(char* file);
void _rmdir(char* folder); 

#endif