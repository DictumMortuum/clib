#ifndef BLOCK_MANAGEMENT_PROTOTYPES__
#define BLOCK_MANAGEMENT_PROTOTYPES__

#include ".\SortDefine.h"

bool  BM_AllocBlock  (char **, const unsigned long, const void *, const unsigned int);

int  equal			 (const void *, const void *, const unsigned int, const char);
int  not_equal		 (const void *, const void *, const unsigned int, const char);

#endif