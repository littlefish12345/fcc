#include <stdio.h>
#include <stdlib.h>

#include <file_process.h>

int get_file_size(FILE *f) {
    int current_offset = ftell(f);
    fseek(f, 0, SEEK_END);
    int file_size = ftell(f);
    fseek(f, current_offset, SEEK_SET);
    return file_size;
}

char *read_file(FILE *f) {
    int file_size = get_file_size(f);
    if (file_size == -1) {
        return NULL;
    }
    char *buffer = (char *)malloc(file_size);
    fread(buffer, file_size, 1, f);
    return buffer;
}

bool file_exists(const char *path) {
    FILE *f = fopen(path, "r");
    bool exists = f != NULL;
    if (exists) {
        fclose(f);
    }
    return exists;
}
