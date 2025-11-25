#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <malloc.h>

#define BLUE_BOLD_COLOR     "\33[1;34m"
#define RED_BOLD_COLOR      "\33[1;31m"
#define COLOR_END           "\33[0m"

#ifdef PRINT_DEBUG
    #define DEBUG_LOG(format, ...)  do { \
        fprintf(stderr, BLUE_BOLD_COLOR "[DEBUG] %s:%d:%s(): " format "\n" COLOR_END, \
            __FILE__, __LINE__, __func__, __VA_ARGS__); \
    } while(0)
#else
    #define DEBUG_LOG(format, ...) 
#endif // PRINT_DEBUG

#define FREE(a) do { \
    free(a); \
    a = NULL; \
} while(0) 

#define LIMIT_STR "%31"

struct FileParam {
    FILE* file;
    const char* fileName;
};

uint64_t SizeFile(FILE* file);

char* BufCreate(FileParam* fileInfo);

int CreateFileParam(const int argc, const char* const argv[], 
    FileParam* fileLoadTree);

int CheckInArray(const char* value, const char* arr[], int size);

void ClearStream();

#endif // TOOLS_H_