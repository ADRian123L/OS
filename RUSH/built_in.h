#ifndef BUILT_IN_H
#define BUILT_IN_H

#include "error_check.h"
#include "read_line.h"

// Macros:
#define PTH "path"
#define EXIT "exit"
#define CD "cd"

// Functions:
bool built_in(comnd_strct ***);
void update_path(comnd_strct *);
void change_dir(comnd_strct *);

#endif /* BUILT_IN_H */
