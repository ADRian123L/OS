#include "error_check.h"
#include "read_line.h"
#include "vector.h"

void throw_error() {
    char *error_message = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    fflush(stderr);
}

void check_input(comnd_strct ***strct) {

    v_str v1;
    construct(&v1);

    comnd_strct **ptr = *strct;
    while (ptr != NULL && *ptr != NULL) {
        if (check(*ptr))
            append(&v1, copyST(*ptr));
        else
            throw_error();

        ++ptr;
    }
    append(&v1, NULL);

    free_memory(*strct);
    *strct = v1.inputs_var;
}

bool check(comnd_strct *strct) {
    if (strct->num_outputs > 1)
        return false;
    if (!strct->redirection && strct->num_outputs > 0)
        return false;
    if (strct->redirection && strpbrk(strct->redir_output[0], ">&"))
        return false;

    char **ptr = strct->commands;
    while (*ptr != NULL) {
        if (strpbrk(*ptr, ">&") != NULL) {
            return false;
        }
        ++ptr;
    }

    return true;
}

comnd_strct *copyST(comnd_strct *ptr) {
    comnd_strct *cp  = (comnd_strct *) calloc(1, sizeof(comnd_strct));
    cp->commands     = cp_string(ptr->commands);
    cp->num_outputs  = ptr->num_outputs;
    cp->redir_output = cp_string(ptr->redir_output);
    cp->redirection  = ptr->redirection;
    return cp;
}

char **cp_string(char **ptr) {
    vec v1;
    construct_string(&v1);
    char **p = ptr;
    while (*p != NULL) {
        append_string(&v1, strdup(*p));
        ++p;
    }
    append_string(&v1, NULL);
    return v1.string;
}
