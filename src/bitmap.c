#include <limits.h>
#include "bitmap.h"

static int  get  (unsigned char,   int);
static void set  (unsigned char *, int);
static void reset(unsigned char *, int);

int bitmapGet(unsigned char *bitmap, int pos)
/* gets the value of the bit at pos */
{
    return get(bitmap[pos/CHAR_BIT], pos%CHAR_BIT);
}

void bitmapSet(unsigned char *bitmap, int pos)
/* sets bit at pos to 1 */
{
    set(&bitmap[pos/CHAR_BIT], pos%CHAR_BIT);
}

void bitmapReset(unsigned char *bitmap, int pos)
/* sets bit at pos to 0 */
{
    reset(&bitmap[pos/CHAR_BIT], pos%CHAR_BIT);
}

int bitmapSearch(unsigned char *bitmap, int n, int size, int start)
/* Finds the first n value in bitmap after start */
/* size is the Bitmap size in bytes */
{
    int i;
    /* size is now the Bitmap size in bits */
    for(i = start+1, size *= CHAR_BIT; i < size; i++)
        if(bitmapGet(bitmap,i) == n)
            return i;
    return -1;
}

static int get(unsigned char a, int pos)
/* pos is something from 0 to 7*/
{
    return (a >> pos) & 1;
}

static void set(unsigned char *a, int pos)
/* pos is something from 0 to 7*/
/* sets bit to 1 */
{
    *a |= 1 << pos;
}

static void reset(unsigned char *a, int pos)
/* pos is something from 0 to 7*/
/* sets bit to 0 */
{
  *a &= ~(1 << pos);
}
