
#include "inverted-search.h"

ret_type_e insert_files_db(hashtable_t **hashtable, char *args[]) {
    

    return pass;
}

ret_type_e save_db_file(hashtable_t *table) {
    const char *filepath = STORAGEFILE_PATH;
    FILE *fp = fopen(filepath, "wb");
    if(fp == NULL) {
        return pass;
    }

    unsigned int buff1_word_cnt[ALPHA_COUNT];
    for(unsigned int iter = 0; iter < ALPHA_COUNT; iter++) {
        buff1_word_cnt[iter] = getWordCount(table, iter);
    }
    if(fwrite(buff1_word_cnt, sizeof(unsigned int), ALPHA_COUNT, fp) != ALPHA_COUNT) {
        fclose(fp);
        return fail;
    }

    for(unsigned int iter1 = 0; iter1 < ALPHA_COUNT; iter1++) {
        unsigned int idx_cnt = buff1_word_cnt[iter1];

        for(unsigned int iter2 = 0; iter2 < idx_cnt; iter2++) {
            char word[WORD_SIZE];
            if(getWord(table, iter1, iter2, word) != pass) {
                fclose(fp);
                return fail;
            }
            unsigned int word_len = strlen(word) + 1;
            unsigned int buff2_file_cnt = getFileCount(table, iter1, iter2);

            if(fwrite(&buff2_file_cnt, sizeof(unsigned int), 1, fp) != 1) {
                fclose(fp);
                return fail;
            }
            if(fwrite(&word_len, sizeof(unsigned int), 1, fp) != 1) {
                fclose(fp);
                return fail;
            }
            if(fwrite(word, 1, word_len, fp) != word_len) {
                fclose(fp);
                return fail;
            }

            for(unsigned int iter3 = 0; iter3 < buff2_file_cnt; iter3++) {
                char filename[FILENAME_SIZE];
                if(getFileName(table, iter1, iter2, iter3, filename) != pass) {
                    fclose(fp);
                    return fail;
                }
                unsigned int filename_len = strlen(filename) + 1;

                if(fwrite(&filename_len, sizeof(unsigned int), 1, fp) != 1) {
                    fclose(fp);
                    return fail;
                }
                if(fwrite(filename, 1, filename_len, fp) != filename_len) {
                    fclose(fp);
                    return fail;
                }
            }
        }
    }

    fclose(fp);
    return pass;
}

ret_type_e load_db_file(hashtable_t **hash) {
    const char *filepath = STORAGEFILE_PATH;
    FILE *fp = fopen(filepath, "rb");
    if(fp == NULL) {
        return pass;
    }

    unsigned int buff1_word_cnt[ALPHA_COUNT];
    if(fread(buff1_word_cnt, sizeof(unsigned int), ALPHA_COUNT, fp) != ALPHA_COUNT) {
        fclose(fp);
        return fail;
    }

    for(unsigned int iter1 = 0; iter1 < ALPHA_COUNT; iter1++) {
        unsigned int idx_cnt = buff1_word_cnt[iter1];

        for(unsigned int iter2 = 0; iter2 < idx_cnt; iter2++) {
            unsigned int buff2_file_cnt, word_len;
            if(fread(&buff2_file_cnt, sizeof(unsigned int), 1, fp) != 1) {
                fclose(fp);
                return fail;
            }
            if(fread(&word_len, sizeof(unsigned int), 1, fp) != 1) {
                fclose(fp);
                return fail;
            }
            char word[word_len];
            if(fread(word, 1, word_len, fp) != word_len) {
                fclose(fp);
                return fail;
            }

            if(insertWordToHash(word) != pass) {
                fclose(fp);
                return fail;
            }
            
            for(unsigned int iter3 = 0; iter3 < buff2_file_cnt; iter3++) {
                unsigned int filename_len;
                if(fread(&filename_len, sizeof(unsigned int), 1, fp) != 1) {
                    fclose(fp);
                    return fail;
                }
                char filename[filename_len];
                if(fread(filename, 1, filename_len, fp) != filename_len) {
                    fclose(fp);
                    return fail;
                }

                if(attachFileToWord(word, filename) != pass) {
                    fclose(fp);
                    return fail;
                }
            }
        }
    }

    fclose(fp);
    return pass;
}
