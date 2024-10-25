#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int char_to_value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'Z') {
        return c - 'A' + 10;
    }
    return -1;
}

char value_to_char(int value) {
    if (value >= 0 && value <= 9) {
        return value + '0';
    } else if (value >= 10 && value < 36) {
        return value - 10 + 'A';
    }
    return '?';
}

char* add_large_numbers(const char *num1, const char *num2, int base) {
    int len1 = strlen(num1);
    int len2 = strlen(num2);
    int max_len = len1 > len2 ? len1 : len2;
    char *result = malloc(max_len + 2);
    if (!result) return NULL;

    int carry = 0, i = 0;
    result[max_len + 1] = '\0';

    for (i = 0; i < max_len; i++) {
        int digit1 = (len1 - 1 - i >= 0) ? char_to_value(num1[len1 - 1 - i]) : 0;
        int digit2 = (len2 - 1 - i >= 0) ? char_to_value(num2[len2 - 1 - i]) : 0;

        int sum = digit1 + digit2 + carry;
        carry = sum / base;
        result[max_len - i] = value_to_char(sum % base);
    }

    if (carry) {
        result[0] = value_to_char(carry);
        return result;
    }

    memmove(result, result + 1, max_len + 1);
    return result;
}

void sum_in_base(int base, int count, ...) {
    va_list args;
    va_start(args, count);

    char *current_sum = strdup(va_arg(args, const char*));
    if (!current_sum) {
        printf("Ошибка при сложении.\n");
        return;
    }

    for (int j = 0; j < strlen(current_sum); j++) {
        if (!isdigit(current_sum[j]) && !(current_sum[j] >= 'A' && current_sum[j] < 'A' + base - 10)) {
            printf("Число \"%s\" недопустимо.\n", current_sum);
            free(current_sum);
            return;
        }
    }

    for (int i = 1; i < count; i++) {
        const char *next_number = va_arg(args, const char*);

        for (int j = 0; j < strlen(next_number); j++) {
            if (!isdigit(next_number[j]) && !(next_number[j] >= 'A' && next_number[j] < 'A' + base - 10)) {
                printf("Число \"%s\" недопустимо.\n", next_number);
                free(current_sum);
                va_end(args);
                return;
            }
        }

        char *new_sum = add_large_numbers(current_sum, next_number, base);
        free(current_sum);
        current_sum = new_sum;
    }

    va_end(args);
    printf("Сумма: %s\n", current_sum);
    free(current_sum);
}

int main() {
    sum_in_base(10, 3, "10", "22", "37");
    sum_in_base(10, 3, "-10", "+22", "=37");
    sum_in_base(10, 3, "999999999999", "1", "123456789012");
    sum_in_base(16, 3, "FFFFFFFFFFFFFFFF", "1", "1");
    sum_in_base(18, 3, "A1B2C3", "123456", "ABCDE");
    sum_in_base(36, 3, "ZZZZZZZZZZZZZZZZZZZZZZZZZ", "1", "ZZ");
    sum_in_base(2, 4, "11111111111111111111111111111111", "1111111111111111111111111111111", "1", "101010");
    return 0;
}
