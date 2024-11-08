#include "5.h"

#include <stdio.h>
#include <stdlib.h>

int print_error(error_msg error){
	switch (error) {
		case INPUT_FILE_ERROR:
			fprintf(stderr, "Input file didnt open\n");
		return 1;
		case OUTPUT_FILE_ERROR:
			fprintf(stderr, "Output file didnt open\n");
		return 2;
		case MEMORY_ALLOCATED_ERROR:
			fprintf(stderr, "Memory allocated\n");
		return 3;
		case INDEX_VECTOR_ERROR:
			fprintf(stderr, "Handling outside the array index\n");
		return 4;
		case OVERFLOW_ERROR:
			fprintf(stderr, "Overflow error\n");
		return 5;
		case INCORRECT_OPTIONS_ERROR:
			fprintf(stderr, "Incorrect options\n");
		return 6;
		case UNRECOGNIZED_CHARACTER_ERROR:
			fprintf(stderr, "Unrecognized character\n");
		return 7;
		case NUMERAL_SYSTEM_ERROR:
			fprintf(stderr, "This numeral system doesn't exist\n");
		return 8;
		default:
			return 0;
	}
}

IntVector* create_int_vector(int capacity) {
	IntVector* vector = (IntVector*)malloc(sizeof(IntVector));
	if (!vector) {
		return NULL;
	}
	vector->arr = (int*)malloc(sizeof(int) * capacity);
	if (!(vector->arr)) {
		return NULL;
	}
	vector->size = 0;
	vector->capacity = capacity;
	return vector;
}

int resize_int_vector(IntVector* vector, int new_capacity) {
	vector->arr = (int*)realloc(vector->arr, sizeof(int) * new_capacity);
	if (!(vector->arr)) {
		return MEMORY_ALLOCATED_ERROR;
	}
	vector->capacity = new_capacity;
	return SUCCESS;
}

error_msg push_end_intvector(IntVector* vector, int new_element) {
	if (vector->size == vector->capacity) {
		error_msg error = resize_int_vector(vector, vector->capacity * 2);
		if (error) return error;
	}
	vector->size += 1;
	vector->arr[vector->size - 1] = new_element;
	return SUCCESS;
}

error_msg get_intvector(IntVector* vector, int index, int* value) {
	if (index >= vector->size) {
		return INDEX_VECTOR_ERROR;
	}
	*value = vector->arr[index];
	return SUCCESS;
}

error_msg at_intvector(IntVector* vector, int index, int new_element) {
	if (index >= vector->size) {
		return INDEX_VECTOR_ERROR;
	}
	vector->arr[index] = new_element;
	return SUCCESS;
}

int size_intvector(IntVector* vector) { return vector->size; }

void destroy_int_vector(IntVector* vector) {
	free(vector->arr);
	free(vector);
}

void print_intvector(FILE * stream, IntVector* vector, char * separator) {
	for (int i = 0; i < vector->size; ++i) {
		fprintf(stream, "%d%s", vector->arr[i], separator);
	}
}

error_msg create_string(String* vec, char* src) {
	vec->size = SizeString(src);
	vec->capacity = vec->size + 5;
	vec->arr = (char*)malloc(sizeof(char) * vec->capacity);
	if (!vec->arr) {
		return MEMORY_ALLOCATED_ERROR;
	}
	error_msg errorMsg = strcopy(src, vec->arr, 0, SizeString(src));
	if (errorMsg) return errorMsg;
	return SUCCESS;
}

error_msg mstrcopy(const String* dest, String* result, int start, int end) {
	if(start > end){
		return INCORRECT_OPTIONS_ERROR;
	}
	error_msg errorMsg;
	errorMsg = resize_string(result, dest->size + 1);
	if (errorMsg) return errorMsg;
	result->size = end - start;
	if (start > 0) result->size += 1;
	errorMsg = strcopy(dest->arr, result->arr, start, end);
	if (errorMsg) return errorMsg;
	return SUCCESS;
}

error_msg resize_string(String* vector, int new_capacity) {
	if (vector->capacity < new_capacity) {
		char* tmp = (char*)realloc(vector->arr, sizeof(char) * new_capacity);
		if (!tmp) {
			return MEMORY_ALLOCATED_ERROR;
		}
		vector->arr = tmp;
		vector->capacity = new_capacity;
	}
	return SUCCESS;
}

error_msg push_end_string(String* vector, char new_element) {
	if (vector->size >= vector->capacity - 1) {
		error_msg error = resize_string(vector, vector->capacity * 2);
		if (error) return error;
	}
	vector->size += 1;
	vector->arr[vector->size - 1] = new_element;
	vector->arr[vector->size] = '\0';
	return SUCCESS;
}

error_msg get_string(String* vector, int index, char* value) {
	if (index >= vector->capacity) {
		return INDEX_VECTOR_ERROR;
	}
	*value = vector->arr[index];
	return SUCCESS;
}

error_msg at_string(String* vector, int index, char new_element) {
	if (index >= vector->capacity) {
		return INDEX_VECTOR_ERROR;
	}
	vector->arr[index] = new_element;
	return SUCCESS;
}

int size_string(String* vector) { return vector->size; }

void destroy_string(String* vector) {
	vector->size = 0;
	vector->capacity = 0;
	free(vector->arr);
}

void print_string(FILE* stream, String* vector, char* separator) {
	for (int i = 0; i < vector->size; ++i) {
		fprintf(stream, "%c%s", vector->arr[i], separator);
	}
}

void vector_string(String* vector) { vector->size = 0; }

int strings_equals(const String* s1, const String* s2) { return string_cmp(s1->arr, s2->arr); }

error_msg mstrcopynew(const String* src, String* result) {
	error_msg errorMsg = create_string(result, src->arr);
	if (errorMsg) return errorMsg;
	return SUCCESS;
}

error_msg mstrcat(String* first, const String* second) {
	error_msg errorMsg;

	if (first->capacity < first->size + second->size) {
		errorMsg = resize_string(first, first->size + second->size + 1);
		if (errorMsg) return errorMsg;
	}

	errorMsg = my_strcat(first->arr, second->arr);

	if (errorMsg) return errorMsg;
	first->size += second->size;
	return SUCCESS;
}

int string_comp(const String* s1, const String* s2) {
	int len1 = SizeString(s1->arr);
	int len2 = SizeString(s2->arr);
	if (len1 > len2) return 1;
	if (len1 < len2) return -1;
	return string_compare(s1->arr, s2->arr);
}

int string_to_int(const String* s1) {
	int sum = 0;
	for (int i = 0; i < s1->size; ++i) {
		sum = sum * 10 + (s1->arr[i] - '0');
	}
	return sum;
}

int read_string(FILE* stream, String* string) {
	if (!stream) return 0;
	int count_read_symbol = 0;
	char c;
	while (((c = getc(stream)) != EOF) && (c == ' ' || c == '\t'));
	do {
		if (c == '\n' || c == '\t' || c == ' ' || c == EOF) {
			return count_read_symbol;
		}
		count_read_symbol++;
		error_msg errorMsg = push_end_string(string, c);
		if (errorMsg) return count_read_symbol;
	} while ((c = getc(stream)) != EOF);
	return count_read_symbol;
}

int read_line(FILE* stream, String* string) {
	if (!stream) return 0;
	int count_read_symbol = 0;
	char c;
	while ((c = getc(stream)) != EOF) {
		if (c == '\n' || c == EOF) {
			break;
		}
		count_read_symbol++;
		error_msg errorMsg = push_end_string(string, c);
		if (errorMsg) {
			return count_read_symbol;
		}
	}
	if (c == '\n') {
		count_read_symbol++;
		error_msg errorMsg = push_end_string(string, c);
		if (errorMsg) {
			return count_read_symbol;
		}
	}
	return count_read_symbol;
}


void clear_string(String * string){
	string->size = 0;
	string->arr[0] = '\0';
}


int find_index_string(String * s, char c){
	for(int i = 0; i < s->size; ++i){
		if(s->arr[i] == c){
			return i;
		}
	}
	return -1;
}

int read_string_before_separator(FILE * stream, String * string, char separator){
	int count = 0;
	char c;
	error_msg errorMsg;
	while (!feof(stream)){
		c = getc(stream);
		if(c == separator){
			return count;
		}
		errorMsg = push_end_string(string, c);
		if(errorMsg){
			return count;
		}
	}
	return count;
}

StringVector *create_string_vector(int size) {
	StringVector *vec = (StringVector *)malloc(sizeof(StringVector));
	if (!vec) return NULL;
	vec->data = (char **)malloc(size * sizeof(char *));
	if (!vec->data) {
		free(vec);
		return NULL;
	}
	vec->capacity = size;
	vec->size = 0;
	return vec;
}

error_msg resize_string_vector(StringVector *vec) {
	char **new_data = (char **)realloc(vec->data, vec->capacity * 2 * sizeof(char *));
	if (!new_data) {
		return MEMORY_ALLOCATED_ERROR;
	}
	vec->data = new_data;
	vec->capacity *= 2;
	return SUCCESS;
}

error_msg push_end_string_vector(StringVector *vec, char *string) {
	error_msg error;
	if (vec->size == vec->capacity) {
		error = resize_string_vector(vec);
		if (error) return error;
	}
	vec->data[vec->size] = (char *)malloc((SizeString(string) + 1) * sizeof(char));
	if (!(vec->data[vec->size])) {
		destroy_string_vector(vec);
		return MEMORY_ALLOCATED_ERROR;
	}
	strcopy(string, vec->data[vec->size], 0, SizeString(string));
	vec->size += 1;
	//	print_string_vector(vec);
	return SUCCESS;
}

error_msg at_string_vector(StringVector *vec, int index, char *res) {
	if (index >= vec->size) {
		return INDEX_VECTOR_ERROR;
	}
	free(vec->data[index]);
	vec->data[index] = (char *)malloc((SizeString(res) + 1) * sizeof(char));
	if (!vec->data[index]) {
		destroy_string_vector(vec);
		return MEMORY_ALLOCATED_ERROR;
	}
	strcopy(res, vec->data[index], 0, SizeString(res));
	return SUCCESS;
}

error_msg get_string_vector(StringVector *vec, int index, char **res) {
	if (index >= vec->size) {
		return INDEX_VECTOR_ERROR;
	}
	*res = vec->data[index];
	return SUCCESS;
}

void print_string_vector(FILE * stream, StringVector *vec, char *separator) {
	for (int i = 0; i < vec->size; ++i) {
		fprintf(stream, "%s%s", (vec->data)[i], separator);
	}
}

void destroy_string_vector(StringVector *vec) {
	if (!vec) return;
	if (vec->data) {
		for (int i = 0; i < vec->size; i++) {
			if (vec->data[i]) {
				free(vec->data[i]);
			}
		}
		free(vec->data);
	}
	free(vec);
}

error_msg str_to_k(const char **string, char *separator, char *result) {
	error_msg error;
	int len_string = SizeString(*string);
	int len_separator = SizeString(separator);
	char b[len_separator + 1];
	if (len_string == 0) {
		result = NULL;
		return SUCCESS;
	}
	for (int i = 0; i < len_string; ++i) {
		//        printf("%s\n", *string);
		error = strcopy(*string, b, 0, len_separator);
		if (error) return error;
		if (string_cmp(b, separator)) {
			result[i] = '\0';
			*string += len_separator;
			break;
		}
		if (len_separator == SizeString(*string)) {
			result[i] = '\0';
			error = my_strcat(result, *string);
			if (error) return error;
			*string = "\0";
			break;
		}
		result[i] = **string;
		*string += 1;
	}
	return SUCCESS;
}

error_msg my_strcat(char *a, const char *b) {
	int i = SizeString(a);
	for (int j = 0; j < SizeString(b); ++i, ++j) {
		a[i] = b[j];
	}
	a[i] = '\0';
	return SUCCESS;
}

int string_cmp(const char *a, const char *b) {
	int i;
	for (i = 0; a[i] != '\0' && b[i] != '\0'; ++i) {
		if (a[i] != b[i]) return 0;
	}
	if (a[i] != b[i]) return 0;
	return 1;
}

error_msg strcopy(const char *a, char *result, int start, int end) {
	int i = 0;
	int end_for = end - start;
	if (start > 0) end_for++;
	for (; i < end_for && i + start < SizeString(a); ++i) {
		result[i] = a[i + start];
	}
	result[i] = '\0';
	return SUCCESS;
}

int SizeString(const char *a) {
	int i = 0;
	for (; a[i] != '\0'; ++i)
		;
	return i;
}

int string_compare(const char *s1, const char*s2){
	int i = 0;
	for(; s1[i] && s2[i]; ++i){
		if(s1[i] - s2[i] != 0) return s1[i] - s2[i];
	}
	return s1[i] - s2[i];
}


int strtoint(const char * a){
	int sum = 0;
	while (*a){
		sum *= 10 + (*a - '0');
		a++;
	}
	return sum;
}


int IsCorrectNameLastName(String * s){
	for(int i = 0;i < s->size; ++i){
		if(!((s->arr[i] >= 'a' && s->arr[i] <= 'z') ||(s->arr[i] >= 'A' && s->arr[i] <= 'Z'))){
			return 0;
		}
	}
	return 1;
}


error_msg create_student(Student *student) {
	error_msg errorMsg = create_string(&(student->name), "");
	if (errorMsg) {
		return errorMsg;
	}

	errorMsg = create_string(&(student->last_name), "");
	if (errorMsg) {
		destroy_string(&(student->name));
		return errorMsg;
	}

	errorMsg = create_string(&(student->group), "");
	if (errorMsg) {
		destroy_string(&(student->name));
		destroy_string(&(student->last_name));
		return errorMsg;
	}

	student->grades = (unsigned char *)malloc(sizeof(unsigned char) * 5);
	if (!student->grades) {
		destroy_string(&(student->name));
		destroy_string(&(student->last_name));
		destroy_string(&(student->group));
		return MEMORY_ALLOCATED_ERROR;
	}
	return SUCCESS;
}

void destroy_student(Student *student) {
	student->id = 0;
	destroy_string(&(student->name));
	destroy_string(&(student->last_name));
	destroy_string(&(student->group));
	free(student->grades);
}

int read_student(FILE *stream, Student *student) {
	int count_read = 0;
	if (!stream) {
		return INPUT_FILE_ERROR;
	}

	int n = fscanf(stream, "%u ", &(student->id));
	if (n <= 0) {
		return count_read;
	}
	count_read += 1;

	n = read_string(stream, &(student->name));
	if (n <= 0 || !IsCorrectNameLastName(&(student->name))) {
		return count_read;
	}
	count_read += 1;

	n = read_string(stream, &(student->last_name));
	if (n <= 0 || !IsCorrectNameLastName(&(student->last_name))) {
		return count_read;
	}
	count_read += 1;

	n = read_string(stream, &(student->group));
	if (n <= 0) {
		return count_read;
	}
	count_read += 1;

	for (int i = 0; i < 5; ++i) {
		if (feof(stream)) {
			return count_read;
		}
		student->grades[i] = getc(stream);
		if (i != 4) {
			getc(stream);
		}
		count_read += 1;
	}
//	student->grades[5] = '\0';
	return count_read;
}

void print_student(FILE *stream, Student *student) {
	fprintf(stream, "%u ", student->id);
	print_string(stream, &(student->name), "");
	putc(' ', stream);
	print_string(stream, &(student->last_name), "");
	putc(' ', stream);
	print_string(stream, &(student->group), "");
	for (int i = 0; i < 5; ++i) {
		fprintf(stream, " %c", student->grades[i]);
	}
	putc('\n', stream);
}

error_msg GetPaths(int argc, char **argv, const char **path1, const char **path2) {
	if (argc != 3) {
		return INCORRECT_OPTIONS_ERROR;
	}
	*path1 = argv[1];
	*path2 = argv[2];
	return SUCCESS;
}

error_msg read_students(FILE *stream, Students *students) {
	if (!stream) {
		return INPUT_FILE_ERROR;
	}

	students->capacity_students = 20;
	students->count_students = 0;
	students->students = (Student *)calloc(sizeof(Student), students->capacity_students);
	if (!students->students) {
		return MEMORY_ALLOCATED_ERROR;
	}

	int n;
	error_msg errorMsg;
	while (!feof(stream)){
		if(students->count_students == students->capacity_students){
			Student * tmp = (Student*)realloc(students->students, students->capacity_students * 2 * sizeof(Student));
			if(!tmp){
				destroy_students(students);
				return MEMORY_ALLOCATED_ERROR;
			}
			students->students = tmp;
			students->capacity_students *= 2;
		}

		errorMsg = create_student(&(students->students[students->count_students]));
		if(errorMsg){
			students->count_students += 1;
			destroy_students(students);
			return errorMsg;
		}

		n = read_student(stream, &(students->students[students->count_students]));
		if(n == 0 && feof(stream)){
			destroy_student(&(students->students[students->count_students]));
			return SUCCESS;
		}

		if(n != 9 || !check_student_id(students, &(students->students[students->count_students]))){
			destroy_student(&(students->students[students->count_students]));
			destroy_students(students);
			return INCORRECT_OPTIONS_ERROR;
		}

		students->count_students += 1;
	}

	return SUCCESS;
}

void print_students(FILE *stream, Students *students) {
	for (int i = 0; i < students->count_students; ++i) {
		print_student(stream, &(students->students[i]));
	}
}

void destroy_students(Students *students) {
	for (int i = 0; i < students->count_students; ++i) {
		destroy_student(&(students->students[i]));
	}
	free(students->students);
	students->count_students = 0;
	students->capacity_students = 0;
}

int check_student_id(Students *students, Student *student) {
	for (int i = 0; i < students->count_students; ++i) {
		if (student->id == students->students[i].id) {
			return 0;
		}
	}
	return 1;
}

void clear_student(Student *student) {
	student->name.size = 0;
	student->last_name.size = 0;
	student->group.size = 0;
}

int find_student_by_id(Students * students, unsigned int id){
	for(int i = 0; i < students->count_students; ++i){
		if(id == students->students[i].id){
			return i;
		}
	}
	return -1;
}

int find_students_by_last_name(Students * students, IntVector * vec, String * last_name){
	int count_students = 0;
	for(int i = 0; i < students->count_students; ++i){
		if(strings_equals(&(students->students[i].last_name), last_name)){
			count_students += 1;
			error_msg errorMsg = push_end_intvector(vec, i);
			if(errorMsg){
				return -1;
			}
		}
	}
	return count_students;
}


int find_students_by_name(Students * students, IntVector * vec, String * name){
	int count_students = 0;
	for(int i = 0; i < students->count_students; ++i){
		if(strings_equals(&(students->students[i].name), name)){
			count_students += 1;
			error_msg errorMsg = push_end_intvector(vec, i);
			if(errorMsg){
				return -1;
			}
		}
	}
	return count_students;
}

int find_students_by_group(Students *students, IntVector *vec, String *group) {
	int count_students = 0;
	for(int i = 0; i < students->count_students; ++i){
		if(strings_equals(&(students->students[i].group), group)){
			count_students += 1;
			error_msg errorMsg = push_end_intvector(vec, i);
			if(errorMsg){
				return -1;
			}
		}
	}
	return count_students;
}

int compare_by_id(const void * a, const void * b){
	Student first = *((Student*)a);
	Student second = *((Student*)b);
	return (int)first.id - (int)second.id;
}

void sort_by_id(Students * students){
	qsort(students->students, students->count_students, sizeof(Student), compare_by_id);
}

int compare_by_name(const void * a, const void * b){
	Student first = *((Student*)a);
	Student second = *((Student*)b);
	return string_comp(&(first.name), &(second.name));
}

void sort_by_name(Students * students){
	qsort(students->students, students->count_students, sizeof(Student), compare_by_name);
}


int compare_by_last_name(const void * a, const void * b){
	Student first = *((Student*)a);
	Student second = *((Student*)b);
	return string_comp(&(first.last_name), &(second.last_name));
}

void sort_by_last_name(Students * students){
	qsort(students->students, students->count_students, sizeof(Student), compare_by_last_name);
}

int compare_by_group(const void * a, const void * b){
	Student first = *((Student*)a);
	Student second = *((Student*)b);
	return string_comp(&(first.group), &(second.group));
}

void sort_by_group(Students * students){
	qsort(students->students, students->count_students, sizeof(Student), compare_by_group);
}


double calculate_average_score_student(Student * student){
	double score = 0.0;
	for(int i = 0; i < 5; ++i){
		score += (double )(student->grades[i] - '0');
	}
	score /= 5.0;
	return score;
}

double calculate_average_score_students(Students * students){
	double score = 0.0;
	for(int i = 0; i < students->count_students; ++i){
		score += calculate_average_score_student(&(students->students[i]));
	}
	score /= (double )students->count_students;
	return score;
}

int filter_students_by_average_score(Students * students, IntVector * result){
	if(!result || !result->arr){
		return -1;
	}
	int count_good_students = 0;
	error_msg errorMsg;
	double average_score;
	double average_score_all_students = calculate_average_score_students(students);
	for(int i = 0; i < students->count_students; ++i){
		average_score = calculate_average_score_student(&(students->students[i]));
		if(average_score > average_score_all_students){
			count_good_students += 1;
			errorMsg = push_end_intvector(result, i);
			if(errorMsg) return -1;
		}
	}
	return count_good_students;
}


void clear_buffer(){
	while ( getchar() != '\n' );
}

