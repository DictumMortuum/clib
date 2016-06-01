#include "compare.h"
#include <stdlib.h>
#include <string.h>

int compare_equal (const void *value1, const void *value2, const unsigned int size, const char attrType)
/* checks if two values are equal */
/* problem is, floating point numbers don't work with memcmp */
/* also, memcmp for strings is a little buggy, because after the end of the string */
/* '\0' you can have any character. So memcmp will fail, that's why i had to use */
/* strcmp, which compares the two strings until it reaches the first '\0' */
{
	if (attrType == 'i') {
		return !memcmp (value1, value2, size);
  }	else if (attrType == 'c') {
		return !strcmp (value1, value2);
  }	else if (attrType == 'f') {
		float f1 = *(float *)value1;
		float f2 = *(float *)value2;
		float precision = 0.0001;

		if (((f1 - precision) < f2) && ((f1 + precision) > f2)) {
			return 1;
		} else {
			return 0;
		}
	}
}

int compare_not_equal (const void *value1, const void *value2, const unsigned int size, const char attrType)
/* just return the !equal result */
{
	return !equal (value1, value2, size, attrType);
}
