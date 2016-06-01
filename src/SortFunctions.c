#include "..\header\BlockManagementPrototypes.h"
#include "..\header\FileManagementPrototypes.h"
#include "..\header\DataManagementPrototypes.h"
#include "..\header\SortPrototypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEBUG 1

int Sort_errno;

static void extSort_swap (char **, char **);
static void extSort_bubbleSort (char **sortPtr, const unsigned int length, int (*)(const void *, const void*));
static unsigned int extSort_rem ();
static unsigned long int extSort_entryNm ();
static bool extSort_ptrTable (char ***, char *, const int);
static bool extSort_RunSort (const char *, const int, const int, int (*)(const void *, const void *));
static int cmp (const void *, const void *); 
static void SortInfoInit (const char, const int, const char, const int); 
static unsigned int SortInfoUpdate ();

struct SortInfo {
	FILE *stream;
	char attrType1, attrType2;
	int attrLength1, attrLength2;
	unsigned int maxRecs, lastBlock, lastRec;
} SortInfo;


int main (void) {
	int j = 5, k, i;
	   int numOfRecs, errno;
   char val1[8];
   char val2[8];
   char val3[15];
   int val4;
   FILE *input, *output;

   //Create File input-data1 -> to be sorted
   if ((errno = Sort_Create_File("input-data1", 'c', 8, 'c', 15)) != SORT_OK)
	   Sort_PrintError("Problem in Sort_Create_File for file input-data1\n");
 

   if ((input = fopen("file_1.txt","r")) == NULL) {
	   fprintf(stderr,"You must have file_1.txt to run this program! \n");
	   exit(1);
   }

   //Insert records of file file_1.txt to file input-data1
   do {
      fscanf(input, "%s %s\n", val1, val3);

      if ((errno = Sort_InsertEntry("input-data1", val1, val3)) != SORT_OK)
	      Sort_PrintError("Problem in insertion of record in file input-data1\n");

   } while(!feof(input) );
     
   fclose(input);
	printf ("%d ", Sort_Print("input-data1"));
Sort_ExtSort("input-data1", 4);
	printf ("%d ", Sort_Print("input-data1"));

	return 0;
}

void Sort_Init (void)
{
	;
}

int Sort_Create_File (char *fileName, char attrType1, int attrLength1, char attrType2, int attrLength2)
{
	char *buffer, *mode = "wb";
	
	/* checks if fileName already exists */
	if (FM_exists (fileName))
		return Sort_errno = SORT_FILE_EXISTS;
		
	/* creates an empty file */
	if (!FM_create (fileName))
		return Sort_errno = SORT_FILE_CREATE;

	/* initialize our global structure with the user-defined values */
	SortInfoInit (attrType1, attrLength1, attrType2, attrLength2);
	
	/* open the empty file */
	if (!FM_open (fileName, mode, &SortInfo.stream))
		return Sort_errno = SORT_FILE_OPEN;

	/* alloc a new empty block */
	if (!BM_AllocBlock (&buffer, BLOCKSIZE, NULL, 0))
		return Sort_errno = SORT_BLOCK_ALLOC;
		
	/* write the block in the file */
	if (!DM_writeBlock (SortInfo.stream, 0, buffer))
		return Sort_errno = SORT_BLOCK_WRITE;
	
	/* empty the buffer */
	free (buffer);
	
	/* close the file */
	if (!FM_close (SortInfo.stream))
		return Sort_errno = SORT_FILE_CLOSE;
	
	/* success! */
	return SORT_OK;
}

int Sort_Destroy_File (char *fileName)
{
	/* just destroy the file */
	if (!FM_destroy (fileName))
		return Sort_errno = SORT_FILE_DESTROY;
	else 
		return SORT_OK;
}

int Sort_InsertEntry (char *fileName, void *value1, void *value2)
{
	char *buffer, *blockPtr, *mode = "r+b";
	register unsigned int pos;

	/* open the file */
	if (!FM_open (fileName, mode, &SortInfo.stream))
		return Sort_errno = SORT_FILE_OPEN;
		
	/* alloc a new empty block */
	if (!BM_AllocBlock (&buffer, BLOCKSIZE, NULL, 0))
		return Sort_errno = SORT_BLOCK_ALLOC;

	/* get the entry's position */
	if ((pos = SortInfoUpdate ()) == 0) 
		/* alloc a new block in the file if the previous is full */
		if (!DM_writeBlock (SortInfo.stream, SortInfo.lastBlock, buffer))
			return Sort_errno = SORT_BLOCK_WRITE;

	/* read the block from the file */
	if (!DM_readBlock (SortInfo.stream, SortInfo.lastBlock, buffer))
		return Sort_errno = SORT_BLOCK_WRITE;
		
	/* set blockPtr to hold the buffer's position */
	blockPtr = buffer;
	
	/* move blockPtr to the writing position */
	blockPtr += pos * (SortInfo.attrLength1 + SortInfo.attrLength2);
	
	/* insert the entry */
	memcpy (blockPtr, value1, SortInfo.attrLength1);
	blockPtr += SortInfo.attrLength1;
	memcpy (blockPtr, value2, SortInfo.attrLength2);
	
	/* write the updated block back into the file */
	if (!DM_writeBlock (SortInfo.stream, SortInfo.lastBlock, buffer))
		return Sort_errno = SORT_BLOCK_WRITE;	
		
	/* empty the buffer */
	free (buffer);

	/* close the file */
	if (!FM_close (SortInfo.stream))
		return Sort_errno = SORT_FILE_CLOSE;

	return SORT_OK;
}

int Sort_Print (char *fileName) 
{
	char *buffer, *blockPtr; 
	void *value1, *value2;
	register unsigned int i, j, counter = 0;

#if DEBUG
	printf ("max recs: %d\n", SortInfo.maxRecs);
#endif

	value1 = malloc (SortInfo.attrLength1);
	value2 = malloc (SortInfo.attrLength2);

	/* open the file */
	if (!FM_open (fileName, "rb", &SortInfo.stream))
		return Sort_errno = SORT_FILE_OPEN;

	/* alloc a new empty block */
	if (!BM_AllocBlock (&buffer, BLOCKSIZE, NULL, 0))
		return Sort_errno = SORT_BLOCK_ALLOC;

	for (i = 0; i <= SortInfo.lastBlock; i++) {
		/* read the block from the file */
		if (!DM_readBlock (SortInfo.stream, i, buffer))
			return Sort_errno = SORT_BLOCK_WRITE;

		/* set blockPtr to hold the buffer's position */
		blockPtr = buffer;

		for (j = 0; j < SortInfo.maxRecs; j++) { 
			memcpy (value1, blockPtr, SortInfo.attrLength1);
			blockPtr += SortInfo.attrLength1;
			memcpy (value2, blockPtr, SortInfo.attrLength2);
			blockPtr += SortInfo.attrLength2;
			counter++;
#if DEBUG
			printf ("rec No: %d ", j);
			printf("value1: %s value2: %s\n", (char *)value1, (char *)value2);
#endif
			if (i == SortInfo.lastBlock && j == SortInfo.lastRec)
				break;
		}
	}

	free(value1);
	free(value2);

	/* close the file */
	if (!FM_close (SortInfo.stream))
		return Sort_errno = SORT_FILE_CLOSE;

	return counter;
}

int Sort_ExtSort (char *fileName, int bufferSize)
{
	char *buffer, *bufferPtr, *mode = "r+b";
	register unsigned long int runs;		/* runs is size of the file in blocks, rounded up */
	register unsigned int i;
	register int j, end, p, n, q, k;
	char **sortPtr = NULL;
	

	if (!FM_exists (fileName))							
		return -1;							/* checks if fileName exists */
		
	if (!FM_open (fileName, mode, &SortInfo.stream))			
		return -1;							/* open the file to be sorted */

	if (!BM_AllocBlock (&buffer, bufferSize * BLOCKSIZE, NULL, 0))
		return -1;							/* allocate buffer memory */

	runs = FM_size (SortInfo.stream, &bufferSize);

///////////////////////////////////////////////////////////////////////////////////////////
										/* phase 1 */
///////////////////////////////////////////////////////////////////////////////////////////

	/* for run times, bring bufferSize blocks on the memory and sort them */
	/* then write them back in the file in the correct position */
	
	for (i = 0; i <= runs; i++) {			/* fill the buffer with blocks from the file */
		for (j = 0, bufferPtr = buffer; j < bufferSize; j++, bufferPtr += BLOCKSIZE)
			if (!DM_readBlock (SortInfo.stream, i * bufferSize + j, bufferPtr)) 
				break;

		if (!extSort_ptrTable (&sortPtr, buffer, j))
			return -1;						/* make a pointer table for the entries in the buffer */

		if (j == bufferSize)				/* sort the entries through the pointer table */
			extSort_bubbleSort (sortPtr, j * extSort_entryNm(), cmp);
		else 
			extSort_bubbleSort (sortPtr, SortInfo.lastRec + 1, cmp);

		free (sortPtr);						/* we don't need the table pointer any more */
		
		for (bufferPtr -= BLOCKSIZE; --j > -1; bufferPtr -= BLOCKSIZE) {
			if (!DM_writeBlock (SortInfo.stream, i * bufferSize + j, bufferPtr))
				return -1;					/* write the blocks back into the file in the right position */
		}
	}

	if (!FM_close (SortInfo.stream))
		return -1;

///////////////////////////////////////////////////////////////////////////////////////////
										/* phase 2 */
///////////////////////////////////////////////////////////////////////////////////////////

	//ypologise tis epanalipseis pou xreiazontai
	//for i = 0 mexri epanalipsi tade 
	//fere sti mnimi to prwto block apo ta prwta n - 1 runs

	i = 1;
	//p = log m toy k - 1
	p = 2;

	if (!FM_open (fileName, mode, &SortInfo.stream))			
		return -1;							/* open the file to be sorted */

	while (i <= p) {
		n = 1;
		end = (int) ceil ((runs * 1.0)/(bufferSize - 1));

		printf ("end: %d\n", end);

		while (n <= end) {
			extSort_RunSort (buffer, n, bufferSize, cmp);
			n++;
		}
		runs = end;
		
		i++;
	}

	if (!FM_close (SortInfo.stream))
		return -1;

	free (buffer);
		
	return SORT_OK;
}

void Sort_PrintError (char *errString)
{
}

typedef struct RunInfo {
	char *blockPtr;
	bool valid;
	unsigned int blocks;
	unsigned int firstBlock;
	unsigned int lastRec;
} RunInfo;

typedef struct OutputInfo {
	char *writePtr;
	unsigned int block;
	unsigned int lastRec;
	FILE *stream;
} OutputInfo;

static bool extSort_RunSort (const char *buffer, const int n, const int bufferSize, int (*compare)(const void *, const void *))
{
	register unsigned int i, j, temp, recSize = SortInfo.attrLength1 + SortInfo.attrLength2; 
	char *bufferPtr;
	RunInfo *run;
	OutputInfo output;

	/* reset the buffer */
	memset(buffer, 0, bufferSize * BLOCKSIZE);
		
	if (!FM_open ("temp", "wb", &output.stream))			
		return -1;							/* open the file to be sorted */

	/* get the first block of the next buffersize - 1 blocks */
	for (i = 0, bufferPtr = buffer; i < bufferSize - 1; i++, bufferPtr += BLOCKSIZE) {
		printf ("%d ", bufferSize * ((n-1) * (bufferSize - 1) + i));
		if (!DM_readBlock (SortInfo.stream, bufferSize * ((n-1) * (bufferSize - 1) + i), bufferPtr)) 
			break;	
	}
	output.writePtr = bufferPtr;
	output.lastRec = 0;
	output.block = 0;

	if ((run = malloc (i * sizeof (struct RunInfo))) == NULL)
		return -1;

	for (j = 0, bufferPtr = buffer; j < i; j++, bufferPtr += BLOCKSIZE) {
		run[j].valid = true;
		run[j].blocks = 0;
		run[j].firstBlock = bufferSize * ((n-1) * (bufferSize - 1) + j);
		run[j].blockPtr = bufferPtr;
		run[j].lastRec = 0;
	}

	while(1) {
		/* get the first valid run */
		for (j = 0; j < i; j++)
			if (run[j].valid == true) {
				temp = j;
				break;
			}

		/* if no run is valid, no run has a block left, finished */
		if (j == i) 
			break;

		/* compare the first value of each valid block */
		for (j = 0; j < i - 1; j++) {
			if (compare(run[temp].blockPtr, run[j+1].blockPtr) > 0 && run[j+1].valid == true)
				temp = j + 1;
		}

		/* and write it to the last buffer block */
		memcpy (output.writePtr, run[temp].blockPtr, recSize);
		output.writePtr += recSize;
		run[temp].blockPtr += recSize;
		output.lastRec++;
		run[temp].lastRec++;

		if (output.lastRec == extSort_entryNm()) {

			output.writePtr = buffer + (bufferSize - 1) * BLOCKSIZE;
			output.lastRec = 0;

#if DEBUG
			for (j = 0; j < 22; j++)
				printf ("%s \n", output.writePtr + recSize * j);	
#endif

			if (!DM_writeBlock (output.stream, output.block++, output.writePtr))
				return -1;

			memset(output.writePtr, 0, BLOCKSIZE);
		}

		/* change the table data */

		
		if (run[temp].lastRec == extSort_entryNm()) {
			run[temp].blocks++;
			run[temp].blockPtr = buffer + temp * BLOCKSIZE;
			run[temp].lastRec = 0;

			memset (run[temp].blockPtr, 0, BLOCKSIZE);

			if (!DM_readBlock (SortInfo.stream, run[temp].firstBlock + run[temp].blocks, run[temp].blockPtr)) 
				run[temp].valid = false;

#if DEBUG			
			printf ("\n\nBlockNm: %d\n", run[temp].firstBlock + run[temp].blocks);
			for (j = 0; j < 22; j++)
				printf ("%d: %s\n", j, run[temp].blockPtr + recSize * j);	
#endif


		}
		if (run[temp].blocks == n * bufferSize)
			run[temp].valid = false;	
	}

	if (!FM_close (output.stream))
		return -1;

	/* cleanup by moving the temporary file back to the original file */
	for (j = 0; j <= output.block; j++) {
		if (!DM_readBlock (output.stream, j, buffer)) 
			return -1;
	
		if (!DM_writeBlock (SortInfo.stream, run[0].firstBlock + j, buffer))
			return -1;
	}

	FM_destroy ("temp");

	free (run);
	
	return SORT_OK;
}



static unsigned long int extSort_entryNm ()
/* returns the number of entries a block is able to hold */
{
	return (int) floor ((double)BLOCKSIZE/(SortInfo.attrLength1 + SortInfo.attrLength2));
}

static unsigned int extSort_rem ()
/* returns the remaining size in the block, if any */
{
	return BLOCKSIZE - (extSort_entryNm() * (SortInfo.attrLength1 + SortInfo.attrLength2));
}

static bool extSort_ptrTable (char ***sortPtr, char *buffer, const int bufferSize)
/* creates a pointer table to the buffer entries */
{
	register unsigned long int j, entries = extSort_entryNm ();
	register int i;
	register unsigned int unused = extSort_rem ();
	char *tempPtr = buffer;

#if DEBUG
	printf ("\nentries: %d, unused: %d\n", entries, unused); 
	printf ("length: %d\n", entries * bufferSize);
#endif

	if ((*sortPtr = malloc (sizeof (char *) * entries * bufferSize)) == NULL)
		return false;
	
	for (i = 0; i < bufferSize; i++, tempPtr += unused) {
		for (j = 0; j < entries; j++, tempPtr += SortInfo.attrLength1 + SortInfo.attrLength2) {
			(*sortPtr)[i * entries + j] = tempPtr;
		}
	}

#if DEBUG
	for (i = 0; i < bufferSize; ++i) 
		for (j = 0; j < entries; ++j) {
			printf ("%d %s\n", i * entries + j, (*sortPtr)[i * entries + j]);
		}
#endif

	return true;
}

static void extSort_bubbleSort (char **sortPtr, const unsigned int length, int (*compare)(const void *, const void*))
{
	bool flag; 
	register unsigned int i;

	do {
		flag = false;
		for (i = 0; i < length - 1; i++)
			if (compare (sortPtr[i], sortPtr[i+1]) > 0) {
				extSort_swap (&sortPtr[i], &sortPtr[i+1]);
				flag = true;

			}
	} while (flag);

#if DEBUG
	printf("last entries: %s, %s\n", sortPtr[i-1], sortPtr[i]);
#endif
}

static void extSort_swap (char **v1, char **v2)
{
	register unsigned int size = SortInfo.attrLength1 + SortInfo.attrLength2;
	char *temp = malloc (size);
	
	memcpy (temp, *v1, size);
	memcpy (*v1, *v2, size);
	memcpy (*v2, temp, size);
	free(temp);
}

static void SortInfoInit (const char attrType1, const int attrLength1, const char attrType2, const int attrLength2) 
{
	SortInfo.attrType1 = attrType1;
	SortInfo.attrType2 = attrType2;
	SortInfo.attrLength1 = attrLength1;
	SortInfo.attrLength2 = attrLength2;
	SortInfo.maxRecs = (int)(BLOCKSIZE/(attrLength1 + attrLength2));
	SortInfo.lastBlock = 0;
	SortInfo.lastRec = -1;
}

static unsigned int SortInfoUpdate ()
{
	if (SortInfo.maxRecs == SortInfo.lastRec + 1) {
		++SortInfo.lastBlock;
		return SortInfo.lastRec = 0;
	}
	else 
		return ++SortInfo.lastRec;
}

static int cmp (const void *value1, const void *value2) 
{
	if (SortInfo.attrType1 == 'i') 
		return memcmp (value1, value2, SortInfo.attrLength1);
	else if (SortInfo.attrType1 == 'c')
		return strcmp (value1, value2);
	else if (SortInfo.attrType1 == 'f') {
		//do stuff
		;
	}
	return 1234;
}