#include "read_line.h"

// Prompts
inp **get_commands() {
    char  *input = NULL;
    size_t size  = 0;
    // Prompt:
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
    getline(&input, &size, stdin);

    // Process the inputs:
    char **commands        = divide_commands(input, AMPER);
    inp  **structure_array = get_structures(commands);

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

    // Get the command and its arguments:
    char **array_string      = divide_commands(str, WHTSP);
    present_command->command = array_string[0]; // Store the command

    // Check for redirection operators:
    char **starting_point_redi = NULL;
    size_t counter             = 0;
    for (char **ptr = array_string + 1; *ptr != NULL; ++ptr, ++counter)
        if (strcmp(*ptr, REDIR) == 0) {
            present_command->redirection = true;
            *ptr                         = NULL;
            starting_point_redi          = ptr + 1;
            --counter;
        }
    present_command->arguments = array_string + 1;
    present_command->redi_argu = starting_point_redi;
    present_command->argc      = counter;

    return present_command;
}

inp **get_structures(char **string_array) {
    // Allocate the structures:
    v_str vector = {15, 0, NULL};
    vector.inputs_var =
        (struct inputs **) calloc(vector.capacity, sizeof(struct inputs *));

    // Append the structures:
    char **ptr = string_array;
    while (*ptr != NULL) {
        append(&vector, get_tokens(*ptr));
        ++ptr;
    }
    append(&vector, NULL);
    return vector.inputs_var;
}
