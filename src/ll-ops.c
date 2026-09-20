
#include "ll-ops.h"

ret_type_e getFileName(hashtable_t ht, unsigned int idx, unsigned int w_idx, unsigned int f_idx, char *file) {
    hashtable_node_t *wordnode = ht[idx];
    for(unsigned int iter = 0; wordnode != NULL && iter < w_idx; iter++, wordnode = wordnode->link);
    filenames_t *filenode = wordnode->fname_list;
    for(unsigned int iter = 0; filenode != NULL && iter < f_idx; iter++, filenode = filenode->link);
    strcpy(file, filenode->name);
    return pass;
}

ret_type_e getWord(hashtable_t ht, unsigned int idx, unsigned int w_idx, char *word) {
    hashtable_node_t *node = ht[idx];
    for(unsigned int iter = 0; node != NULL && iter < w_idx; iter++, node = node->link);
    strcpy(word, node->word);
    return pass;
}

unsigned int getFileWC(hashtable_t ht, unsigned int idx, unsigned int w_idx, unsigned int f_idx) {
    hashtable_node_t *wordnode = ht[idx];
    for(unsigned int iter = 0; wordnode != NULL && iter < w_idx; iter++, wordnode = wordnode->link);
    filenames_t *filenode = wordnode->fname_list;
    for(unsigned int iter = 0; filenode != NULL && iter < f_idx; iter++, filenode = filenode->link);
    return filenode->wordcount;
}

unsigned int getFileCount(hashtable_t ht, unsigned int idx, unsigned int w_idx) {
    hashtable_node_t *node = ht[idx];
    for(unsigned int iter = 0; node != NULL && iter < w_idx; iter++, node = node->link);
    return node->fname_list->file_idx;
}

unsigned int getWordCount(hashtable_t ht, unsigned int idx) {
    return ht[idx]->word_idx;
}

ret_type_e resetFileData(char *file, hashtable_t *ht) {
    for(unsigned int iter1 = 0; iter1 < ALPHA_COUNT; iter1++) {
        for(hashtable_node_t *wordnode = (*ht)[iter1]; wordnode != NULL;) {
            filenames_t *prev_file = NULL;
            filenames_t *filenode = wordnode->fname_list;

            while(filenode != NULL) {
                if(strcmp(filenode->name, file) == 0) {
                    if(prev_file == NULL) {
                        wordnode->fname_list = filenode->link;
                    }
                    else {
                        prev_file->link = filenode->link;
                        for(filenames_t *fn = wordnode->fname_list; fn != filenode; fn = fn->link) {
                            fn->file_idx--;
                        }
                    }
                    free(filenode);
                    break;
                }
                prev_file = filenode;
                filenode = filenode->link;
            }

            if(wordnode->fname_list == NULL) {
                hashtable_node_t *prev_word = NULL;
                hashtable_node_t *node = (*ht)[iter1];
                while(node != NULL && node != wordnode) {
                    prev_word = node;
                    node = node->link;
                }

                if(prev_word == NULL) {
                    (*ht)[iter1] = wordnode->link;
                }
                else {
                    prev_word->link = wordnode->link;
                    for(hashtable_node_t *wn = (*ht)[iter1]; wn != wordnode; wn = wn->link) {
                        wn->word_idx--;
                    }
                }
                hashtable_node_t *next = wordnode->link;
                free(wordnode);
                wordnode = next;
            }
            else {
                wordnode = wordnode->link;
            }
        }
    }

    return pass;
}

ret_type_e attachFileToWord(char *word, unsigned int wc, char *fn, hashtable_t *ht) {
    int _idx = (*word >= 97) ? (*word - 97) : (*word - 65);
    if(_idx < 0 || _idx >= ALPHA_COUNT) {
        return fail;
    }
    unsigned int idx = (unsigned int)_idx;

    hashtable_node_t *_wordnode;
    for(_wordnode = (*ht)[idx]; _wordnode != NULL && strcmp(word, _wordnode->word) != 0; _wordnode = _wordnode->link);
    if(wc == 0) {
        for(filenames_t *filenode = _wordnode->fname_list; filenode != NULL; filenode = filenode->link) {
            if(strcmp(filenode->name, fn) == 0) {
                filenode->wordcount++;
                return pass;
            }
        }
    }
    if(_wordnode == NULL) {
        return fail;
    }

    filenames_t *filenode = (filenames_t*)malloc(sizeof(filenames_t));
    if(filenode == NULL) {
        return fail;
    }
    
    strcpy(filenode->name, fn);
    filenode->link = _wordnode->fname_list;
    filenode->file_idx = (_wordnode->fname_list == NULL) ? 1 : (_wordnode->fname_list->file_idx + 1);
    filenode->wordcount = (wc == 0) ? 1 : wc;
    _wordnode->fname_list = filenode;

    return pass;
}

ret_type_e insertWordToHash(char *word, hashtable_t *ht) {
    int _idx = (*word >= 97) ? (*word - 97) : (*word - 65);
    if(_idx < 0 || _idx >= ALPHA_COUNT) {
        return fail;
    }
    unsigned int idx = (unsigned int)_idx;

    for(hashtable_node_t *wordnode = (*ht)[idx]; wordnode != NULL; wordnode = wordnode->link) {
        if(strcmp(wordnode->word, word) == 0) {
            return pass;
        }
    }

    if((*ht)[idx] == NULL) {
        (*ht)[idx] = (hashtable_node_t*)malloc(sizeof(hashtable_node_t));
        if((*ht)[idx] == NULL) {
            return fail;
        }

        strcpy((*ht)[idx]->word, word);
        (*ht)[idx]->link = NULL;
        (*ht)[idx]->word_idx = 1;
        (*ht)[idx]->fname_list = NULL;
        
        return pass;
    }

    hashtable_node_t *wordnode = (hashtable_node_t*)malloc(sizeof(hashtable_node_t));
    if(wordnode == NULL) {
        return fail;
    }

    strcpy(wordnode->word, word);
    wordnode->fname_list = NULL;
    wordnode->word_idx = (*ht)[idx]->word_idx + 1;
    wordnode->link = (*ht)[idx];

    (*ht)[idx] = wordnode;

    return pass;
}

