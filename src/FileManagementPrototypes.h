#ifndef FM_PROTOTYPES__
#define FM_PROTOTYPES__

#include ".\SortDefine.h"
#include <stdio.h>

bool FM_create  (const char *);							/* creates a file with a given filename */
bool FM_destroy (const char *);							/* destroys a file with a given filename*/
bool FM_open    (const char *, const char *, FILE **);	/* opens a file with a given filename */
bool FM_close   (FILE *);								/* closes a file with a given filename */
bool FM_exists  (const char *);							/* checks if a file exists */
unsigned long int FM_size (FILE *, unsigned int *);

#endif
