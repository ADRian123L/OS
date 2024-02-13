#ifndef EXECUTE_H
#define EXECUTE_H

#include "error_check.h"
#include "read_line.h"
#include <fcntl.h>
#include <stdbool.h>
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

// Global variable:
extern char **PATH;
extern char  *error_message;

// Functions:
void execute_childs(comnd_strct **);
int  execute_child(comnd_strct *);
bool command_exist(char *, char **);

#endif /* EXECUTE_H */
