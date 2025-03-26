
#ifndef _GOD_SLANG_H
#define _GOD_SLANG_H

enum slang_input {
	SLANG_INPUT_SLANG = 0,
	SLANG_INPUT_GLSL,
	SLANG_INPUT_HLSL,
	SLANG_INPUT_C,
	SLANG_INPUT_CPP,
	SLANG_INPUT_CUDA,
};

enum slang_output {
	SLANG_OUTPUT_SPIRV,
	SLANG_OUTPUT_CPP,
	SLANG_OUTPUT_C,
};

struct slang_settings {
	enum slang_input input_type
	enum slang_output output_type;
	char* entry;
        char** include_dirs;	
};

char* slang_compile(struct project p, struct slang_settings settings);

#endif