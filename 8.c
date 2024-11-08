#include "8.h"

error_msg create_node(Node** node, int data) {
	*node = (Node*)calloc(1, sizeof(Node));
	if (!*node) {
		return MEMORY_ALLOCATED_ERROR;
	}
	(*node)->data = data;
	return SUCCESS;
}

void add_node(Node* prev, Node* cur) {
	cur->next = prev->next;
	prev->next = cur;
}

void delete_node(Node* prev, Node* cur) {
	prev->next = cur->next;
	cur->data = 0;
	cur->next = NULL;
	free(cur);
}
void destroy_LinkedList(Node* head) {
	Node* tmp;
	while (head) {
		tmp = head;
		head = head->next;
		tmp->data = 0;
		tmp->next = NULL;
		free(tmp);
	}
}

Node* find_node(Node* head, int data) {
	while (head) {
		if (head->data == data) {
			return head;
		}
		head = head->next;
	}
	return NULL;
}

error_msg push_node_end(Node** head, int data) {
	Node* tmp = *head;
	error_msg errorMsg;
	if (!tmp) {
		errorMsg = create_node(head, data);
		if (errorMsg) {
			return errorMsg;
		}
	} else {
		while (tmp->next) tmp = tmp->next;
		errorMsg = create_node(&(tmp->next), data);
		if (errorMsg) {
			return errorMsg;
		}
	}
	return SUCCESS;
}

error_msg push_node_start(Node** head, int data) {
	Node* t;
	error_msg errorMsg = create_node(&t, data);
	if (errorMsg) {
		return errorMsg;
	}
	t->next = (*head);
	*head = t;
	return SUCCESS;
}

void print_LinkedList(FILE* stream, Node* head, char* separator) {
	while (head) {
		fprintf(stream, "%d%s", head->data, separator);
		head = head->next;
	}
}

error_msg create_polynomial(Polynomial** polynomial) {
	*polynomial = (Polynomial*)calloc(1, sizeof(Polynomial));
	if (!*polynomial) {
		return MEMORY_ALLOCATED_ERROR;
	}
	(*polynomial)->degree = 0;
	return SUCCESS;
}

void destroy_polynomial(Polynomial** polynomial) {
	destroy_LinkedList((*polynomial)->coefficients);
	(*polynomial)->degree = 0;
	(*polynomial)->coefficients = NULL;
	free((*polynomial));
	(*polynomial) = NULL;
}

error_msg read_polynomial_from_string(Polynomial** pl, String* input_string) {
	Polynomial* polynomial = *pl;
	error_msg errorMsg = create_polynomial(&polynomial);
	if (errorMsg) {
		return print_error(errorMsg);
	}
	String string;
	errorMsg = create_string(&string, "");
	if (errorMsg) {
		destroy_polynomial(&polynomial);
		return errorMsg;
	}
	for (int i = 0; i < input_string->size; ++i) {
		if (input_string->arr[i] != ' ') {
			errorMsg = push_end_string(&string, input_string->arr[i]);
			if (errorMsg) {
				destroy_polynomial(&polynomial);
				destroy_string(&string);
				return errorMsg;
			}
		}
		if (input_string->arr[i] != ' ' && input_string->arr[i] != 'x' && input_string->arr[i] != '+' &&
		    input_string->arr[i] != '-' && input_string->arr[i] != '^' &&
		    (input_string->arr[i] < '0' || input_string->arr[i] > '9')) {
			destroy_string(&string);
			destroy_polynomial(&polynomial);
			return INCORRECT_OPTIONS_ERROR;
		}
	}
	int sign = 1;
	int degree = 0;
	int coefficient = 0;
	int has_coefficient = 0;
	for (int i = 0; i < string.size;) {
		if (string.arr[i] == '+' || string.arr[i] == '-') {
			if (string.arr[i] == '+')
				sign = 1;
			else
				sign = -1;
			++i;
		}

		while (string.arr[i] >= '0' && string.arr[i] <= '9') {
			coefficient = coefficient * 10 + (string.arr[i] - '0');
			i++;
			has_coefficient = 1;
		}

		if (!has_coefficient) {
			coefficient = 1;
		}
		coefficient *= sign;
		if (string.arr[i] == 'x') {
			i++;
			if (string.arr[i] != '^' && string.arr[i] != '+' && string.arr[i] != '-' && string.arr[i] != '\0') {
				destroy_string(&string);
				destroy_polynomial(&polynomial);
				return INCORRECT_OPTIONS_ERROR;
			}
			if (string.arr[i] == '^') {
				i++;
				if (!(string.arr[i] >= '0' && string.arr[i] <= '9')) {
					destroy_string(&string);
					destroy_polynomial(&polynomial);
					return INCORRECT_OPTIONS_ERROR;
				}
				while (string.arr[i] >= '0' && string.arr[i] <= '9') {
					degree = degree * 10 + (string.arr[i] - '0');
					i++;
				}
			} else {
				degree = 1;
			}
		} else {
			degree = 0;
		}
		if (polynomial->degree <= degree) {
			int size = degree - polynomial->degree + 1;
			if (polynomial->coefficients) {
				size -= 1;
			}
			for (int j = 0; j < size; ++j) {
				errorMsg = push_node_start(&(polynomial->coefficients), 0);
				if (errorMsg) {
					destroy_string(&string);
					destroy_polynomial(&polynomial);
					return errorMsg;
				}
			}
			polynomial->degree = degree;
		}
		Node* moving_head = polynomial->coefficients;
		int j = polynomial->degree;
		while (j > degree) {
			moving_head = moving_head->next;
			j--;
		}
		moving_head->data = coefficient;
		coefficient = 0;
		has_coefficient = 0;
		degree = 0;
		sign = 1;
	}
	destroy_string(&string);
	delete_leading_zeros(polynomial);
	*pl = polynomial;
	return SUCCESS;
}

void delete_leading_zeros(Polynomial* p) {
	if (p->degree == 0) return;
	Node* t;
	while (p->coefficients->next && p->coefficients->data == 0) {
		t = p->coefficients;
		p->coefficients = p->coefficients->next;
		free(t);
		p->degree -= 1;
	}
}

error_msg add_polynomials(Polynomial** result, Polynomial* first, Polynomial* second) {
	Polynomial* res = *result;
	error_msg errorMsg = create_polynomial(&res);
	if (errorMsg) {
		return errorMsg;
	}

	// Делаем так, чтобы первый был меньше по степени
	if (first->degree > second->degree) {
		Polynomial* t = second;
		second = first;
		first = t;
	}

	Node* moving_head1 = first->coefficients;
	Node* moving_head2 = second->coefficients;
	int size = (second->degree - first->degree);
	if (!moving_head1) {
		size++;
	}
	for (int i = 0; i < size && moving_head2; ++i) {
		errorMsg = push_node_end(&(res->coefficients), moving_head2->data);
		if (errorMsg) {
			destroy_polynomial(&res);
			return errorMsg;
		}
		moving_head2 = moving_head2->next;
	}
	while (moving_head2 && moving_head1) {
		errorMsg = push_node_end(&(res->coefficients), moving_head1->data + moving_head2->data);
		if (errorMsg) {
			destroy_polynomial(&res);
			return errorMsg;
		}
		moving_head1 = moving_head1->next;
		moving_head2 = moving_head2->next;
	}
	res->degree = second->degree;
	delete_leading_zeros(res);
	*result = res;
	return SUCCESS;
}

error_msg product_polynomials(Polynomial** result, Polynomial* first, Polynomial* second) {
	Polynomial* res = *result;
	error_msg errorMsg = create_polynomial(&res);
	if (errorMsg) {
		return errorMsg;
	}
	for (int i = 0; i <= first->degree + second->degree; ++i) {
		errorMsg = push_node_start(&(res->coefficients), 0);
		if (errorMsg) {
			destroy_polynomial(&res);
			return errorMsg;
		}
	}
	Node* moving_head1 = first->coefficients;
	Node* moving_head2 = second->coefficients;
	int result_degree = first->degree + second->degree;
	int i = first->degree;
	int j;
	res->degree = first->degree + second->degree;
	while (moving_head1) {
		moving_head2 = second->coefficients;
		j = second->degree;
		while (moving_head2) {
			//			printf("|%d|\n", result_degree - (i + j));
			Node* f = find_node_by_index(res->coefficients, result_degree - (i + j));
			if (!f) {
				destroy_polynomial(&res);
				return INDEX_VECTOR_ERROR;
			}
			f->data += moving_head1->data * moving_head2->data;
			j--;
			moving_head2 = moving_head2->next;
		}

		i--;
		moving_head1 = moving_head1->next;
	}
	delete_leading_zeros(res);
	*result = res;
	return SUCCESS;
}

Node* find_node_by_index(Node* head, int index) {
	Node* moving_head = head;
	int i = 0;
	while (moving_head && i++ < index) {
		moving_head = moving_head->next;
	}
	if (moving_head) {
		return moving_head;
	}
	return NULL;
}

error_msg subtract_polynomials(Polynomial** res, Polynomial* first, Polynomial* second) {
	Polynomial* tmp;
	error_msg errorMsg = create_polynomial(&tmp);
	if (errorMsg) {
		return errorMsg;
	}
	Node* moving_head = second->coefficients;
	while (moving_head) {
		errorMsg = push_node_end(&(tmp->coefficients), -moving_head->data);
		if (errorMsg) {
			destroy_polynomial(&tmp);
			return errorMsg;
		}
		moving_head = moving_head->next;
	}
	tmp->degree = second->degree;
	errorMsg = add_polynomials(res, first, tmp);
	if (errorMsg) {
		destroy_polynomial(&tmp);
	}
	destroy_polynomial(&tmp);
	return SUCCESS;
}

error_msg divide_polynomials(Polynomial** quotient, Polynomial** remainder, Polynomial* numerator,
                             Polynomial* denominator) {
	Polynomial* quotient_tmp = *quotient;
	Polynomial* remainder_tmp = *remainder;
	error_msg errorMsg = create_polynomial(&quotient_tmp);
	if (errorMsg) {
		return errorMsg;
	}
	if (denominator->degree == 0 && (!denominator->coefficients || denominator->coefficients->data == 0)) {
		destroy_polynomial(&quotient_tmp);
		return INCORRECT_OPTIONS_ERROR;
	}

	errorMsg = create_polynomial(&remainder_tmp);
	if (errorMsg) {
		destroy_polynomial(&quotient_tmp);
		return errorMsg;
	}

	remainder_tmp->degree = numerator->degree;
	Node* moving_head_numerator = numerator->coefficients;
	for (int i = 0; i <= remainder_tmp->degree; ++i) {
		errorMsg = push_node_end(&(remainder_tmp->coefficients), moving_head_numerator->data);
		if (errorMsg) {
			destroy_polynomial(&quotient_tmp);
			destroy_polynomial(&remainder_tmp);
			return errorMsg;
		}
		moving_head_numerator = moving_head_numerator->next;
	}
	while (remainder_tmp->degree >= denominator->degree) {
		int degree_diff = remainder_tmp->degree - denominator->degree;
		int quotient_coefficient = remainder_tmp->coefficients->data / denominator->coefficients->data;
		errorMsg = push_node_end(&(quotient_tmp->coefficients), (int)quotient_coefficient);
		if (errorMsg) {
			destroy_polynomial(&quotient_tmp);
			destroy_polynomial(&remainder_tmp);
			return errorMsg;
		}
		quotient_tmp->degree = max(quotient_tmp->degree, degree_diff);

		Node* moving_head_denominator = denominator->coefficients;
		Node* moving_head_remainder = remainder_tmp->coefficients;
		while (moving_head_denominator) {
			moving_head_remainder->data -= (int)(quotient_coefficient * (double)moving_head_denominator->data);
			moving_head_denominator = moving_head_denominator->next;
			moving_head_remainder = moving_head_remainder->next;
		}
		delete_leading_zeros(remainder_tmp);
	}
	delete_leading_zeros(quotient_tmp);
	delete_leading_zeros(remainder_tmp);
	*quotient = quotient_tmp;
	*remainder = remainder_tmp;

	return SUCCESS;
}

int eval_polynomial(Polynomial* src, int x) {
	int sum = 0;
	Node* moving_head = src->coefficients;
	while (moving_head) {
		sum = sum * x + moving_head->data;

		moving_head = moving_head->next;
	}
	return sum;
}

long long int special_product(int n, int p) {
	long long int x = 1;
	for (int i = 0; i < n; ++i) {
		x *= p;
		p -= 1;
	}
	return x;
}

int diff_polynomial(Polynomial** result, Polynomial* dst, int order) {
	if (order < 0) {
		return INCORRECT_OPTIONS_ERROR;
	}
	Polynomial* res = *result;
	error_msg errorMsg = create_polynomial(&res);
	if (errorMsg) {
		return errorMsg;
	}
	if (order > dst->degree) {
		errorMsg = push_node_start(&(res->coefficients), 0);
		if (errorMsg) {
			destroy_polynomial(&res);
			return errorMsg;
		}
		*result = res;
		return SUCCESS;
	}
	Node* moving_head = dst->coefficients;
	int i = dst->degree;
	while (moving_head) {
		if (i >= order) {
			errorMsg = push_node_end(&(res->coefficients), moving_head->data * (int)special_product(order, i));
			if (errorMsg) {
				destroy_polynomial(&res);
				return errorMsg;
			}
		} else {
			errorMsg = push_node_end(&(res->coefficients), 0);
			if (errorMsg) {
				destroy_polynomial(&res);
				return errorMsg;
			}
		}
		moving_head = moving_head->next;
		i--;
	}
	res->degree = dst->degree - order;
	delete_leading_zeros(res);
	*result = res;
	return SUCCESS;
}

void print_polynomial(FILE* stream, Polynomial* polynomial) {
	if (polynomial->degree == 0 && polynomial->coefficients->data == 0) {
		fprintf(stream, "0");
		return;
	}
	Node* moving_head = polynomial->coefficients;
	int i = polynomial->degree;
	while (moving_head) {
		if (moving_head->data != 0) {
			if (moving_head != polynomial->coefficients) {
				if ((moving_head->data == 1 || moving_head->data == -1) &&( moving_head->next || i != 0)) {
					if (moving_head->data > 0) {
						fprintf(stream, " + ");
					} else {
						fprintf(stream, " - ");
					}
				} else if (moving_head->data > 0) {
					fprintf(stream, " + %d", moving_head->data);
				} else {
					fprintf(stream, " - %d", -moving_head->data);
				}
			} else {
				if (moving_head->data != 1 && moving_head->data != -1) {
					fprintf(stream, "%d", moving_head->data);
				} else if (moving_head->data == -1) {
					fprintf(stream, "-");
				}
			}
			if (i > 1) {
				fprintf(stream, "x^%d", i);
			} else if (i == 1) {
				fprintf(stream, "x");
			}
		}

		i--;
		moving_head = moving_head->next;
	}
}

error_msg composition_polynomials(Polynomial** result, Polynomial* first, Polynomial* second) {
	Polynomial* res = *result;
	error_msg errorMsg = create_polynomial(&res);
	if (errorMsg) {
		return errorMsg;
	}

	Node* moving_head1 = first->coefficients;
	for (int i = first->degree; i >= 0; --i) {
		Polynomial* tmp1;
		errorMsg = create_polynomial(&tmp1);
		if (errorMsg) {
			destroy_polynomial(&res);
			return errorMsg;
		}

		tmp1->degree = 0;
		errorMsg = push_node_start(&(tmp1->coefficients), moving_head1->data);
		if (errorMsg) {
			destroy_polynomial(&tmp1);
			destroy_polynomial(&res);
			return errorMsg;
		}

		// получаем Q(x)^i
		Polynomial* temp;
		errorMsg = create_polynomial(&temp);
		if (errorMsg) {
			destroy_polynomial(&res);
			destroy_polynomial(&tmp1);
			return errorMsg;
		}
		errorMsg = push_node_end(&(temp->coefficients), 1);
		if (errorMsg) {
			destroy_polynomial(&res);
			destroy_polynomial(&tmp1);
			return errorMsg;
		}
		for (int j = 0; j < i; ++j) {
			Polynomial* temp2;
			errorMsg = product_polynomials(&temp2, temp, second);
			if (errorMsg) {
				destroy_polynomial(&res);
				destroy_polynomial(&tmp1);
				destroy_polynomial(&temp);
				destroy_polynomial(&temp2);
				return errorMsg;
			}
			destroy_polynomial(&temp);
			temp = temp2;
		}
		Polynomial* tmp_d_sum;
		errorMsg = product_polynomials(&tmp_d_sum, tmp1, temp);
		if (errorMsg) {
			destroy_polynomial(&res);
			destroy_polynomial(&tmp1);
			destroy_polynomial(&temp);
			return errorMsg;
		}

		Polynomial* sum;
		errorMsg = add_polynomials(&sum, res, tmp_d_sum);
		if (errorMsg) {
			destroy_polynomial(&tmp1);
			destroy_polynomial(&temp);
			destroy_polynomial(&sum);
			destroy_polynomial(&res);
			destroy_polynomial(&tmp_d_sum);
			return errorMsg;
		}
		destroy_polynomial(&res);
		destroy_polynomial(&tmp1);
		destroy_polynomial(&temp);
		destroy_polynomial(&tmp_d_sum);
		res = sum;
		moving_head1 = moving_head1->next;
	}
	*result = res;
	return SUCCESS;
}

error_msg read_file_with_instruction(FILE* in, FILE* out) {
	error_msg errorMsg;
	if (!in) {
		return INPUT_FILE_ERROR;
	}
	String s;
	errorMsg = create_string(&s, "0");
	if (errorMsg) {
		return errorMsg;
	}
	Polynomial* polynomial;
	errorMsg = read_polynomial_from_string(&polynomial, &s);
	destroy_string(&s);
	if (errorMsg) {
		return errorMsg;
	}
	String input;
	errorMsg = create_string(&input, "");
	if (errorMsg) {
		destroy_polynomial(&polynomial);
		return errorMsg;
	}

	String command;

	errorMsg = create_string(&command, "");
	if (errorMsg) {
		destroy_polynomial(&polynomial);
		return errorMsg;
	}
	do {
		clear_string(&input);
		errorMsg = read_command(in, &input, ';');
		if (errorMsg) {
			destroy_string(&input);
			destroy_polynomial(&polynomial);
			destroy_string(&command);
			return errorMsg;
		}
		if (feof(in) && input.size == 0) {
			break;
		}
		int index_first_bracket = find_index_string(&input, '(');
		int index_second_bracket = find_index_string(&input, ')');
		if (index_second_bracket != input.size - 1) {
			destroy_polynomial(&polynomial);
			destroy_string(&input);
			return INCORRECT_OPTIONS_ERROR;
		}
		int index_comma = find_index_string(&input, ',');
		Polynomial* polynomial1;
		Polynomial* polynomial2;
		if (index_comma != -1) {
			String pol1;
			destroy_polynomial(&polynomial);
			errorMsg = create_string(&pol1, "");
			if (errorMsg) {
				destroy_polynomial(&polynomial);
				destroy_string(&input);
				destroy_string(&command);
				return errorMsg;
			}
			errorMsg = mstrcopy(&input, &pol1, index_first_bracket + 1, index_comma - 1);
			if (errorMsg) {
				destroy_polynomial(&polynomial);
				destroy_string(&pol1);
				destroy_string(&input);
				destroy_string(&command);
				return errorMsg;
			}

			String pol2;
			errorMsg = create_string(&pol2, "");
			if (errorMsg) {
				destroy_polynomial(&polynomial);
				destroy_string(&input);
				destroy_string(&command);
				return errorMsg;
			}
			errorMsg = mstrcopy(&input, &pol2, index_comma + 1, index_second_bracket - 1);
			if (errorMsg) {
				destroy_polynomial(&polynomial);
				destroy_string(&pol1);
				destroy_string(&pol2);
				destroy_string(&input);
				destroy_string(&command);
				return errorMsg;
			}

			errorMsg = read_polynomial_from_string(&polynomial1, &pol1);
			if (errorMsg) {
				destroy_string(&pol2);
				destroy_string(&pol1);
				destroy_string(&input);
				destroy_string(&command);
				return errorMsg;
			}

			errorMsg = read_polynomial_from_string(&polynomial2, &pol2);
			if (errorMsg) {
				destroy_polynomial(&polynomial);
				destroy_string(&command);
				destroy_string(&pol1);
				destroy_polynomial(&polynomial1);
				destroy_string(&input);
				return errorMsg;
			}
			destroy_string(&pol2);
			destroy_string(&pol1);
		} else {
			errorMsg = copy_polynomial(&polynomial, &polynomial1);
			if (errorMsg) {
				destroy_string(&input);
				destroy_string(&command);
				return errorMsg;
			}
			destroy_polynomial(&polynomial);

			String pol2;
			errorMsg = create_string(&pol2, "");
			if (errorMsg) {
				destroy_string(&input);
				destroy_string(&command);
				destroy_polynomial(&polynomial1);
				return errorMsg;
			}
			errorMsg = mstrcopy(&input, &pol2, index_first_bracket + 1, index_second_bracket - 1);
			if (errorMsg) {
				destroy_string(&pol2);
				destroy_string(&input);
				destroy_string(&command);
				destroy_polynomial(&polynomial1);
				return errorMsg;
			}

			errorMsg = read_polynomial_from_string(&polynomial2, &pol2);
			if (errorMsg) {
				destroy_string(&input);
				destroy_string(&pol2);
				destroy_string(&command);
				destroy_polynomial(&polynomial1);
				return errorMsg;
			}
			destroy_string(&pol2);
		}

		clear_string(&command);
		errorMsg = mstrcopy(&input, &command, 0, index_first_bracket);
		if (errorMsg) {
			destroy_string(&command);
			destroy_string(&input);
			destroy_polynomial(&polynomial1);
			destroy_polynomial(&polynomial2);
			return errorMsg;
		}

		if (string_cmp(command.arr, "Add")) {
			errorMsg = add_polynomials(&polynomial, polynomial1, polynomial2);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			print_polynomial(out, polynomial);
			putc('\n', out);
		} else if (string_cmp(command.arr, "Sub")) {
			errorMsg = subtract_polynomials(&polynomial, polynomial1, polynomial2);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			print_polynomial(out, polynomial);
			putc('\n', out);
		} else if (string_cmp(command.arr, "Mult")) {
			errorMsg = product_polynomials(&polynomial, polynomial1, polynomial2);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			print_polynomial(out, polynomial);
			putc('\n', out);
		} else if (string_cmp(command.arr, "Div")) {
			Polynomial* remainder;
			errorMsg = divide_polynomials(&polynomial, &remainder, polynomial1, polynomial2);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			print_polynomial(out, polynomial);
			putc('\n', out);
			destroy_polynomial(&remainder);
		} else if (string_cmp(command.arr, "Mod")) {
			Polynomial* quotient;
			errorMsg = divide_polynomials(&quotient, &polynomial, polynomial1, polynomial2);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			print_polynomial(out, polynomial);
			destroy_polynomial(&quotient);
			putc('\n', out);
		} else if (string_cmp(command.arr, "Eval")) {
			if (polynomial2->degree != 0) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			int x = eval_polynomial(polynomial1, polynomial2->coefficients->data);
			errorMsg = create_polynomial(&polynomial);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
			}
			errorMsg = push_node_start(&(polynomial->coefficients), x);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
			}
			fprintf(out, "%d\n", x);
		} else if (string_cmp(command.arr, "Diff")) {
			errorMsg = diff_polynomial(&polynomial, polynomial1, polynomial2->coefficients->data);
			if (errorMsg || polynomial2->degree != 0) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return INCORRECT_OPTIONS_ERROR;
			}
			print_polynomial(out, polynomial);
			putc('\n', out);
		} else if (string_cmp(command.arr, "Cmps")) {
			errorMsg = composition_polynomials(&polynomial, polynomial1, polynomial2);
			if (errorMsg) {
				destroy_string(&command);
				destroy_string(&input);
				destroy_polynomial(&polynomial1);
				destroy_polynomial(&polynomial2);
				return errorMsg;
			}
			print_polynomial(out, polynomial);
			putc('\n', out);
		} else {
			destroy_string(&command);
			destroy_string(&input);
			destroy_polynomial(&polynomial1);
			destroy_polynomial(&polynomial2);
			return INCORRECT_OPTIONS_ERROR;
		}
		destroy_polynomial(&polynomial1);
		destroy_polynomial(&polynomial2);
	} while (!feof(in));
	destroy_string(&input);
	destroy_string(&command);
	destroy_polynomial(&polynomial);
	return SUCCESS;
}

error_msg read_command(FILE* stream, String* string, char separator) {
	clear_string(string);
	if (!stream) return 0;
	int count_read_symbol = 0;
	char c;
	while ((c = getc(stream)) != EOF) {
		if (c == separator) {
			return SUCCESS;
		}
		while (c != EOF && (c == ' ' || c == '\t' || c == separator || c == '\n')) {
			c = getc(stream);
		}
		if (c == '%') {
			while ((c = getc(stream)) != EOF && c != '\n')
				;
			continue;
		}
		if (c == '[') {
			while ((c = getc(stream)) != EOF && c != ']')
				;
			if (c != ']') return INCORRECT_OPTIONS_ERROR;
			continue;
		}

		if (c == EOF) {
			if (!count_read_symbol)
				return SUCCESS;
			else
				return INCORRECT_OPTIONS_ERROR;
		}
		count_read_symbol++;
		error_msg errorMsg = push_end_string(string, c);
		if (errorMsg) return errorMsg;
	}
	if (c == EOF) {
		if (!count_read_symbol)
			return SUCCESS;
		else
			return INCORRECT_OPTIONS_ERROR;
	}
	return SUCCESS;
}

error_msg copy_polynomial(Polynomial** dest, Polynomial** src) {
	error_msg errorMsg;
	errorMsg = create_polynomial(src);
	if (errorMsg) {
		return errorMsg;
	}
	(*src)->degree = (*dest)->degree;
	Node* moving_head = (*dest)->coefficients;
	while (moving_head) {
		errorMsg = push_node_end(&((*src)->coefficients), moving_head->data);
		if (errorMsg) {
			destroy_polynomial(&*src);
			return errorMsg;
		}
		moving_head = moving_head->next;
	}
	return SUCCESS;
}