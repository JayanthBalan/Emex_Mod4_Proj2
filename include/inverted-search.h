
#ifndef INVERTED_SEARCH_H
#define INVERTED_SEARCH_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define FILENAME_SIZE 128
#define WORD_SIZE 64
#define ALPHA_COUNT 26

#define STRING_TERMINATOR "\0"
#define STORAGEFILE_DIR "target-files/"
#define STORAGEFILE_NAME "archive"
#define STORAGEFILE_EXTENSION ".dat"
#define STORAGEFILE_PATH STORAGEFILE_DIR STORAGEFILE_NAME STORAGEFILE_EXTENSION STRING_TERMINATOR
#define TEXTFILE_EXTENSION ".txt" STRING_TERMINATOR

typedef enum ret_type {
    pass = 0,
    fail = -1
} ret_type_e;

typedef struct filenames {
    unsigned int wordcount;
    char name[FILENAME_SIZE];
    struct filenames *link;
} filenames_t;

typedef struct hashtable {
    unsigned int idx;
    char word[WORD_SIZE];
    struct hashtable *link;
    filenames_t *fname_list;
} hashtable_t[ALPHA_COUNT];

ret_type_e load_db_file(hashtable_t**);
ret_type_e save_db_file(hashtable_t*);
ret_type_e insert_files_db(hashtable_t**, char*[]);
ret_type_e disp_db(hashtable_t*);
ret_type_e search_db(hashtable_t*);

unsigned int getWordCount(hashtable_t*, unsigned int);
unsigned int getFileCount(hashtable_t*, unsigned int, unsigned int);
ret_type_e getWord(hashtable_t*, unsigned int, unsigned int, char*);

#endif
