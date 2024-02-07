#ifndef READ_LINE
#define READ_LINE

#include "vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Macros:
#define PROMPT "rush> "
#define AMPERS "&"
#define REDIRS ">"
#define WHTSPS " "
#define EMPTYS "#"
#define TABS "\t"
#define NULLCHARS "\0"
#define AMPER '&'
#define REDI '>'
#define NEWL '\n'
#define ENDC '\0'

// Structure
typedef struct inputs {
    char **commands;
    bool   redirection;
    char **redir_output;
    size_t num_outputs;
} comnd_strct;

// Functions:
comnd_strct **get_commands();
char        **tokens(char *);
comnd_strct **commands(char **);
void          free_memory(comnd_strct **);
comnd_strct  *command(char **start, char **end);
comnd_strct  *create_strct(char **, bool, char **, size_t);
void          free_strings(char **);

#endif /* READ_LINE_H */
