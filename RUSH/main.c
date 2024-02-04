#include "read_line.h"

int main(int argc, char *argv[]) {
    inp  **arri = NULL;
    size_t num_commands;
    do {
        inp **arri = get_commands(&num_commands);

        free_memory(arri);
    } while (true);
    return 0;
}
