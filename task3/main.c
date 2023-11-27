#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    printf("To end input press Enter, Ctrl+D (EOF)\n");

    int c;
    while ((c = fgetc(stdin)) != EOF) {
        fputc(c, file);
    }

    return fclose(file);
}