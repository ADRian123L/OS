#ifndef ERROR_CHECK_H
#define ERROR_CHECK_H

#include "execute.h"
#include "read_line.h"
#include <stdio.h>
#include <unistd.h>

// Functions
void throw_error();
bool check_input(comnd_strct **);

#endif /* ERROR_CHECK_H */
