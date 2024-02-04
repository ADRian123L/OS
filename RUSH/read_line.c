#include "read_line.h"

// Prompts
char **get_commands(char *str) {
    char  *input = NULL;
    size_t size  = 0;
    // Prompt:
    getline(&input, &size, stdin);
    // Process the inputs:
    char **commands = divide_commands(str);
    inp    inputs;
    // Release the memory:
    free(input);
    return NULL;
}

// Divides the commands
char **divide_commands(char *str) {
    // Creating an array to store the strings
    vec v1;
    v1.capacity = 15;
    v1.size     = 0;
    v1.string   = (char **) calloc(v1.capacity, sizeof(char *));

    // Appending the strings:
    char *strings = NULL;
    while ((strings = strsep(&str, AMPER)) != NULL)
        append_string(&v1, strdup(strings));
    return v1.string;
}

inp **get_tokens(char *str) {
    // Creating an object to store the structures
    v_str v1 = {15, 0, NULL};
    v1.inputs_var =
        (struct inputs **) calloc(v1.capacity, sizeof(struct inputs *));
    inp **tokens_for_each_input;

    // Appending the structures
    char *strings = NULL;
    while ((strings = strsep(&str, WHTSP)) != NULL) {
        append(&v1, strings);
    }
    return tokens_for_each_input;
}
