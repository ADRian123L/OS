#include "built_in.h"
#include "error_check.h"
#include "execute.h"
#include "read_line.h"
#include "vector.h"

char **PATH;

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

        for (comnd_strct **ptr = array; *ptr != NULL; ++ptr) {
            for (char **ptr1 = (*ptr)->commands; *ptr1 != NULL; ++ptr1) {
                printf("COMMAND [%s]\n", *ptr1);
            }
            printf("The [%ld] command\n", (ptr - array));
        }

        printf("\n AFTER check_input \n\n");

        check_input(&array);

        for (comnd_strct **ptr = array; *ptr != NULL; ++ptr) {
            for (char **ptr1 = (*ptr)->commands; *ptr1 != NULL; ++ptr1) {
                printf("COMMAND2 [%s]\n", *ptr1);
            }
            printf("The [%ld] command\n", (ptr - array));
        }

        flag = built_in(&array);
        execute_childs(array);
        fflush(stdout);
        free_memory(array);
    } while (flag);

    return 0;
}
