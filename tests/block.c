#include <stdio.h>
#include "../src/block.h"

int main(void) {
  clib_block *block = clib_block_init();

  FILE *fp = fopen("test.bin", "wb");

  double A[5] = {1.,2.,3.,4.,5.};
  block->set(A, sizeof *A, 0, fp);

  clib_block_free(block);
  return 0;
}
