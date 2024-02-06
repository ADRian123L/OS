#ifndef EXECUTE_H
#define EXECUTE_H

#include "read_line.h"
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

// Global variable:
extern char **PATH;
extern char  *error_message;

// Functions:
void execute_childs(comnd_strct **);
void execute_child(comnd_strct *);
bool command_exist(char *, char **);

#endif /* EXECUTE_H */
