#ifndef _GOD_UTILS_H
#define _GOD_UTILS_H


extern int linkcounter;
extern int compilecounter;
extern char trace;

#define TERMINAL_RED "\x1B[31m"
#define TERMINAL_GREEN "\x1B[32m"
#define TERMINAL_YELLOW "\x1B[33m"
#define TERMINAL_BLUE "\x1B[34m"
#define TERMINAL_MAGENTA "\x1B[35m"
#define TERMINAL_CYAN "\x1B[36m"
#define TERMINAL_WHITE "\x1B[37m"
#define TERMINAL_RESET "\x1B[0m"



/* Copies file from source to destination */
void mv(char* dest, char* source);

void mv2(char* dest, char* source);
void makedir(char* folder);
void rem(char* file);
void remdir(char* folder);

struct run_project {
	char* wd;
	char* executable;
	char** args;
};
void run_add_arg(struct run_project* project, char* arg);
void run_run(struct run_project* project);
struct run_project run_new(char* name);

void add_item(char** list, char* item);

#endif