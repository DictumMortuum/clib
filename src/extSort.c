#include "..\header\BlockManagementPrototypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long int extSort_entryNm ()
/* returns the number of entries a block is able to hold */
{
	return (int) floor ((double)BLOCKSIZE/(SortInfo.attrLength1 + SortInfo.attrLength2));
}

unsigned int extSort_rem ()
/* returns the remaining size in the block, if any */
{
	return BLOCKSIZE - extSort_entryNm ();
}

bool extSort_ph1 (const char *fileName, const int bufferSize)
{
	char *buffer, *bufferPtr, *mode = "wb+";
	FILE *tempStream;
	register unsigned long int runs;			/* runs is size of the file in blocks, rounded up */
	register unsigned int i, j;
	char **sortPtr;
	
	if (!FM_exists (fileName))							
		return false;							/* checks if fileName exists */
		
	if (!FM_open (fileName, mode, &tempStream))			
		return false;							/* open the file to be sorted */

	if (!BM_AllocBlock (&buffer, bufferSize * BLOCKSIZE, NULL, 0))
		return false;							/* allocate buffer memory */

	runs = FM_Size (tempStream, bufferSize);
	
	/* for run times, bring bufferSize blocks on the memory and sort them */
	/* then write them back in the file in their correct position */
	
	for (i = 0; i < runs; i++) {				/* fill the buffer with blocks from the file */
		for (j = 0, bufferPtr = buffer; j < bufferSize; j++, bufferPtr += BLOCKSIZE) {
			if (!DM_readBlock (tempStream, i * bufferSize + j, bufferPtr))
				break;
		}
		
		if (!extSort_ptrTable (sortPtr, buffer, bufferSize))
			return false;
		
		extSort_bubbleSort (sortPtr, bufferSize * extSort_entriesNm (), NULL);
		
		for (bufferPtr -= BLOCKSIZE; --j > -1; bufferPtr -= BLOCKSIZE) {
			if (!DM_writeBlock (tempStream, i * bufferSize + j, bufferPtr))
				return false;
	}
	
	//ypologise tis epanalipseis pou xreiazontai
	
	//for i = 0 mexri epanalipsi tade 
	//fere sti mnimi to prwto block apo ta prwta n - 1 runs
	//
	
	free (buffer);
	
	if (!FM_close (tempStream))
		return false;
		
	return true;
	} 
}

bool extSort_ptrTable (char **sortPtr, const char *buffer, const int bufferSize)
/* creates a pointer table to the buffer entries */
{
	register unsigned long int j, entries = extSort_entriesNm ();
	register unsigned int i, unused = extSort_rem ();
	char *tempPtr = buffer;
	
	if ((sortPtr = malloc (sizeof (char *) * entries * bufferSize)) == NULL)
		return false;
	
	for (i = 0; i < bufferSize; ++i, tempPtr += unused) 
		for (j = 0; j < entries; ++j, tempPtr += SortInfo.attrLength1 + SortInfo.attrLength2)
			sortPtr[j] = tempPtr;
	return true;
}

void extSort_bubbleSort (char **sortPtr, const unsigned int length, int (*compare)(const void *, const void*))
{
	bool flag; 
	register unsigned int i;
	//length is length of the sortPtr array 
	
	do {
		flag = false;
		for (i = 0; i < length; i++)
			if (compare (sortPtr[i], sortPtr[i+1]) < 0) {
				extSort_swap (&sortPtr[i], &sortPtr[i+1]);
				flag = true;
			}
	} while (flag);
}

void extSort_swap (char **v1, char **v2)
{
	char *temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}