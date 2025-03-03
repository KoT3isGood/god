#ifndef _GOD_BUILD_H
#define _GOD_BUILD_H




enum build_optimize {
        BUILD_OPTIMIZE_DEBUG,
        BUILD_OPTIMIZE_RELEASE_SAFE,
        BUILD_OPTIMIZE_RELEASE_FAST,
        BUILD_OPTIMIZE_RELEASE_SIZE,
};

/*
 *
 */
struct build_data {
        char* arch;
        char* os;
        enum build_optimize optimize;
};


/*
 * Project metadata.
 * All it does is it prepares files to be built by compiler.
 * Compiler variable points on compiler to be used during the build. Can be NULL.
 * All installed files should be temporary if compile function doesn't provide
 * cache type.
 */
struct project {
        struct build_data* b;
        char* name;
        char* compiler;
        /* Last file should be NULL */
        char** files;
};

/*
 * mv() - moves file or directory from source to destination.
 * dest and source should be valid strings
 */
void mv(char* dest, char* source);


#endif