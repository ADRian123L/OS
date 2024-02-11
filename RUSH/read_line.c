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
    comnd_strct **array_struct = commands(tokens(input, AMPERS));
    // Release the memory:
    free(input);
    // Return the array of structures
    return array_struct;
}

// The function separates a strings by some delimiter
char **tokens(char *str, char *delims) {
    vec v1;
    construct_string(&v1);
    // Separate the strings by some delimiter
    char *token;
    while ((token = strsep(&str, delims)) != NULL)
        if (*token != ENDC) append_string(&v1, strdup(token));
    append_string(&v1, NULL);
    return v1.string;
}

// The function stores the commands in array string
comnd_strct **commands(char **array_strings) {
    // Create a vector to store the structures:
    v_str v1;
    construct(&v1);
    // Separate the commands:
    char **ptr = array_strings;
    while (*ptr != NULL)
        append(&v1, strs(tokens(*(ptr++), " \t\n")));
    append(&v1, NULL);
    // Return the array of structures
    free_strings(array_strings);
    return v1.inputs_var;
}

comnd_strct *strs(char **str) {
    vec v1, v2;
    construct_string(&v1);
    construct_string(&v2);
    bool   redi  = false;
    size_t count = 0;
    // Separate and create structures containing the commands
    char **ptr = str;
    while (*ptr != NULL && **ptr != REDI)
        append_string(&v1, strdup(*(ptr++)));
    append_string(&v1, NULL);
    if (*ptr != NULL && **ptr == REDI) {
        redi         = true;
        char **token = tokens(*ptr, ">");
        for (char **ptrt = token; *ptrt != NULL; ++ptrt)
            append_string(&v2, strdup(*ptrt));
        append_string(&v2, NULL);
        free_strings(token);
    }
    free_strings(str);
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
