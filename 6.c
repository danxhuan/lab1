#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
#include <ctype.h>

typedef enum {
    SUCCESS = 0,
    OVERFLOW_ERROR = 1,
    NUMERAL_SYSTEM_ERROR = 2,
    INPUT_FILE_ERROR = 3
} ErrorCode;

int RomanTOInt(const char *s, int *res);
int ZeckendorfTOUInt(const char *s, unsigned int *res);
int sequence_number(char c);
int FromXTo10(const char *s, int base, int *result);
int overfscanf(FILE *stream, const char *format, ...);
int oversscanf(const char *buffer, const char *format, ...);

int RomanTOInt(const char *s, int *res) {
    int map[256] = {0};
    map['I'] = 1; map['V'] = 5; map['X'] = 10;
    map['L'] = 50; map['C'] = 100; map['D'] = 500; map['M'] = 1000;
    *res = 0;

    for (int i = 0; s[i]; ++i) {
        if (map[s[i]] < map[s[i + 1]]) {
            if ((*res - map[s[i]]) < INT_MIN) return OVERFLOW_ERROR;
            *res -= map[s[i]];
        } else {
            if ((*res + map[s[i]]) > INT_MAX) return OVERFLOW_ERROR;
            *res += map[s[i]];
        }
    }
    return SUCCESS;
}

int ZeckendorfTOUInt(const char *s, unsigned int *res) {
    unsigned int fib[33] = {1, 1};
    *res = 0;

    for (int i = 2; i < 33; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    for (int i = 0; s[i]; ++i) {
        if (s[i] == '1') {
            if ((*res + fib[i]) > INT_MAX) return OVERFLOW_ERROR;
            *res += fib[i];
        }
    }
    return SUCCESS;
}

int sequence_number(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

int FromXTo10(const char *s, int base, int *result) {
    *result = 0;
    int is_negative = 0;

    for (int i = 0; s[i]; ++i) {
        if (s[i] == '-') is_negative = 1;
        else {
            int seq_num = sequence_number(s[i]);
            if (seq_num == -1 || seq_num >= base) return NUMERAL_SYSTEM_ERROR;
            *result = *result * base + seq_num;
        }
    }
    if (is_negative) *result *= -1;
    return SUCCESS;
}

int overfscanf(FILE *stream, const char *format, ...) {
    if (stream == NULL) return INPUT_FILE_ERROR;

    va_list args;
    va_start(args, format);
    int count = 0;
    char *p = (char *)format;

    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == 'R') {
                int *res = va_arg(args, int *);
                char input_str[100] = {0};
                if (fscanf(stream, "%s", input_str) != 1) {
                    return INPUT_FILE_ERROR;
                }
                if (RomanTOInt(input_str, res) == SUCCESS) count++;
            } else if (*p == 'Z') {
                unsigned int *res = va_arg(args, unsigned int *);
                char input_str[50] = {0};
                if (fscanf(stream, "%s", input_str) != 1) {
                    return INPUT_FILE_ERROR;
                }
                if (ZeckendorfTOUInt(input_str, res) == SUCCESS) count++;
            } else if (*p == 'C' || *p == 'v') {
                int *res = va_arg(args, int *);
                int base = va_arg(args, int);
                char str[100] = {0};
                if (fscanf(stream, "%s", str) != 1) {
                    return INPUT_FILE_ERROR;
                }
                if (FromXTo10(str, base, res) == SUCCESS) count++;
            } else if (*p == 'C' || *p == 'V') {
                int *res = va_arg(args, int *);
                int base = va_arg(args, int);
                char str[100] = {0};
                if (fscanf(stream, "%s", str) != 1) {
                    return INPUT_FILE_ERROR;
                }
                for (int i = 0; str[i]; i++) {
                    str[i] = toupper(str[i]);
                }
                if (FromXTo10(str, base, res) == SUCCESS) count++;
            }
            p++;
        } else {
            fgetc(stream);
            p++;
        }
    }

    va_end(args);
    return count;
}

int oversscanf(const char *buffer, const char *format, ...) {
    int count = 0;
    va_list args;
    va_start(args, format);
    const char *p = format;

    while (*p) {
        if (*p == '%') {
            p++;
            if (*p == 'R') {
                int *res = va_arg(args, int *);
                char input_str[100] = {0};
                sscanf(buffer, "%s", input_str);
                if (RomanTOInt(input_str, res) == SUCCESS) count++;
            } else if (*p == 'Z') {
                unsigned int *res = va_arg(args, unsigned int *);
                char input_str[50] = {0};
                sscanf(buffer, "%s", input_str);
                if (ZeckendorfTOUInt(input_str, res) == SUCCESS) count++;
            } else if (*p == 'C' || *p == 'v') {
                int *res = va_arg(args, int *);
                int base = va_arg(args, int);
                char str[100] = {0};
                sscanf(buffer, "%s", str);
                if (FromXTo10(str, base, res) == SUCCESS) count++;
            } else if (*p == 'C' || *p == 'V') {
                int *res = va_arg(args, int *);
                int base = va_arg(args, int);
                char str[100] = {0};
                sscanf(buffer, "%s", str);
                for (int i = 0; str[i]; i++) {
                    str[i] = toupper(str[i]);
                }
                if (FromXTo10(str, base, res) == SUCCESS) count++;
            }
            p++;
        } else {
            buffer++;
            p++;
        }
    }
    va_end(args);
    return count;
}

void test_overfscanf() {
    FILE *file;
    int res_int;
    unsigned int res_uint;
    int count;

    file = fopen("input.txt", "w");
    fprintf(file, "Roman: MCMLXXXIV\nZr: 010001\nBase 16: ff\n");
    fclose(file);

    file = fopen("input.txt", "r");
    count = overfscanf(file, "Roman: %R", &res_int);
    printf("overfscanf Roman: count = %d, result = %d\n", count, res_int);

    count = overfscanf(file, "Zr: %Z", &res_uint);
    printf("overfscanf Zeckendorf: count = %d, result = %u\n", count, res_uint);

    count = overfscanf(file, "Base 16: %C 16", &res_int);
    printf("overfscanf Base 16: count = %d, result = %d\n", count, res_int);

    fclose(file);
}

void test_oversscanf() {
    char input[] = "Roman: MCMLXXXIV\nZr: 010001\nBase 16: ff\n";
    int res_int;
    unsigned int res_uint;
    int count;

    count = oversscanf(input, "Roman: %R", &res_int);
    printf("oversscanf Roman: count = %d, result = %d\n", count, res_int);

    count = oversscanf(input, "Zr: %Z", &res_uint);
    printf("oversscanf Zeckendorf: count = %d, result = %u\n", count, res_uint);

    count = oversscanf(input, "Base 16: %C 16", &res_int);
    printf("oversscanf Base 16: count = %d, result = %d\n", count, res_int);
}

int main() {
    test_overfscanf();
    test_oversscanf();
    return 0;
}
