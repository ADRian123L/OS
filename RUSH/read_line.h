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
char        **tokens(char *, char *, bool);
comnd_strct **commands(char **);
comnd_strct  *strs(char **);
void          free_memory(comnd_strct **);
comnd_strct  *create_strct(char **, bool, char **, size_t);
char         *cleaner(char *);
void          free_strings(char **);
bool          check(comnd_strct *);

#endif /* READ_LINE_H */
