#include <stdio.h>
#include <dirent.h>

int print_directory_content(char *dir_name);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    int e;
    e = print_directory_content(".");
    if (e != 0) {
        return e;
    }
    e = print_directory_content(argv[1]);
    if (e != 0) {
        return e;
    }

    return 0;
}

int print_directory_content(char *dir_name) {
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        printf("Error opening directory '%s'!\n", dir_name);
        return 1;
    }

    printf("The content of the directory %s:\n", dir_name);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s/%s\n", dir_name, entry->d_name);
    }
    printf("\n");

    return closedir(dir);
}