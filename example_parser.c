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
static void print_dirs(
    const struct cppd_t* handle, 
    const struct Node* node, 
    unsigned int level) 
{
    struct cppd_dirent_t de;

    for (unsigned int i = 0;; ++i) 
    {
        if (!cppd_readdir(handle, node, i, &de)) 
        {
            for (unsigned int t = 0; t < level; ++t) 
            {
                printf("\t");
            }

            printf("%s (%d)\r\n", de.name, de.type);

            if (de.type == 0) /* type 0 means folder */ 
            { 
                print_dirs(handle, de.data, level + 1);
            }
        } 
        else 
        {
            break;
        }
    }
}

int main(int argc, const char *argv[])
{
    static uint64_t buf[1000];

    if (argc < 2)
    {
        perror("Please specify input file");
        return EXIT_FAILURE;
    }

    const char* fname = argv[1];
    FILE* const fp = fopen(fname, "rb");

    if (!fp) 
    {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    fseek(fp, 0, SEEK_END);
    const size_t siz = ftell(fp);
    rewind(fp);

    if (siz > sizeof(buf)) 
    {
        perror("Too big binary data file.");
        return EXIT_FAILURE;
    }

    const size_t read_bytes = fread(buf, 1, siz, fp);

    printf("Read %ld bytes\r\n", read_bytes);
    
    struct cppd_t handle;

    if (cppd_init(&handle, buf)) 
    {
        perror("Wrong binary data file.");
        return EXIT_FAILURE;
    }

    const struct Node* node = cppd_open(&handle, "/");
    print_dirs(&handle, node, 0);
    fclose(fp);

    return EXIT_SUCCESS;
}





