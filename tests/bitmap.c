#include <stdio.h>
#include <limits.h>
#include "../src/bitmap.h"

int main(void) {
  clib_bitmap *bitmap = clib_bitmap_init(2);
  printf("%ld\n", bitmap->size);
  bitmap->set(bitmap, 10);
  printf("%d\n", bitmap->get(bitmap, 10));
  int temp = 34;
  printf("%d %d %d %ld\n", bitmap->get(bitmap, temp), CLIB_WORD_BIT, INT_MAX, sizeof(int));
  bitmap->set(bitmap, temp);
  /* this should print 4, if int = 4 bytes */
  printf("%d\n", bitmap->map[1]);
  clib_bitmap_free(bitmap);
  return 0;
}
