#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "vector.h"
struct inputs;

// Structures
typedef struct vector {
    size_t capacity;
    size_t size;
    char **string;
} vec;

typedef struct vector_struct {
    size_t          capacity;
    size_t          size;
    struct inputs **inputs_var;
} v_str;

// Functions:
void append(v_str *, struct inputs *);
void append_string(vec *, char *);
void construct(v_str *);
void construct_string(vec *);

#endif /* VECTOR_H */
