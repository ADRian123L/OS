#ifndef READ_LINE
#define READ_LINE

#include "vector.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Macros:
#define PROMPT "rush> "
#define AMPER "&"
#define REDIR ">"
#define WHTSP " "
#define EMPTY "#"

// Structure
typedef struct inputs {
    char  *command;
    int    redirection;
    char **arguments;
    char **redi_argu;
    size_t argc;
} inp;

// Functions:
inp  **read_prompt();
char **get_commands(char *);
char **divide_commands(char *, const char *);
inp   *get_tokens(char *);

#endif /* READ_LINE_H */
