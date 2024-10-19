#include "ex2.h"

#include <math.h>
#include <stdio.h>

long double fac(int num) {
	long double f = 1;
	for (int i = 2; i <= num; ++i) {
		f *= i;
	}
	return f;
}

long double CharToDouble(char *string) {
	int k = -1;
	int fl = 0;
	fl = 0;
	long double number = 0;
	for (int j = 0; string[j] != '\0'; ++j) {
		// printf("%c\n", string[j]);
		if (string[j] == '-')
			fl = 1;
		else if (string[j] >= '0' && string[j] <= '9') {
			number *= 10;
			number += (string[j] - '0');
			if (k != -1) k += 1;
		} else if (string[j] == '.')
			k = 0;
		else {
			return -1;
		}
	}
	for (int k_null = 0; k_null < k; ++k_null) number /= 10.0;
	k = -1;
	if (fl) number *= -1;
	// putchar('\n');
	// printf("%Lf\n", number);
	return number;
}

// Общий метод поиска предeла
long double calculate_limit(void (*func)(int *, long double *), long double value, long double last_value,
                            long double eps, int first_n) {
	int i = first_n;
	do {
		last_value = value;
		// Ищем предел для конкретного значения
		func(&i, &value);
		// printf("value: %Lf last value: %Lf\n", value, last_value);
	} while (fabsl(value - last_value) >= eps);
	return value;
}

// Общий метод вычисления ряда
long double calculate_sequence(long double (*func)(int n), long double value, long double eps, int n) {
	long double last_value = 1.0;
	long double precision = 1.0;
	while (fabsl(last_value - value) >= eps) {
		precision = func(n);
		// printf("%Lf %Lf\n", value, last_value);
		last_value = value;
		value += precision;
		n += 1;
	}
	return value;
}

// Общий метод дихотомии
long double calculate_dichotomy(long double (*func)(long double x), long double a, long double b, long double eps) {
	long double c = (a + b) / 2;
	while (fabsl(func(c)) >= eps) {
		if (func(c) > 0) {
			b = c;
		} else {
			a = c;
		}
		c = (a + b) / 2;
		// printf("%Lf %Lf %Lf\n", c);
	}
	return c;
}

// Общий метод итерации
long double calculate_simple_iteration(long double (*func)(long double x),
                                       long double (*func_eqvivalent)(long double x, long double lambda), long double a,
                                       long double b, long double lambda, long double eps) {
	long double last_value = (a + b) / 2;
	long double value = func_eqvivalent(last_value, lambda);
	while (fabsl(value - last_value) >= eps / 10000) {
		last_value = value;
		value = func_eqvivalent(last_value, lambda);
		// printf("%Lf %Lf\n", last_value, value);
	}
	return value;
}

// Общий метод произведения
long double calculate_product(long double (*func)(int), long double product, long double last_product, long double eps,
                              int first_k) {
	int k = first_k;

	while (fabsl(product - last_product) >= eps) {
		last_product = product;
		product = func(last_product);
		k += 1;
	}
	return product;
}

// Вычисление предела e
void limit_e(int *n, long double *value) {
	*value = (1 + 1.0 / *n);
	for (int j = 2; j <= *n; ++j) {
		*value *= (1 + 1.0 / *n);
	}
	*n *= 2;
}

long double calculate_e_limit(long double eps) { return calculate_limit(limit_e, 2, 2, eps, 2); }

// Вычисление значения функции ln(x) = 1
long double ln1(long double x) { return log(x) - 1; }

long double calculate_e_dichotomy(long double eps) { return calculate_dichotomy(ln1, 1, 4, eps); }

long double e_sequence(int n) { return 1.0 / fac(n); }

long double calculate_e_sequence(long double eps) { return calculate_sequence(e_sequence, 0, eps, 0); }

void to_simple_numbers(long int *a, long int *b) {
	for (int i = 2; i <= *a; ++i) {
		if (*a % i == 0 && *b % i == 0) {
			*a /= i;
			*b /= i;
			i -= 1;
		}
	}
}

void lim_pi(int *n, long double *value) {
	// printf("до: %Lf %d\n", *value);
	*value *= 4.0 * (*n) / ((2 * (*n) - 1) * (2 * (*n) - 1));
	// printf("после: %Lf\n", *value);

	*value *= (long double)(*n - 1);

	// printf("%d %d %Lf\n", 16 * (*n)* (*n)* (*n), (2 * (*n) * (2*(*n) - 1) * 2 * (*n) * (2*(*n) - 1)), *value);
	*n += 1;
}

long double calculate_pi_limit(long double eps) { return calculate_limit(lim_pi, 4, 0, eps, 2); }

long double seq_pi(int n) { return (4.0 * (((n - 1) % 2 == 0) ? 1.0 : -1.0) / (2.0 * n - 1.0)); }

long double calculate_pi_sequence(long double eps) { return calculate_sequence(seq_pi, 0, eps, 1); }

long double eqv_simple_it_pi(long double x, long double lambda) { return x + lambda * (cos(x) + 1); }

long double simple_it_pi(long double x) { return cos(x) + 1; }

long double calculate_pi_simple_iteration(long double eps) {
	return calculate_simple_iteration(simple_it_pi, eqv_simple_it_pi, 1, 5, 0.5, eps);
}

void ln2_limit(int *n, long double *value) {
	*value = *n * (pow(2, 1.0 / *n) - 1);
	*n *= 5;
}

long double calculate_ln2_limit(long double eps) { return calculate_limit(ln2_limit, 0, 0, eps, 1); }

long double ln2_seq(int n) { return (((n - 1) % 2 == 0) ? 1.0 : -1.0) / (n); }

long double calculate_ln2_sequence(long double eps) { return calculate_sequence(ln2_seq, 0, eps, 1); }

long double func_ln2_dech(long double x) { return pow(calculate_e_dichotomy(0.0001), x) - 2; }

long double calculate_ln2_dech(long double eps) { return calculate_dichotomy(func_ln2_dech, 0, 2, eps); }

void sqrt2_limit(int *n, long double *value) {
	*value = -0.5;

	for (int i = 1; i <= *n; ++i) {
		*value = *value - *value * *value / 2.0 + 1.0;
	}
	*n += 1;
}

long double calculate_sqrt2_limit(long double eps) { return calculate_limit(sqrt2_limit, 0, -0.5, eps, 1); }

long double sqrt2_product(int k) { return pow(2, pow(2, -k)); }

long double calculate_sqrt2_product(long double eps) { return calculate_product(sqrt2_product, 0, -1, eps, 2); }

long double sqrt2_dech(long double x) { return x * x - 2; }

long double calculate_sqrt2_dech(long double eps) { return calculate_dichotomy(sqrt2_dech, 0, 4, eps); }

void gamma_limit(int *n, long double *value) {
	*value = 0;
	// printf("%Lf\n", *value);
	for (int i = 1; i <= *n; ++i) {
		*value += log(fac(i)) * ((i % 2 == 0) ? 1 : -1) / i * fac(*n) / fac(i) / fac(*n - i);
		// printf("%Lf\n", log(fac(i)) * ((i % 2 == 0) ? 1 : -1) / i * fac(*n) / fac(i) / fac(*n - i));
	}

	*n += 2;
}

long double calculate_gamma_limit(long double eps) { return calculate_limit(gamma_limit, 2, 1, eps, 2); }

long double gamma_seq(int n) {
	// printf("%Lf\n", 1.0 / ((floor(sqrt(n))) * floor(sqrt(n))) - 1.0 / (long double)n);
	return 1.0 / ((floor(sqrt(n))) * floor(sqrt(n))) - 1.0 / (long double)n;
}

long double calculate_gamma_seq(long double eps) {
	return calculate_sequence(gamma_seq, -calculate_pi_sequence(eps) * calculate_pi_sequence(eps) / 6, eps, 2);
}
