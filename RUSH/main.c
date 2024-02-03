#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROMPT strdup("rush> \0")

int main(int argc, char *argv[]) {
    char  *input = NULL;
    size_t size  = 0;
    while (true) {
        fprintf(stdout, "%s", PROMPT);
        getline(&input, &size, stdin);
        write(STDIN_FILENO, input, strlen(input));
        fflush(STDIN_FILENO);
    }
    return 0;
}
