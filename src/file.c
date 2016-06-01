#include "file.h"
#include <math.h>
#include <stdio.h>

int file_exists (const char *fileName)
/* checks if a file exists */
{
	FILE *stream;
	if ((stream = fopen (fileName, "rb")) == NULL)
		return 0;
	else {
		fclose (stream);
		return 1;
	}
}

long int file_blocks (FILE *stream, unsigned int size)
/*
	Returns the number of size blocks that can fit into the file.
  The file must be open with a read mode for this to work.
  The number of blocks are rounded down.
  Returns a negative value in case of an error.
*/
{
	long int offset, original_offset = ftell (stream);
	int retval = 0;

	/* move file pointer to the end of the file, get its size in bytes and then move it back to where it was */
	retval |= fseek (stream, 0, SEEK_END);
	offset = ftell (stream);
	retval |= (offset == 0);
	retval |= fseek (stream, original_offset, SEEK_SET);

	if (size > 0 && retval == 0) {
		return floor (ftell (stream) / size);
	} else {
		return -1;
	}
}
