#include "..\header\FileManagementPrototypes.h"
#include <math.h>
#include <stdio.h>
#pragma warning(disable : 4996) 

bool FM_exists (const char *fileName)
{
/* checks if a file exists */
	FILE *stream;
	if ((stream = fopen (fileName, "rb")) == NULL)
        return false;
	else {
        fclose (stream);
	    return true;
    }
}

unsigned long int FM_size (FILE *stream, unsigned int *size)
/* Returns the number of *size blocks that can fit into file *
 * The file must be open with a read mode for this to work.  *
 * The number of blocks are rounded up.			     		 *
 * Returns a negative value in case of an error.	     	 */
{
	fseek (stream, 0, SEEK_END);
	return size == NULL ? ceil (ftell (stream)/BLOCKSIZE) : ceil (ftell (stream)/((*size)*BLOCKSIZE));
}

bool FM_destroy (const char *fileName)
{
/* destroys a file with a given fileName */
    if (remove (fileName) == 0)
             return true;              
    else 
             return false;
}

bool FM_create (const char *fileName)
{
/* creates a file with a given fileName */
    FILE *stream;
    if ((stream = fopen (fileName, "wb")) == NULL)
		return false;
	else {
	    fclose (stream);
     	return true;
	}
}

bool FM_open (const char *fileName, const char *mode, FILE **stream)
{
/* opens a file with a given fileName */
	 if (((*stream) = fopen (fileName, mode)) == NULL)
	 		return false;
	 else 
            return true;
}

bool FM_close (FILE *stream)
{
/* closes the file pointed by stream */
	if (fclose (stream) == 0)
	 		return true;
    else 
            return false;
}
