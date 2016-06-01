#ifndef CLIB_FILE
#define CLIB_FILE

#include <stdio.h>

long int file_blocks (FILE *stream, unsigned int size);
int file_exists (const char *fileName);

#endif
