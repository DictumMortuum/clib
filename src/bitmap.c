#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>

/* bitwise functions */
static bool get (int, int);
static void set (int *, int);
static void reset (int *, int);

/* bitmap functions */
static bool bitmap_get (clib_bitmap *, int);
static void bitmap_set (clib_bitmap *, int);
static void bitmap_reset (clib_bitmap *, int);

clib_bitmap *clib_bitmap_init (uint64_t size)
/* Creates an empty bitmap with @size integers */
{
  clib_bitmap *temp = (clib_bitmap *) calloc (1, sizeof(clib_bitmap));
  temp->map = (int *) calloc (size, sizeof(int));
  temp->size = size;
  temp->get = &bitmap_get;
  temp->set = &bitmap_set;
  temp->reset = &bitmap_reset;
  return temp;
}

void clib_bitmap_free (clib_bitmap *bitmap)
/* Destroys the @bitmap */
{
  free (bitmap->map);
  free (bitmap);
}

static bool bitmap_get (clib_bitmap *self, int n)
/* Returns the value of the @n'th bit of the bitmap */
{
  return get (self->map[n/CLIB_WORD_BIT], n%CLIB_WORD_BIT);
}

static void bitmap_set (clib_bitmap *self, int n)
/* Sets the @n'th bit of the bitmap to true */
{
  set (&self->map[n/CLIB_WORD_BIT], n%CLIB_WORD_BIT);
}

static void bitmap_reset (clib_bitmap *self, int n)
/* Sets the @n'th bit of the bitmap to false */
{
  reset (&self->map[n/CLIB_WORD_BIT], n%CLIB_WORD_BIT);
}

static bool get (int byte, int bit)
/* Returns the value of @byte at @bit */
{
  return (byte >> bit) & 1;
}

static void set (int *byte, int bit)
/* Sets @byte's @bit to 1 */
{
  *byte |= 1 << bit;
}

static void reset (int *byte, int bit)
/* Sets @byte's @bit to 0 */
{
  *byte &= ~(1 << bit);
}
