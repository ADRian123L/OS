#include "read_line.h"

// Prompts
inp **get_commands(size_t *size) {
    char *input = NULL;
    // Prompt:
    size_t num;
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
    getline(&input, &num, stdin);

    // Process the inputs:
    char **commands        = divide_commands(input, AMPER);
    inp  **structure_array = get_structures(commands, size);

    // Release the memory:
    free(input);
    return structure_array;
}

// Divides the commands
char **divide_commands(char *str, const char *delim) {
    // Creating an array to store the strings
    vec v1    = {15, 0, NULL};
    v1.string = (char **) calloc(v1.capacity, sizeof(char *));

    // Appending the strings:
    char *strings = NULL;
    while ((strings = strsep(&str, delim)) != NULL)
        ((strcmp(strings, NULLCHAR) != 0) ? append_string(&v1, strdup(strings))
                                          : NULL);
    append_string(&v1, NULL); // Append NULL
    return v1.string;
}

inp *get_tokens(char *str) {
    // Allocate a structure to store the command:
    inp *present_command = (inp *) calloc(1, sizeof(inp));

    // Structure for storing the output files:
    vec v1    = {15, 0, NULL};
    v1.string = (char **) calloc(v1.capacity, sizeof(char *));

    // Get the command and its arguments:
    char **array_string          = divide_commands(str, WHTSP);
    present_command->whole_array = array_string;
    present_command->command     = array_string[0]; // Store the command

    // Check for redirection operators:
    size_t counter = 0;
    for (char **ptr = array_string + 1; *ptr != NULL; ++ptr)
        if (strcmp(*ptr, REDIR) == 0 && !present_command->redirection) {
            present_command->redirection = true;
            *ptr                         = strdup(NULLCHAR);
            append_string(&v1, *(ptr + 1));
            counter++;
        }
        else if (strcmp(*ptr, REDIR) == 0) {
            append_string(&v1, *(ptr + 1));
            counter++;
        }
    append_string(&v1, NULL);
    present_command->arguments = array_string + 1;
    present_command->redi_argu = v1.string;
    present_command->redi_argc = counter;

    return present_command;
}

inp **get_structures(char **string_array, size_t *size) {
    // Allocate the structures:
    v_str vector = {15, 0, NULL};
    vector.inputs_var =
        (struct inputs **) calloc(vector.capacity, sizeof(struct inputs *));

    // Append the structures:
    *size      = 0;
    char **ptr = string_array;
    while (*ptr != NULL) {
        append(&vector, get_tokens(*ptr));
        ++ptr;
        (*size)++;
    }
    append(&vector, NULL);
    return vector.inputs_var;
}

void free_memory(inp **array) {
    for (inp **ptr = array; *ptr != NULL; ++ptr) {

        for (char **ptr_char = (*ptr)->whole_array; *ptr_char != NULL;
             ++ptr_char)
            free(*ptr_char);

        free((*ptr)->whole_array);
        free((*ptr)->redi_argu);
        free(*ptr);
    }
    free(array);
}
