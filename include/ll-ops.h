
#ifndef LL_OPS_H
#define LL_OPS_H

#include "inverted-search.h"

unsigned int getWordCount(hashtable_t, unsigned int);
unsigned int getFileCount(hashtable_t, unsigned int, unsigned int);
unsigned int getFileWC(hashtable_t, unsigned int, unsigned int, unsigned int);
ret_type_e getWord(hashtable_t, unsigned int, unsigned int, char*);
ret_type_e getFileName(hashtable_t, unsigned int, unsigned int, unsigned int, char*);

ret_type_e insertWordToHash(char*, hashtable_t*);
ret_type_e attachFileToWord(char*, unsigned int, char*, hashtable_t*);
ret_type_e resetFileData(char*, hashtable_t*);

#endif
