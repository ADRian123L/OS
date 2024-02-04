#include "read_line.h"

int main(int argc, char *argv[]) {
    inp **arri = get_commands();
    printf("[INPUT]: %s\n", arri[0]->command);
    return 0;
}
