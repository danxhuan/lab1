#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void simplify_fraction(int *numerator, int *denominator) {
    int divisor = gcd(*numerator, *denominator);
    *numerator /= divisor;
    *denominator /= divisor;
}

int* get_prime_factors(int base, int *count) {
    int *factors = malloc(10 * sizeof(int));
    if (!factors) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        return NULL;
    }

    *count = 0;

    for (int i = 2; i <= base; i++) {
        while (base % i == 0) {
            factors[(*count)++] = i;
            base /= i;
        }
    }
    return factors;
}

int has_finite_representation(double num, int base) {
    if (num <= 0 || num >= 1) {
        return 0;
    }

    const double epsilon = 0.00001;
    int denominator = 1;

    while (num - floor(num) > epsilon) {
        num *= 10;
        denominator *= 10;
    }

    int numerator = (int)(num + 0.5);

    simplify_fraction(&numerator, &denominator);

    int factor_count;
    int *allowed_factors = get_prime_factors(base, &factor_count);
    if (!allowed_factors) {
        return 0;
    }

    for (int i = 0; i < factor_count; i++) {
        while (denominator % allowed_factors[i] == 0) {
            denominator /= allowed_factors[i];
        }
    }

    free(allowed_factors);

    return (denominator == 1);
}

void check_finite_representations(int base, int count, ...) {
    if (base < 2 || base > 36) {
        printf("Основание %d должно быть от 2 до 36.\n", base);
        return;
    }

    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        double num = va_arg(args, double);
        if (num <= 0 || num >= 1) {
            printf("%f не в диапазоне (0, 1).\n", num);
            continue;
        }

        if (has_finite_representation(num, base)) {
            printf("%f имеет конечное представление в %d сс.\n", num, base);
        } else {
            printf("%f не имеет конечного представления в %d сс.\n", num, base);
        }
    }

    va_end(args);
}

int main() {
    check_finite_representations(10, 4, 0.5, 0.25, 0.1, 0.3333);
    check_finite_representations(2, 4, 0.5, 0.75, 0.1, 0.25);
    check_finite_representations(8, 4, 0.0625, 0.125, 0.3, 0.5);
    check_finite_representations(36, 4, 0.0625, 0.1, 0.3, 0.5);
    check_finite_representations(2, 4, 625.0, -0.1, 0.3, 0.5);
    check_finite_representations(1, 4, 625.0, 0.111, 0.673, 0.587);

    return 0;
}
