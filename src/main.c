
#include "inverted-search.h"

static ret_type_e validateCLI(char *[]);

int main(int argc, char *argv[]) {
    uint8_t hash_modified = 0;
    hashtable_t *table = {0};
    if(load_db_file(&table) != pass) {
        fprintf(stderr, "%s>> Load database failure\n", __FILE__);
        return -1;
    }

    if(argc > 1 && validateCLI(argv) == pass) {
        if(insert_files_db(&table, argv) != pass) {
            hash_modified = 1;
            fprintf(stderr, "%s>> Update database failure\n", __FILE__);
            return -1;
        }
    }

    unsigned char runprog = 1;
    while(runprog) {
        fprintf(stdout, "Database Options\n");
        fprintf(stdout, "1. Display Database\n2. Search Database\n3. Close Database\n");
        unsigned char option;
        fscanf(stdin, "%hhu", &option);
        switch(option) {
            case 1:
                if(disp_db(table) != pass) {
                    fprintf(stderr, "%s>>> Display database failure\n", __FILE__);
                    return -1;
                }
                break;
            case 2:
                if(search_db(table) != pass) {
                    fprintf(stderr, "%s>>> Display database failure\n", __FILE__);
                    return -1;
                }
                break;
            case 3:
                runprog = 0;
                break;
            default:
                fprintf(stderr, "%s>>> Invalid Option\n", __FILE__);
        }
    }

    if(hash_modified) {
        if(save_db_file(table) != pass) {
            fprintf(stderr, "%s>>> Save database failure\n", __FILE__);
            return -1;
        }
    }

    return 0;
}

static ret_type_e validateCLI(char *args[]) {
    for(int iter = 0; args[iter] != NULL; iter++) {
        if(strstr(args[iter], TEXTFILE_EXTENSION) != NULL) {
            return fail;
        }
        int fd = open(args[iter], O_WRONLY | O_CREAT | O_EXCL, 0644);
        if(fd < 0) {
            if (errno == ENOENT || errno == EACCES) {
                return fail;
            }
        }
    }
    return pass;
}
