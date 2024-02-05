#ifndef EXECUTE_H
#define EXECUTE_H

#include "read_line.h"
#include <stdbool.h>
#include <unistd.h>

// Global variable:
extern char **PATH;
extern char  *error_message;

// Functions:
void execute_childs(inp **);
void execute_child(inp *);
bool command_exist(char *, char **);

#endif /* EXECUTE_H */
