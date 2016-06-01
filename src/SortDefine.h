#ifndef EXT_SORT_DEFINE__
#define EXT_SORT_DEFINE__

#define BLOCKSIZE 512
#define SORT_FILE_EXISTS -1
#define SORT_FILE_CREATE -2
#define SORT_FILE_OPEN -3
#define SORT_BLOCK_ALLOC -4
#define SORT_BLOCK_WRITE -5
#define SORT_FILE_CLOSE -6
#define SORT_OK 1
#define SORT_FILE_DESTROY -7

typedef enum {false, true} bool;

#endif
