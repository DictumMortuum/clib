#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct clib_block {
  bool (*get)(void *buffer, size_t size, size_t count, FILE *stream);
  bool (*set)(const void *buffer, size_t size, size_t count, FILE *stream);
  bool (*geti)(void *buffer, size_t size, size_t count, FILE *stream);
  bool (*seti)(const void *buffer, size_t size, size_t count, FILE *stream);
} clib_block;

clib_block *clib_block_init(void);
void clib_block_free(clib_block *);
