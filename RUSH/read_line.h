#ifndef READ_LINE
#define READ_LINE

#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Macros:
#define PROMPT "rush> "
#define AMPER "&"
#define REDIR ">"
#define WHTSP " "
#define EMPTY "#"
#define NULLCHAR "\0"

// Structure
typedef struct inputs {
    char  *command;
    bool   redirection;
    char **arguments;
    char **redi_argu;
    size_t argc;
} inp;

// Functions:
inp  **get_commands();
char **divide_commands(char *, const char *);
inp   *get_tokens(char *);
inp  **get_structures(char **);

#endif /* READ_LINE_H */
