//
// Example of binary data parsing.
//

#include <stdio.h>
#include <stdlib.h>
#include "cppdata.h"

//
// Since it is expected that no more than 2-3 levels of nesting I prefer
// recursive function :-P
//
static void print_dirs(const struct cppd_t* handle, const struct Node* node, unsigned int level) {
    struct cppd_dirent_t de;

    for (unsigned int i = 0;; ++i) {
        
        if (!cppd_readdir(handle, node, i, &de)) {
            
            for (unsigned int t = 0; t < level; ++t) {
                printf("\t");
            }

            switch (de.type) {
            case CPPD_FOLDER: 
                printf("folder '%s':\r\n", de.name); 
                break;

            case CPPD_NUMBER: 
                printf(
                    "number '%s' value=(%ld)\r\n", 
                    de.name, 
                    *(uint64_t*)cppd_dataptr(handle, de.data, 0)
                );
                break;

            case CPPD_STRING: 
                printf(
                    "string '%s' value=(%s) length=%ld bytes\r\n", 
                    de.name, 
                    (const char*)cppd_dataptr(handle, de.data, 0), de.size
                ); 
                break;

            case CPPD_BLOB: 
                printf(
                    "binary blob '%s' (%ld bytes)\r\n", 
                    de.name, 
                    de.size
                ); 
                break;

            default: break;
            }

            if (de.type == CPPD_FOLDER) {
                print_dirs(handle, de.data, level + 1);
            }
        } else {
            break;
        }
    }
}

int main(int argc, const char *argv[]) {
    static uint64_t buf[1000];

    if (argc < 2) {
        perror("Please specify input file");
        return EXIT_FAILURE;
    }

    const char* fname = argv[1];
    FILE* const fp = fopen(fname, "rb");

    if (!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    const size_t siz = ftell(fp);
    rewind(fp);

    if (siz > sizeof(buf)) {
        perror("Too big binary data file.");
        return EXIT_FAILURE;
    }

    const size_t read_bytes = fread(buf, 1, siz, fp);

    printf("Read %ld bytes\r\n", read_bytes);
    
    struct cppd_t handle;

    if (cppd_init(&handle, buf)) {
        perror("Wrong binary data file.");
        return EXIT_FAILURE;
    }

    const struct Node* node = cppd_open(&handle, "/");
    print_dirs(&handle, node, 0);

    const char* const test_path = "/folder/nested/three";
    const struct Node* const value_node = cppd_open(&handle, test_path);
    
    if (value_node == 0) {
        perror("Can't find the path specified.");
        return EXIT_FAILURE;
    }

    struct cppd_dirent_t de;
    const uint64_t* const num_ptr = cppd_dataptr(&handle, value_node, 0);

    if (num_ptr) {
        printf("value at %s = (%ld)\r\n", test_path, *num_ptr);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}
