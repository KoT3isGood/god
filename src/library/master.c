#include "god/utils.h"
#include "god/common.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include <sys/wait.h>

int linkcounter = 0;
int compilecounter = 0;
char trace = 0;
void mv(char* dest, char* source) {
	#ifdef __WIN64__

	char* command = string_clone("xcopy /E /I %s %s",source,dest);
	#endif
	#ifdef __linux__
	char* command = string_clone("cp -r %s %s",source,dest);
	#endif
	system(command);
};
void makedir(char* folder) {
	#ifdef __linux__
	char* command = string_clone("mkdir -p %s",folder);
	#endif
	system(command);
};

struct run_project run_new(char* name) {
	struct run_project p = {};
	p.executable=string_clone("%s",name);
	p.args=malloc(sizeof(char*)*2);
	p.args[0]=p.executable;
	p.args[1]=NULL;
	return p;
}
void run_add_arg(struct run_project* project, char* arg) {
	int i = 0;
	do {
		i++;
	} while(project->args[i]);
	project->args=realloc(project->args,sizeof(char*)*(i+2));
	project->args[i]=string_clone("%s",arg);
	project->args[i+1]=NULL;
};

void run_run(struct run_project* project) {

	if (trace) {
		int i = 0;
		do {
			printf("%s ",project->args[i]);
			i++;
		} while(project->args[i]);
		printf("\n");
	}

	pid_t pid = fork();
	if (pid<0) {
		printf("\nfailure\n");
		exit(0);
	}
	if (pid == 0) {
		if (project->wd) {
			char wd[1024];
			getcwd(wd,1024);
			char* w = string_clone("%s/%s",wd,project->wd);
			int a=chdir(w);
		}
		int a = execvp(project->executable, project->args);	
		printf("\n failed to open %s\n",project->executable);
		exit(0);
	};
	int status;
	wait(NULL);
	if (WIFEXITED(status)) {
		if (status>127) {
			printf("\nfailure %d\n",status);
			exit(status);
		}
	}

};

void add_item(char** list, char* item) {
	int i = 0;
	do {
		i++;
	} while(list[i]);
	list=realloc(list,sizeof(char*)*(i+2));
	list[i]=item;
	list[i+1]=NULL;
};
