#ifndef ERROR_CHECK_H
#define ERROR_CHECK_H

#include "execute.h"
#include "read_line.h"
#include <stdio.h>
#include <unistd.h>

// Functions
void         throw_error();
void         check_input(comnd_strct ***);
bool         check(comnd_strct *);
comnd_strct *copyST(comnd_strct *);
char       **cp_string(char **);

#endif /* ERROR_CHECK_H */
