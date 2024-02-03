#ifndef READ_LINE
#define READ_LINE

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Macros:
#define PROMPT "rush> "
#define AMPER "&"
#define REDIR ">"

size_t read_prompt(char **str);
char **seperate(char *str);
char **get_ope(char *str);

#endif /* READ_LINE_H */
