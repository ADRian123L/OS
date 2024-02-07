#include "execute.h"
#include "read_line.h"
#include "vector.h"

char **PATH;
char  *error_message;

int main(int argc, char *argv[]) {

    error_message = strdup("An error has occurred\n");
    if (argc > 1) {
        printf("%s", error_message);
        fflush(stdout);
        return 1;
    }
    // Vector for storing the paths
    vec path_vec;
    construct_string(&path_vec);
    append_string(&path_vec, strdup("/bin/"));
    append_string(&path_vec, strdup("user/bin/"));
    append_string(&path_vec, NULL);
    PATH = path_vec.string;

    do {
        comnd_strct **arri = get_commands();
        execute_childs(arri);
        fflush(stdout);
        free_memory(arri);
    } while (true);

    return 0;
}
