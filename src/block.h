#ifndef CLIB_BLOCK
#define CLIB_BLOCK

#include <stdio.h>

int block_read (FILE *stream, char *buffer, const unsigned int n, const unsigned int size);
int block_write (FILE *stream, const char *buffer, const unsigned int n, const unsigned int size);
int block_allocate (char **bufferPtr, const unsigned int bufferSize, const void *objPtr, const unsigned int objSize)

#endif
