
#include "ll-ops.h"

static ret_type_e insert_file_db(hashtable_t*, char*);

ret_type_e search_db(hashtable_t ht) {
    char delim[] = " ,;:-", flag;
    char line[WORD_SIZE*8], *word;
    getchar();
    if(fgets(line, sizeof(line), stdin) == NULL)
    {
        return fail;
    }
    line[strcspn(line, "\n")] = '\0';

    word = strtok(line, delim);
    while(word != NULL) {
        int _idx = (unsigned int)((word[0] >= 97) ? (word[0] - 97) : (word[0] - 65));
        if(_idx < 0 || _idx >= ALPHA_COUNT) {
            word = strtok(NULL, delim);
            continue;
        }

        unsigned int idx = (unsigned int)_idx;
        flag = 0;
        fprintf(stdout, "[%d] %s -", idx, word);
        
        for(hashtable_node_t *wordnode = ht[idx]; wordnode != NULL; wordnode = wordnode->link) {
            if(strcmp(wordnode->word, word) == 0) {
                flag = 1;
                for(filenames_t *filenode = wordnode->fname_list; filenode != NULL; filenode = filenode->link) {
                    fprintf(stdout, " (%u:%s)", filenode->wordcount, filenode->name);
                }
                break;
            }
        }
        if(flag == 0) {
            fprintf(stdout, " <NO MATCHES>");
        }
        
        fprintf(stdout, "\n");
        word = strtok(NULL, delim);
    }

    return pass;
}

ret_type_e disp_db(hashtable_t ht) {
    fprintf(stdout, "HASHTABLE\n");
    for(int idx = 0; idx < ALPHA_COUNT; idx++) {
        for(hashtable_node_t *wordnode = ht[idx]; wordnode != NULL; wordnode = wordnode->link) {
            fprintf(stdout, "[%c.%d] ", (char)(idx + 97), idx);
            fprintf(stdout, "%s -", wordnode->word);
            for(filenames_t *filenode = wordnode->fname_list; filenode != NULL; filenode = filenode->link) {
                fprintf(stdout, " (%u:%s)", filenode->wordcount, filenode->name);
            }
            fprintf(stdout, "\n");
        }
    }

    return pass;
}

ret_type_e insert_files_db(hashtable_t *hash, char *args[]) {
    for(int iter = 0; args[iter] != NULL; iter++) {
        if(insert_file_db(hash, args[iter]) != pass) {
            return fail;
        }
    }
    return pass;
}

ret_type_e save_db_file(hashtable_t table) {
    const char *filepath = STORAGEFILE_PATH;
    FILE *fp = fopen(filepath, "wb");
    if(fp == NULL) {
        return fail;
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
                unsigned int file_wc = getFileWC(table, iter1, iter2, iter3);

                if(fwrite(&filename_len, sizeof(unsigned int), 1, fp) != 1) {
                    fclose(fp);
                    return fail;
                }
                if(fwrite(filename, 1, filename_len, fp) != filename_len) {
                    fclose(fp);
                    return fail;
                }
                if(fwrite(&file_wc, sizeof(unsigned int), 1, fp) != 1) {
                    fclose(fp);
                    return fail;
                }
            }
        }
    }

    fclose(fp);
    return pass;
}

ret_type_e load_db_file(hashtable_t *hash) {
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
            if(word_len == 0 || word_len > WORD_SIZE) {
                fclose(fp);
                return fail;
            }

            char word[word_len];
            if(fread(word, 1, word_len, fp) != word_len) {
                fclose(fp);
                return fail;
            }

            if(insertWordToHash(word, hash) != pass) {
                fclose(fp);
                return fail;
            }
            
            for(unsigned int iter3 = 0; iter3 < buff2_file_cnt; iter3++) {
                unsigned int filename_len;
                if(fread(&filename_len, sizeof(unsigned int), 1, fp) != 1) {
                    fclose(fp);
                    return fail;
                }
                if(filename_len == 0 || filename_len > FILENAME_SIZE) {
                    fclose(fp);
                    return fail;
                }

                char filename[filename_len];
                unsigned int file_wc;
                if(fread(filename, 1, filename_len, fp) != filename_len) {
                    fclose(fp);
                    return fail;
                }
                if(fread(&file_wc, sizeof(unsigned int), 1, fp) != 1) {
                    fclose(fp);
                    return fail;
                }

                if(attachFileToWord(word, file_wc, filename, hash) != pass) {
                    fclose(fp);
                    return fail;
                }
            }
        }
    }

    fclose(fp);
    return pass;
}

static ret_type_e insert_file_db(hashtable_t *table, char *file) {
    char buffer[WORD_SIZE];
    char delim[] = ". ,;:-";

    FILE *fp = fopen(file, "rb");
    if(fp == NULL) {
        return fail;
    }

    if(resetFileData(file, table) != pass) {
        fclose(fp);
        return fail;
    }
    while(fscanf(fp, "%63s", buffer) == 1) {
        for(char *word = strtok(buffer, delim); word != NULL; word = strtok(NULL, delim)) {
            if((*word < 65) || (*word > 90 && *word < 97) || (*word > 122)) {
                continue;
            }
            if(insertWordToHash(word, table) != pass) {
                fclose(fp);
                return fail;
            }
            if(attachFileToWord(word, 0, file, table) != pass) {
                fclose(fp);
                return fail;
            }
        }
    }

    fclose(fp);
    return pass;
}
