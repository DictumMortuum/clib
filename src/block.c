#include "block.h"
#include "stream.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int block_allocate (char **bufferPtr, const unsigned long bufferSize, const void *objPtr, const unsigned int objSize)
/* creates an empty block and initializes it */
/* if the second argument is a valid pointer, it copies it into the new block */
{
  if ((*bufferPtr = (char *) malloc (sizeof (char) * bufferSize)) == NULL) {
    return 0;
  }

  memset (*bufferPtr, 0, sizeof (char) * bufferSize);

  if (objPtr != NULL) {
    memcpy (*bufferPtr, objPtr, objSize);
  }

  return 1;
}

int block_read (FILE *stream, char *buffer, const unsigned int n, const unsigned int size)
/* Read the n'th block of stream into buffer */
{
  fseek (stream, n * size, SEEK_SET);
  if (fread (buffer, 1, sizeof(char) * size, stream) != (sizeof(char) * size))
    return 0;
  else
    return 1;
}

int block_write (FILE *stream, const char *buffer, const unsigned int n, const unsigned int size)
/* Write buffer as the n'th block of stream */
{
  fseek (stream, n * size, SEEK_SET);
  if (fwrite (buffer, sizeof(char) * size, 1, stream) == 1)
    return 0;
  else
    return 1;
}
