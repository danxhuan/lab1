#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef enum {
    SUCCESS,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INVALID_ARGUMENTS
} StatusCode;

int validate_substring(const char *substring) {
    for (int i = 0; substring[i] != '\0'; i++) {
        if (substring[i] == '\\' && substring[i + 1] == '0') {
            printf("Error: недопустимая последовательность '\\0'\n");
            return 0;
        }
    }
    return 1;
}

void computeLPSArray(const char *substring, int M, int *lps) {
    int length = 0;
    lps[0] = 0;
    int i = 1;

    while (i < M) {
        if (substring[i] == substring[length]) {
            length++;
            lps[i++] = length;
        } else if (length != 0) {
            length = lps[length - 1];
        } else {
            lps[i++] = 0;
        }
    }
}

void KMPSearch(const char *substring, FILE *file, const char *filename) {
    int M = strlen(substring);
    int lps[M];
    computeLPSArray(substring, M, lps);

    int i = 0;
    int j = 0;
    int line_number = 1;
    char buffer[1024];
    int found = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        int len = strlen(buffer);
        for (int k = 0; k < len; k++) {
            if (substring[j] == buffer[k]) {
                j++;
            } else if (j != 0) {
                j = lps[j - 1];
            }

            if (j == M) {
                printf("in %s line %d pos %d\n", filename, line_number, k - M + 2);
                found = 1; //
                j = lps[j - 1]; // дальше ищем
            }

            if (buffer[k] == '\n') {
                line_number++;
            }
        }
    }

    if (!found) {
        printf("Nothing in %s\n", filename);
    }
}

void search_in_file(const char *substring, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror(filename);
        return;
    }

    KMPSearch(substring, file, filename);
    fclose(file);
}

void search_in_files(const char *substring, int file_count, ...) {
    va_list files;
    va_start(files, file_count);

    for (int i = 0; i < file_count; i++) {
        const char *filename = va_arg(files, const char *);
        search_in_file(substring, filename);
    }

    va_end(files);
}

int main() {
    const char *substring = "123\\0 123";
    if (validate_substring(substring)) {
        search_in_files(substring, 4, "4.txt", "3.txt", "2.txt", "1.txt");
    }
    return 0;
}
