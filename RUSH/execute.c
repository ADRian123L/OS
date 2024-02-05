#include "execute.h"
#include <unistd.h>

void execute_childs(inp **array) {
    inp **ptr = array;
    while (*ptr != NULL) {
        execute_child(*ptr);
        ++ptr;
    }
}

// The function executes each command
void execute_child(inp *strct) {

    int pid = fork();
    if (pid == -1) {
        write(STDOUT_FILENO, error_message, strlen(error_message));
    }
    else if (pid == 0) {
        // Check if the command exist:

        char *path = NULL;

        if (command_exist(strct->command, &path) &&
            execv(path, strct->whole_array) == 0)
            ;
        // Call execute vector:
        else {
            write(STDOUT_FILENO, error_message, strlen(error_message));
        }
    }
    else {
        wait(NULL);
    }
}

// The function checks if a command exist
bool command_exist(char *str, char **path) {
    char **ptr = PATH;
    while (*ptr != NULL) {

        size_t size = strlen(*ptr) + strlen(str);
        *path       = (char *) calloc(size + 2, sizeof(char));

        strcat(*path, *ptr);
        strcat(*path, str);

        if (access(*path, X_OK) == 0) return true;
        free(*path);
        ++ptr;
    }
    return false;
}
