#include <stdio.h>
#include <stdlib.h>

int isDigit(char digit) {
    return (digit >= '0' && digit <= '9');
}

int isAlpha(char c) {
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

int isAlnum(char c) {
    return isDigit(c) || isAlpha(c);
}

size_t strLen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char toLower(char litera) {
    if (litera >= 'A' && litera <= 'Z') {
        return litera + 32;
    }
    return litera;
}

int minBase(const char *num) {
    int min_base = 2;
    for (size_t i = 0; i < strLen(num); i++) {
        char symbol = toLower(num[i]);
        int value;

        if (isDigit(symbol)) {
            value = symbol - '0';
        } else if (symbol >= 'a' && symbol <= 'z') {
            value = symbol - 'a' + 10;
        } else {
            continue;
        }

        if (value + 1 > min_base) {
            min_base = value + 1;
        }
    }
    return (min_base <= 36) ? min_base : -1;
}

long long toDecimal(const char* num, int base) {
    long long decNumber = 0;
    int isNegative = 0;

    size_t startIndex = 0;

    if (num[0] == '-') {
        isNegative = 1;
        startIndex = 1;
    }

    for (size_t i = startIndex; i < strLen(num); i++) {
        char symbol = toLower(num[i]);
        int value;

        if (isDigit(symbol)) {
            value = symbol - '0';
        } else if (symbol >= 'a' && symbol <= 'z') {
            value = symbol - 'a' + 10;
        } else {
            continue;
        }

        decNumber = decNumber * base + value;
    }

    return isNegative ? -decNumber : decNumber;
}

long fileSize(FILE *file) {
    char buffer[1024];
    long size = 0;
    size_t bytesRead = 0;

    while ((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), file)) > 0) {
        size += bytesRead;
    }

    rewind(file);
    return size;
}

const char* removeLeadingZeros(const char *num) {
    while (*num == '0' && *(num + 1) != '\0') {
        num++;
    }
    return num;
}

int isZero(const char* num) {
    const char* cleanedToken = removeLeadingZeros(num);
    return (strLen(cleanedToken) == 1 && cleanedToken[0] == '0');
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "Can't open input file\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        fprintf(stderr, "Can't open output file\n");
        fclose(inputFile);
        return 1;
    }

    long file_size = fileSize(inputFile);
    char *buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    fread(buffer, sizeof(char), file_size, inputFile);
    buffer[file_size] = '\0';

    char *token = NULL;
    size_t tokenLen = 0;
    size_t token_capacity = 32;

    token = (char *)malloc(token_capacity);
    if (!token) {
        fprintf(stderr, "Memory allocation for token failed\n");
        free(buffer);
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    for (size_t i = 0; i < file_size; i++) {
        char symbol = buffer[i];

        if (!isAlnum(symbol) && symbol != '-') {
            if (tokenLen > 0) {
                token[tokenLen] = '\0';

                const char* cleanedToken = removeLeadingZeros(token);

                // если 0 или -0
                if (isZero(cleanedToken) || (token[0] == '-' && isZero(token + 1))) {
                    fprintf(outputFile, "0\n");
                } else {
                    int min_base = minBase(cleanedToken);
                    if (min_base != -1) {
                        long long decNumber = toDecimal(cleanedToken, min_base);
                        fprintf(outputFile, "%s %d %lld\n", cleanedToken, min_base, decNumber);
                    }
                }

                tokenLen = 0;
            }
        } else {
            if (symbol == '-' && (i + 1 >= file_size || !isAlnum(buffer[i + 1]))) {
                continue;
            }
            if (tokenLen >= token_capacity) {
                token_capacity *= 2;
                token = (char *)realloc(token, token_capacity);
                if (!token) {
                    fprintf(stderr, "Memory reallocation for token failed\n");
                    free(buffer);
                    fclose(inputFile);
                    fclose(outputFile);
                    return 1;
                }
            }
            token[tokenLen++] = symbol;
        }
    }

    if (tokenLen > 0) {
        token[tokenLen] = '\0';
        const char* cleanedToken = removeLeadingZeros(token);

        if (isZero(cleanedToken) || (token[0] == '-' && isZero(token + 1))) {
            fprintf(outputFile, "0\n");
        } else {
            int min_base = minBase(cleanedToken);
            if (min_base != -1) {
                long long decNumber = toDecimal(cleanedToken, min_base);
                fprintf(outputFile, "%s %d %lld\n", cleanedToken, min_base, decNumber);
            }
        }
    }

    free(buffer);
    free(token);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
