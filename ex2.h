#ifndef LAB1_EX2_EX2_H
#define LAB1_EX2_EX2_H

#include <math.h>

// Общие методы поиска предела/ряда/метода дихотомии/простой итерации/произведения
long double calculate_limit(void (*func)(int *, long double *), long double value, long double last_value,
                            long double eps, int first_n);
long double calculate_sequence(long double (*func)(int n), long double value, long double eps, int n);
long double calculate_dichotomy(long double (*func)(long double x), long double a, long double b, long double eps);
long double calculate_simple_iteration(long double (*func)(long double x),
                                       long double (*func_eqvivalent)(long double x, long double lambda), long double a,
                                       long double b, long double lambda, long double eps);
long double calculate_product(long double (*func)(int), long double product, long double last_product, long double eps,
                              int first_k);

// Функции длят вычисления e
long double calculate_e_limit(long double eps);
long double calculate_e_sequence(long double eps);
long double calculate_e_dichotomy(long double eps);
long double fac(int num);
void limit_e(int *n, long double *value);

// Функции для вычисления Pi
long double calculate_pi_limit(long double eps);
long double calculate_pi_sequence(long double eps);
long double calculate_pi_simple_iteration(long double eps);
void lim_pi(int *n, long double *value);
long double seq_pi(int n);
long double eqv_simple_it_pi(long double x, long double lambda);
long double simple_it_pi(long double x);

// Функции для вычисления ln2
long double calculate_ln2_limit(long double eps);
long double calculate_ln2_sequence(long double eps);
long double calculate_ln2_dech(long double eps);
long double func_ln2_dech(long double x);
void ln2_limit(int *n, long double *value);

// Функции для вычисления sqrt2
long double calculate_sqrt2_limit(long double eps);
long double calculate_sqrt2_product(long double eps);
long double calculate_sqrt2_dech(long double eps);
void sqrt2_limit(int *n, long double *value);
long double calculate_sqrt2_product(long double eps);

long double CharToDouble(char *string);

// Фунции для вычисления гамма
long double calculate_gamma_limit(long double eps);
long double calculate_gamma_seq(long double eps);
#endif