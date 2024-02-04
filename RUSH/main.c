#include "read_line.h"

int main(int argc, char *argv[]) {
    inp **arri = get_commands();
    for (inp **ptr = arri; *ptr != NULL; ++ptr) {
        printf("[INPUT]: %s\n", (*ptr)->command);
        for (char **ptr_char = (*ptr)->arguments; *ptr_char != NULL;
             ++ptr_char) {
            printf("[ARGUMENT %lld]: %s\n",
                   (long long int) (ptr_char - (*ptr)->arguments),
                   *ptr_char);
        }
        printf("[Redi] %d\n", (*ptr)->redirection);
        for (char **ptr_char = (*ptr)->redi_argu; *ptr_char != NULL;
             ++ptr_char) {
            printf("[Redirection ARGUMENT %lld]: %s\n",
                   (long long int) (ptr_char - (*ptr)->redi_argu),
                   *ptr_char);
        }
        write(STDOUT_FILENO, "\n", strlen("\n"));
    }
    return 0;
}
