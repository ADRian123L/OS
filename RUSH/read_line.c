#include "read_line.h"
#include "error_check.h"

// Prompts
comnd_strct **get_commands() {
    // Prompt:
    char  *input = NULL;
    size_t num;
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
    getline(&input, &num, stdin);

    // Parse the input
    comnd_strct **array_struct = commands(tokens(input));

    // Release the memory:
    free(input);

    // Return the array of structures
    return array_struct;
}

// Parse the input into tokens:
char **tokens(char *str) {
    // Create a vector of strings to store the tokens:
    vec v1;
    construct_string(&v1);

    // Separate the string into tokens
    char *token = NULL;
    while ((token = strsep(&str, " \t\n")) != NULL)
        if (strcmp(token, NULLCHARS) != 0)
            append_string(&v1, strdup(token));
    append_string(&v1, NULL);

    // Return the array of strings
    return v1.string;
}

// The function stores the commands in array string
comnd_strct **commands(char **array_strings) {
    // Create a vector to store the structures:
    v_str v1;
    construct(&v1);

    // Separate the commands:
    char **ptr = array_strings, **from;
    for (from = ptr; *ptr != NULL; ++ptr)
        if (**ptr == AMPER && strlen(*ptr) == 1) {
            append(&v1, command(from, ptr));
            from = ptr + 1;
        }
    append(&v1, command(from, ptr));
    append(&v1, NULL);

    free_strings(array_strings);
    return v1.inputs_var;
}

// The function stores each command's arguments
comnd_strct *command(char **start, char **end) {
    // Create a vector to store the strings
    vec v1, v2;
    construct_string(&v1);
    construct_string(&v2);
    bool   redi  = false;
    size_t count = 0;

    // Create a string of the arguments
    char **ptr;
    for (ptr = start; ptr < end && **ptr != REDI; ++ptr)
        append_string(&v1, strdup(*ptr));
    append_string(&v1, NULL);
    if (*ptr != NULL && **ptr == REDI) {
        redi = true;
        for (char **ptr2 = ptr + 1; ptr2 < end; ++ptr2, count++)
            append_string(&v2, strdup(*ptr2));
        append_string(&v2, NULL);
    }

    // Structure for storing command
    comnd_strct *new_strct = create_strct(v1.string, redi, v2.string, count);
    return new_strct;
}

// The function creates a structure object
comnd_strct *
create_strct(char **command, bool redi, char **out_redi, size_t num_out) {
    comnd_strct *new_strct  = (comnd_strct *) calloc(1, sizeof(comnd_strct));
    new_strct->commands     = command;
    new_strct->redirection  = redi;
    new_strct->redir_output = out_redi;
    new_strct->num_outputs  = num_out;
    return new_strct;
}

// The functions frees the memory used by the strings array
void free_strings(char **array_string) {
    for (char **ptr = array_string; *ptr != NULL; ++ptr) {
        free(*ptr);
        *ptr = NULL;
    }
    free(array_string);
    array_string = NULL;
}

// The function frees the memory used by the structures
void free_memory(comnd_strct **array) {
    for (comnd_strct **ptr = array; ptr != NULL && *ptr != NULL; ++ptr) {
        for (char **ptr2 = (*ptr)->commands; *ptr2 != NULL; ++ptr2) {
            free(*ptr2);
            *ptr2 = NULL;
        }
        free((*ptr)->commands);
        (*ptr)->commands = NULL;

        for (char **ptr2 = (*ptr)->redir_output; *ptr2 != NULL; ++ptr2) {
            free(*ptr2);
            *ptr2 = NULL;
        }
        free((*ptr)->redir_output);
        (*ptr)->redir_output = NULL;
    }
    free(array);
    array = NULL;
}
