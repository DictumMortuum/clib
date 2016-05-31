#ifndef CLIB_BITMAP
#define CLIB_BITMAP

int  bitmapGet   (unsigned char *, int);
void bitmapSet   (unsigned char *, int);
void bitmapReset (unsigned char *, int);
int  bitmapSearch(unsigned char *, int, int, int);

#endif
