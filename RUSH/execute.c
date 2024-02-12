#include "execute.h"
#include "error_check.h"
#include "vector.h"
#include <stdlib.h>

// The function executes all commands:
void execute_childs(comnd_strct **array) {
    comnd_strct **ptr = array;
    char          nums[15];
    vec           number;
    construct_string(&number);
    while (*ptr != NULL) {
        sprintf(nums, "%d", execute_child(*ptr));
        append_string(&number, nums);
        ++ptr;
    }
    char **pt = number.string;
    while (*pt != NULL)
        waitpid(atoi(*(pt++)), NULL, 0);
}

// The function executes each command
int execute_child(comnd_strct *strct) {
    int pid = fork();
    if (pid < 0) {
        throw_error();
        return -1;
    }
    else if (pid == 0) {
        // If the redirection operator was used :
        if (strct->redirection) {
            close(STDOUT_FILENO);
            int rc = open(
                strct->redir_output[0], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
            if (rc < 0) {
                throw_error();
                fflush(stderr);
                exit(EXIT_FAILURE);
            }
        }
        // Check if the command exist if yes execute it :
        char *path = NULL;
        if (command_exist(strct->commands[0], &path) == true) {
            execv(path, strct->commands);
            throw_error();
            exit(EXIT_FAILURE);
        }
        else {
            throw_error();
            exit(EXIT_FAILURE); // Exit the child
        }
    }
    else {
        return pid;
    }
}

// The function checks if a command exist
bool command_exist(char *str, char **path) {
    char **ptr = PATH;
    while (*ptr != NULL) {

        size_t size = strlen(*ptr) + strlen(str);
        *path       = (char *) calloc(size + 2, sizeof(char));

        strcat(*path, *ptr);
        strcat(*path, "/");
        strcat(*path, str);

        if (access(*path, X_OK) == 0) return true;
        free(*path);
        ++ptr;
    }
    return false;
}
