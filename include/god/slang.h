
#ifndef _GOD_SLANG_H
#define _GOD_SLANG_H



struct slang_settings {
        char** include_dirs;
	
};

char* slang_compile(struct project );

#endif