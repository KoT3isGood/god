#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "libgen.h"
#include "god/slang.h"
#include "god/utils.h"
#include "god/common.h"

char* slang_compile(struct slang_settings settings)
{
	if (settings.entry==NULL) {
		printf(
			"C_compile: "
			TERMINAL_RED 
			"error:"
			TERMINAL_RESET
			" entry wasn't specified\n"
			);
		return NULL;
	};
	printf(TERMINAL_MAGENTA"  SLANG    %s"TERMINAL_RESET"\n", settings.entry);
	char *output = string_clone(".god/slang/%i/%s.spv",compilecounter, settings.entry);
	char *outputDir = string_clone("%s",output);
	dirname(outputDir);
	makedir(outputDir);
	compilecounter++;
	struct run_project slangc = run_new("slangc");
	run_add_arg(&slangc, "-o");
	run_add_arg(&slangc, output);
	run_add_arg(&slangc, "-stage");
	switch (settings.stage)
	{
	case SLANG_STAGE_COMPUTE:
		run_add_arg(&slangc, "compute");
		break;
	case SLANG_STAGE_VERTEX:
		run_add_arg(&slangc, "vertex");
		break;
	case SLANG_STAGE_FRAGMENT:
		run_add_arg(&slangc, "fragment");
		break;
	case SLANG_STAGE_GEOMETRY:
		run_add_arg(&slangc, "geometry");
		break;
	case SLANG_STAGE_HULL:
		run_add_arg(&slangc, "hull");
		break;
	case SLANG_STAGE_DOMAIN:
		run_add_arg(&slangc, "domain");
		break;
	case SLANG_STAGE_MESH:
		run_add_arg(&slangc, "mesh");
		break;
	case SLANG_STAGE_RAYGEN:
		run_add_arg(&slangc, "raygen");
		break;
	case SLANG_STAGE_INTERSECTION:
		run_add_arg(&slangc, "intersection");
		break;
	case SLANG_STAGE_ANYHIT:
		run_add_arg(&slangc, "anyhit");
		break;
	case SLANG_STAGE_CLOSESTHIT:
		run_add_arg(&slangc, "closesthit");
		break;
	case SLANG_STAGE_MISS:
		run_add_arg(&slangc, "miss");
		break;
	case SLANG_STAGE_CALLABLE:
		run_add_arg(&slangc, "callable");
		break;
	case SLANG_STAGE_AMPLIFICATION:
		run_add_arg(&slangc, "amplification");
		break;
	};
	run_add_arg(&slangc, settings.entry);
	run_run(&slangc);
	return output;
};
