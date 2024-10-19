#include <stdio.h>

#include "ex2.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Количество аргументов командной строки неверно\n");
		return 1;
	}
	double eps = CharToDouble(argv[1]);
	if (eps < 0) {
		printf("Точность записана неверно\n");
		return 1;
	}

	// 27182818284 5904523536 0287471352 6624977572 4709369995
	printf("limit: %.15Lf\n", calculate_e_limit(eps));
	printf("sequence: %.15Lf\n", calculate_e_sequence(eps));
	printf("dech: %.15Lf\n\n", calculate_e_dichotomy(eps));

	// 3.141592653589793
	printf("limit: %.15Lf\n", calculate_pi_limit(eps));
	printf("sequence: %.15Lf\n", calculate_pi_sequence(eps));
	printf("simple_iteration: %.15Lf\n\n", calculate_pi_simple_iteration(eps));

	// 0.6931471805599453
	printf("limit: %.15Lf\n", calculate_ln2_limit(eps));
	printf("sequence: %.15Lf\n", calculate_ln2_sequence(eps));
	printf("dech: %.15Lf\n\n", calculate_ln2_dech(eps));

	// 1.4142135623730951
	printf("limit: %.15Lf\n", calculate_sqrt2_limit(eps));
	printf("product: %.15Lf\n", calculate_sqrt2_product(eps));
	printf("dech: %.15Lf\n\n", calculate_sqrt2_dech(eps));

	// printf("limit: %.15Lf\n", calculate_gamma_limit(eps));
	// printf("seq: %.15Lf\n", calculate_gamma_seq(eps));

	return 0;
}