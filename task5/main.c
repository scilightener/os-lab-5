#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <sourcefile> <targetfile>\n", argv[0]);
        return 1;
    }

    FILE *source = fopen(argv[1], "r");
    if (source == NULL) {
        printf("Error opening source file!\n");
        return 1;
    }

    FILE *target = fopen(argv[2], "w");
    if (target == NULL) {
        printf("Error opening target file!\n");
        fclose(source);
        return 1;
    }

    char c;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, target);
    }

    int e = fclose(source);
    if (e != 0) {
        return e;
    }

    e = fclose(target);
    if (e != 0) {
        return e;
    }

    struct stat st;
    if (stat(argv[1], &st) != 0 || chmod(argv[2], st.st_mode) != 0) {
        printf("Error while working with file permissions!\n");
        return 1;
    }

    return 0;
}