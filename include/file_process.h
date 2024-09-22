#ifndef __FILE_PROCESS_H__
#define __FILE_PROCESS_H__

#include <stdio.h>
#include <stdbool.h>

int get_file_size(FILE *f);
char *read_file(FILE *f);

#endif
