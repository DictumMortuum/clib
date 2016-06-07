#include "block.h"
#include <stdlib.h>

static bool block_get (void *buffer, size_t size, size_t count, FILE *stream);
static bool block_geti (void *buffer, size_t size, size_t count, FILE *stream);
static bool block_set (const void *buffer, size_t size, size_t count, FILE *stream);
static bool block_seti (const void *buffer, size_t size, size_t count, FILE *stream);

clib_block *clib_block_init (void)
{
  clib_block *temp = (clib_block *) calloc (1, sizeof(clib_block));
  temp->get = &block_get;
  temp->geti = &block_geti;
  temp->set = &block_set;
  temp->seti = &block_seti;
  return temp;
}

void clib_block_free (clib_block *block)
{
  free (block);
}

static bool block_get (void *buffer, size_t size, size_t count, FILE *stream)
/* Reads from @stream the @count'th object of @size and copies it into @buffer */
/* The file pointer will be modified after this operation */
{
  if(fseek (stream, (long)(count * size), SEEK_SET) != 0) {
    return false;
  }

  size_t retval = fread (buffer, size, 1, stream);

  if(retval != count) {
    if (feof (stream)) {
      return true;
    }
    if (ferror (stream)) {
      return false;
    }
  }

  return true;
}

static bool block_geti (void *buffer, size_t size, size_t count, FILE *stream)
/* Reads from @stream the @count'th object of @size and copies it into @buffer */
/* The file pointer is reset after this operation to its original position */
{
  long int pos = ftell (stream);

  if (pos == -1L) {
    return false;
  }

  if (fseek (stream, (long)(count * size), SEEK_SET) != 0) {
    return false;
  }

  size_t retval = fread (buffer, size, 1, stream);

  if (retval != count) {
    if (ferror (stream)) {
      return false;
    }
  }

  if (fseek (stream, pos, SEEK_SET) != 0) {
    return false;
  }

  return true;
}

static bool block_set (const void *buffer, size_t size, size_t count, FILE *stream)
/* Writes into @stream the @buffer as its @count'th object of @size */
/* The file pointer will be modified after this operation */
{
  if(fseek (stream, (long)(count * size), SEEK_SET) != 0) {
    return false;
  }

  size_t retval = fwrite (buffer, size, 1, stream);

  if(retval != count) {
    if (feof (stream)) {
      return true;
    }
    if (ferror (stream)) {
      return false;
    }
  }

  return true;
}

static bool block_seti (const void *buffer, size_t size, size_t count, FILE *stream)
/* Writes into @stream the @buffer as its @count'th object of @size */
/* The file pointer is reset after this operation to its original position */
{
  long int pos = ftell (stream);

  if (pos == -1L) {
    return false;
  }

  if (fseek (stream, (long)(count * size), SEEK_SET) != 0) {
    return false;
  }

  size_t retval = fwrite (buffer, size, 1, stream);

  if (retval != count) {
    if (ferror (stream)) {
      return false;
    }
  }

  if (fseek (stream, pos, SEEK_SET) != 0) {
    return false;
  }

  return true;
}
