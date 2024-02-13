#include "built_in.h"

extern char **PATH; // The path to bin

// The function checks if the input is a built-in command
bool built_in(comnd_strct ***struct_array) {
    bool  flag = true;
    v_str v1;
    construct(&v1);
    comnd_strct **ptr = *struct_array;
    while (*ptr != NULL) {
        if (strcmp((*ptr)->commands[0], PTH) == 0)
            update_path(*ptr);
        else if (strcmp((*ptr)->commands[0], EXIT) == 0)
            if ((*ptr)->commands[1] == NULL)
                flag = false;
            else
                throw_error();
        else if (strcmp((*ptr)->commands[0], CD) == 0)
            change_dir(*ptr);
        else
            // Append to the new array structure
            append(&v1, copyST(*ptr));
        ++ptr;
    }
    append(&v1, NULL);
    free_memory(*struct_array);
    *struct_array = v1.inputs_var;
    return flag;
}

// The function updates the path
void update_path(comnd_strct *strct) {
    vec v1;
    construct_string(&v1);
    char **ptr = strct->commands + 1;
    while (*ptr != NULL) {
        append_string(&v1, strdup(*ptr));
        ++ptr;
    }
    append_string(&v1, NULL);
    free_strings(PATH);
    PATH = v1.string;
}

// The function changes the directory
void change_dir(comnd_strct *strct) {
    if (strct->commands[1] != NULL || strct->commands[2] == NULL) {
        if (chdir(strct->commands[1]) == -1)
            throw_error();
    }
}
