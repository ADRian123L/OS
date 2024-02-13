#include "error_check.h"

// The function throws an error
void throw_error() {
    char *error_message = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    fflush(stderr);
}

// The function checks the input
void check_input(comnd_strct ***strct) {
    // A new array of structures
    v_str v1;
    construct(&v1); // Construct the vector
    comnd_strct **ptr = *strct;
    while (ptr != NULL && *ptr != NULL) {
        if (check(*ptr)) // Check if the input is valid
            append(&v1, copyST(*ptr));
        else
            throw_error();
        ++ptr;
    }
    append(&v1, NULL);

    free_memory(*strct);
    *strct = v1.inputs_var; // Assign the new array to the old one
}

// The function checks if the input is valid
bool check(comnd_strct *strct) {
    if (*strct->commands == NULL && strct->commands)
        return false;
    if (strct->redirection && strct->num_outputs == 0)
        return false;
    if (strct->num_outputs > 1)
        throw_error();
    for (char **ptr = strct->commands; *ptr != NULL; ++ptr)
        if (strpbrk(*ptr, ">&") != NULL)
            return false;
    if (!strct->redirection && strct->num_outputs > 0)
        return false;
    return true;
}

// The function frees the memory
comnd_strct *copyST(comnd_strct *ptr) {
    comnd_strct *cp  = (comnd_strct *) calloc(1, sizeof(comnd_strct));
    cp->commands     = cp_string(ptr->commands);
    cp->num_outputs  = ptr->num_outputs;
    cp->redir_output = cp_string(ptr->redir_output);
    cp->redirection  = ptr->redirection;
    return cp; // Return the copied structure
}

// The function copies a string
char **cp_string(char **ptr) {
    vec v1;
    construct_string(&v1);
    char **p = ptr;
    while (*p != NULL)
        append_string(&v1, strdup(*(p++))); // Append the string to the vector
    append_string(&v1, NULL);
    return v1.string;
}
