#ifndef _GOD_BUILD_H
#define _GOD_BUILD_H




enum build_optimize {
        BUILD_OPTIMIZE_DEBUG,
        BUILD_OPTIMIZE_RELEASE_SAFE,
        BUILD_OPTIMIZE_RELEASE_FAST,
        BUILD_OPTIMIZE_RELEASE_SIZE,
};

enum build_kernel {
	BUILD_KERNEL_NONE,
	BUILD_KERNEL_LINUX,
	BUILD_KERNEL_WINDOWS,
	BUILD_KERNEL_MACOS,
};

enum build_arch {
	BUILD_ARCH_x86,
	BUILD_ARCH_x86_64,
};

/*
 * Build data for projects.
 */
struct build_data {
	enum build_kernel kernel; /* Operating system compiled for*/
	enum build_arch arch; /**/
        enum build_optimize optimize;
};


/*
 * Project metadata.
 * All it does is it prepares files to be built by compiler.
 * Compiler variable points on compiler to be used during the build.
 * All files are stored in .god directory until they are moved out.
 */
struct project {
        struct build_data* b;
        char* name;
        char* compiler;
	/* 
	 * Compiled files
	 * Last file should be null 
	 */
        char** files; 
};



#endif