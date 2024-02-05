#include "execute.h"
#include "read_line.h"
#include "vector.h"

char **PATH;
char  *error_message;

int main(int argc, char *argv[]) {

    error_message = strdup("An error has occurred\n");
    // Vector for storing the paths
    vec path_vec    = {15, 0, NULL};
    path_vec.string = (char **) calloc(path_vec.capacity, sizeof(char *));
    append_string(&path_vec, strdup("/bin/"));
    append_string(&path_vec, strdup("user/bin/"));
    append_string(&path_vec, NULL);
    PATH = path_vec.string;

    size_t num_commands;

    do {
        inp **arri = get_commands(&num_commands);
        execute_childs(arri);
        fflush(stdout);
        free_memory(arri);
    } while (true);

    return 0;
}
