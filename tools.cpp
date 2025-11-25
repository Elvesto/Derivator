#include "tools.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

uint64_t SizeFile(FILE* file) {
    assert(file);

    struct stat info;
    if (fstat(fileno(file), &info) != 0) {
        fprintf(stderr, "Что-то пошло не так\n");
        return 0;
    }

    return (uint64_t)info.st_size;
}

char* BufCreate(FileParam* fileInfo) {
    assert(fileInfo);

    uint64_t fsize = SizeFile(fileInfo->file);

    char* buffer = (char*)calloc(fsize + 1, sizeof(char));
    if (buffer == NULL) {
        printf("ALLOCATION BUFFER ERROR\n");
        return NULL;
    }
    // fprintf(stderr, "%s\n", buffer);

    fread(buffer, sizeof(char), fsize, fileInfo->file);

    buffer[fsize] = '\0';

    return buffer;
}


int CreateFileParam(const int argc, const char* const argv[], FileParam* fileLoadTree) {
    assert(argv);
    assert(fileLoadTree);

    if (argc == 2) {
        fileLoadTree->fileName = argv[1];
        fileLoadTree->file = fopen(argv[1], "r+");
    } 

    if (fileLoadTree->file == NULL) {
    // FIXME - fclose 
        fprintf(stderr, "FILE NO OPEN - %s\n", argv[1]);
        
        fileLoadTree->fileName = NULL;

        return -1;
    }

    return 0;
}

int CheckInArray(const char* value, const char* arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (strcmp(value, arr[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void ClearStream() {
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF);
}