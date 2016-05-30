#ifndef BITMAP__
#define BITMAP__

#define BIT (8*sizeof(byte))
#define BITMAP_NOTFOUND -1

typedef enum{false=0, true} bool;
typedef unsigned char byte;

bool bitmapGet   (byte *, int);
void bitmapSet   (byte *, int);
void bitmapReset (byte *, int);
int  bitmapSearch(byte *, bool, int, int);

#endif
