#include "read_line.h"

// Prompts
char **get_commands(char *str) {
    char  *input = NULL;
    size_t size  = 0;
    // Prompt:
    getline(&input, &size, stdin);
    // Process the inputs:
    char **commands = divide_commands(str, AMPER);
    inp    inputs;
    // Release the memory:
    free(input);
    return NULL;
}

// Divides the commands
char **divide_commands(char *str, const char *delim) {
    // Creating an array to store the strings
    vec v1;
    v1.capacity = 15;
    v1.size     = 0;
    v1.string   = (char **) calloc(v1.capacity, sizeof(char *));

    // Appending the strings:
    char *strings = NULL;
    while ((strings = strsep(&str, delim)) != NULL)
        append_string(&v1, strdup(strings));
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
            present_command->redirection = 1;
            *ptr                         = NULL;
            starting_point_redi          = ptr + 1;
            --counter;
        }
    present_command->arguments = array_string;
    present_command->redi_argu = starting_point_redi;
    present_command->argc      = counter;

    return present_command;
}
