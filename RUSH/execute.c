#include "execute.h"
#include <sys/_types/_s_ifmt.h>
#include <sys/fcntl.h>
#include <unistd.h>

// The function executes all commands:
void execute_childs(comnd_strct **array) {
    comnd_strct **ptr = array;
    while (*ptr != NULL) {
        execute_child(*ptr);
        ++ptr;
    }
}

// The function executes each command
void execute_child(comnd_strct *strct) {

    int pid = fork();
    if (pid == -1) {
        write(STDOUT_FILENO, error_message, strlen(error_message));
    }
    else if (pid == 0) {
        // If the redirection operator was used:
        if (strct->redirection) {
            close(STDOUT_FILENO);
            open(strct->redir_output[0], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        }
        // Check if the command exist if yes execute it:
        char *path = NULL;
        if (command_exist(strct->commands[0], &path) &&
            execv(path, strct->commands) == 0)
            ;
        else
            write(STDOUT_FILENO, error_message, strlen(error_message));
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
