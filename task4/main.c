#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <N>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int N = atoi(argv[2]);

    if (N < 0) {
        printf("N must be non-negative!\n");
        return 1;
    }

    if (N == 0) {
        N = -1;
    }

    char c;
    int lines = 0;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            lines++;
        }
        if (lines == N) {
            fgetc(stdin);
            lines = 0;
            continue;
        }
        fputc(c, stdout);
    }

    return fclose(file);
}