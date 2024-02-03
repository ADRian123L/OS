#include "read_line.h"

size_t read_prompt(char **str) {
    char  *input = NULL;
    size_t size  = 0;
    getline(&input, &size, stdin);
    return size;
}

char **seprate(char *str) {
    char **vector_str;
    return vector_str;
}
