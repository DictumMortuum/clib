# clib

Migrating the common libraries that I used during my university years to github, using https://matt.sh/howto-c as a guide.

## bitmap

I would love to use uint8_t as a type to represent 'byte', but in this module I can't use the stdint.h types.

```
src/bitmap.c:60:11: error: conversion to ‘uint64_t {aka long unsigned int}’ from ‘int’ may change the sign of the result [-Werror=sign-conversion]
     *byte |= 1 << bit;
           ^
```

I could get past the issue with type casting, as described in the following stackoverflow answers:
- http://stackoverflow.com/questions/18629780/resolving-a-conversion-warning-in-a-compound-assignment
- http://stackoverflow.com/questions/26465295/compiler-warnings-conversion

However, if you have to do bitwise operators, it's probably best to just use the int type, because that way you will get past checks of whether the system uses 32bit or 64bit integers. Additionally, even if you ask for a uint8_t, you will probably get a whole word due to memory alignment.

The purpose of using uint8_t as a type was not to save memory space, but to have more concise and easy-to-read code.
