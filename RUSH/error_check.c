#include "error_check.h"

void throw_error() {
    char *error_message = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
    fflush(stderr);
}

bool check_input(comnd_strct **strct) {
    bool flag = true;
    return flag;
}
