#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <linux/limits.h>

int search_file(char *directory, char *file, int *viewed_files_cnt);
int ends_with(const char *str, const char *suffix);
void print_readable_date(time_t creation_time);
void print_readable_permissions(mode_t mode);
void print_absolute_path(const char *file_path);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <directory> <file>\n", argv[0]);
        return 1;
    }

    int viewed_files_cnt = 0;
    int res = search_file(argv[1], argv[2], &viewed_files_cnt);
    if (res != 0) {
        printf("File %s not found\n", argv[2]);
    }

    printf("Viewed %d files\n", viewed_files_cnt);
    return 0;
}

int search_file(char *directory, char *file, int *viewed_files_cnt) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        printf("Cannot open directory %s\n", directory);
        return 1;
    }

    char path[256];
    struct dirent *entry;
    struct stat file_stat;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char path[257];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        if (stat(path, &file_stat) != 0) {
            printf("Cannot get stat for file %s\n", entry->d_name);
            continue;
        }

        (*viewed_files_cnt)++;

        if (S_ISREG(file_stat.st_mode)) {
            if (ends_with(path, file)) {
                printf("File %s found in directory %s\n", file, directory);
                print_absolute_path(path);
                printf("File size: %lld bytes\n", (long long) file_stat.st_size);
                print_readable_date(file_stat.st_ctime);
                print_readable_permissions(file_stat.st_mode);
                printf("File inode: %lld\n", (long long) file_stat.st_ino);

                closedir(dir);
                return 0;
            }
        } else if (S_ISDIR(file_stat.st_mode)) {
            int res = search_file(path, file, viewed_files_cnt);
            if (res == 0) {
                closedir(dir);
                return 0;
            }
        }
    }

    closedir(dir);
    return 1;
}

int ends_with(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix > lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

void print_readable_date(time_t creation_time) {
    char buffer[30];
    struct tm *tm_info;

    tm_info = localtime(&creation_time);

    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("File date: %s\n", buffer);
}

void print_readable_permissions(mode_t mode) {
    char permissions[10];

    permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (mode & S_IXOTH) ? 'x' : '-';
    permissions[9] = '\0';

    printf("File permissions: %s\n", permissions);
}

void print_absolute_path(const char *file_path) {
    char absolute_path[PATH_MAX];
    if (realpath(file_path, absolute_path) != NULL) {
        printf("File path: %s\n", absolute_path);
    } else {
        printf("Error resolving absolute path for %s\n", file_path);
    }
}