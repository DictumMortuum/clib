#ifndef EXT_SORT_PROTOTYPES__
#define EXT_SORT_PROTOTYPES__

void Sort_Init (void);
int  Sort_Create_File (char *, char, int, char, int);
int  Sort_Destroy_File (char *);
int  Sort_InsertEntry (char *, void *, void *);
int  Sort_ExtSort (char *, int);
void Sort_PrintError (char *);

#endif
