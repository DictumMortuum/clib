#include "..\header\DataManagementPrototypes.h"
#include <stdio.h>

/* reads the stream's v block and copies it in buffer */

bool DM_readBlock (FILE *stream, const int v, char *buffer)
{
      fseek (stream, v * BLOCKSIZE, SEEK_SET);
      if (fread (buffer, 1, sizeof(char) * BLOCKSIZE, stream) != (sizeof(char) * BLOCKSIZE))
             return false;
      else 
             return true;
}

/*	writes in the stream's number v block the data from the buffer */

bool DM_writeBlock (FILE *stream, const int v, const char *buffer)
{
      fseek (stream, v * BLOCKSIZE, SEEK_SET);
      if (fwrite (buffer, sizeof(char) * BLOCKSIZE, 1, stream) == 1)
            return true;
      else  
            return false;
}
