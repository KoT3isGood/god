
#ifndef _GOD_SLANG_H
#define _GOD_SLANG_H
#include "god/build.h"

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

enum slang_stage {
	SLANG_STAGE_COMPUTE,

	SLANG_STAGE_VERTEX,
	SLANG_STAGE_FRAGMENT,
	SLANG_STAGE_GEOMETRY,

	SLANG_STAGE_HULL,
	SLANG_STAGE_DOMAIN,
	SLANG_STAGE_MESH,


	SLANG_STAGE_RAYGEN,
	SLANG_STAGE_INTERSECTION,
	SLANG_STAGE_ANYHIT,
	SLANG_STAGE_CLOSESTHIT,
	SLANG_STAGE_MISS,
	
	SLANG_STAGE_CALLABLE,
	SLANG_STAGE_AMPLIFICATION,
};

struct slang_settings {
	enum slang_stage stage;
	enum slang_output output_type;
	char* entry;
        char** include_dirs;	
};

char* slang_compile(struct slang_settings settings);

#endif
