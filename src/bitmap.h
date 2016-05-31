#ifndef BITMAP__
#define BITMAP__

#define BIT (8*sizeof(unsigned char))
#define BITMAP_NOTFOUND -1

bool bitmapGet   (unsigned char *, int);
void bitmapSet   (unsigned char *, int);
void bitmapReset (unsigned char *, int);
int  bitmapSearch(unsigned char *, int, int, int);

#endif
