// Created by: Adrian Lozada

#include "built_in.h"
#include "error_check.h"
#include "execute.h"
#include "read_line.h"
#include "vector.h"

char **PATH; // The path to bin

int main(int argc, char *argv[]) {
    // Throw an error if any arguments where passed to rush
    if (argc > 1) {
        throw_error();
        return 1;
    }
    // Set the path to bin:
    vec v1;
    construct_string(&v1);
    append_string(&v1, strdup("/bin"));
    append_string(&v1, NULL);
    PATH = v1.string;
    // Start the shell:
    bool flag = true;
    do {
        comnd_strct **array = get_commands();
        check_input(&array);
        flag = built_in(&array);
        execute_childs(array);
        fflush(stdout);
        free_memory(array);
    } while (flag);
    free_strings(PATH);
    return 0;
}
