#pragma once
#include <stdint.h>
#include <stdbool.h>

#define WORD_BIT (int)(sizeof(int)*8)

typedef struct clib_bitmap {
  int *map;
  uint64_t size;
  bool (*get)(struct clib_bitmap *, int);
  void (*set)(struct clib_bitmap *, int);
  void (*reset)(struct clib_bitmap *, int);
} clib_bitmap;

clib_bitmap *clib_bitmap_init(uint64_t size);
void clib_bitmap_free(clib_bitmap *bitmap);
