#include "vector.h"

// The function appends a new string to the array.
void append_string(vec *vect, char *str) {
    if (vect->size + 1 >
        vect->capacity) { // If the size not large enough, allocate space.
        vect->string   = (char **) realloc(vect->string,
                                         sizeof(char *) * vect->capacity * 2);
        vect->capacity = vect->size * 2;
    }
    vect->string[vect->size] = str;
    vect->size++;
}

// The function appends a new structure.
void append(v_str *vec, struct inputs *input) {
    if (vec->size + 1 > vec->capacity) {
        vec->inputs_var = (struct inputs **) realloc(
            vec->inputs_var, sizeof(struct inputs *) * vec->capacity * 2);
        vec->capacity = vec->size * 2;
    }
    vec->inputs_var[vec->size] = input;
    vec->size++;
}

void construct(v_str *object) {
    object->capacity = 15;
    object->size     = 0;
    object->inputs_var =
        (struct inputs **) calloc(object->capacity, sizeof(struct inputs *));
}

void construct_string(vec *object) {
    object->capacity = 15;
    object->size     = 0;
    object->string   = (char **) calloc(object->capacity, sizeof(char *));
}
