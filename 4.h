#ifndef LAB3_MAIL_H
#define LAB3_MAIL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef enum error_msg{
	SUCCESS,
	INPUT_FILE_ERROR,
	OUTPUT_FILE_ERROR,
	MEMORY_ALLOCATED_ERROR,
	INDEX_VECTOR_ERROR,
	OVERFLOW_ERROR,
	INCORRECT_OPTIONS_ERROR,
	UNRECOGNIZED_CHARACTER_ERROR,
	NUMERAL_SYSTEM_ERROR
} error_msg;

int print_error(error_msg error);

error_msg str_to_k(const char **string, char *separator, char *result);
error_msg my_strcat(char *a, const char *b);
int string_cmp(const char *a, const char *b);
error_msg strcopy(const char *a, char *result, int start, int end);
int SizeString(const char *a);
int string_compare(const char *s1, const char*s2);
int strtoint(const char * a);

typedef struct String {
	int capacity;
	int size;
	char* arr;
} String;

error_msg create_string(String* vec, char* src);
error_msg push_end_string(String* vector, char new_element);
int find_index_string(String * s, char c);
error_msg get_string(String* vector, int index, char* value);
error_msg mstrcopy(const String* dest, String* result, int start, int end);
int strings_equals(const String* s1, const String* s2);
void destroy_string(String* vector);
error_msg at_string(String* vector, int index, char new_element);
int size_string(String* vector);
void print_string(FILE* stream, String* vector, char* separator);
void vector_string(String* vector);
error_msg resize_string(String* vector, int new_capacity);
error_msg mstrcopynew(const String* dest, String* result);
error_msg mstrcat(String* first, const String* second);
int string_comp(const String* s1, const String* s2);
int string_to_int(const String* s1);

int read_string_before_separator(FILE * stream, String * string, char separator);
int read_string(FILE* stream, String* string);
int read_line(FILE* stream, String* string);

void clear_string(String * string);


typedef struct Address{
	String city;
	String street;
	int house;
	String building;
	int apartment;
	String index;

}Address;

typedef struct Mail{
	Address address;
	double parcel_weight;
	String mail_id;
	String mail_create_time;
	String mail_receipt_time;
}Mail;

typedef struct Post{
	Address * address;
	Mail * mails;
} Post;


typedef struct Time{
	int day;
	int month;
	int year;
	int hour;
	int minute;
	int sec;
} Time;

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

// адреса
error_msg create_address_ptr(Address ** address, char * city, char * street, int house, char * building, int apartment, char * index);
error_msg create_address(Address * address, char * city, char * street, int house, char * building, int apartment, char * index);
void destroy_address_ptr(Address * address);
void destroy_address(Address *address) ;
void print_address(FILE *stream, Address *address);

// Почта
error_msg create_post(Post * post, int capacity_mails, char * city, char * street, int house, char * building, int apartment, char * index);
void print_post(FILE * stream, Post * post, int count_mails);
void destroy_post(Post *post, int count_mails);

error_msg create_mail(Mail *,char * city, char * street, int house, char * building, int apartment, char * index, double parcel_weight, char * mail_id, Time mail_create_time, Time mail_receipt_time);
void destroy_mail(Mail * mail);
void print_mail(FILE * stream, Mail * mail);

// Добавл письма
error_msg push_mail_into_post(Post * post, int* count_mails, int *size_post, Mail* mail);
error_msg find_mail_in_post(Post * post, Mail* mail, String* mail_id, int count_mails, int * index);
error_msg delete_mail_in_post(Post * post, String * mail_id, int* count_mails);

// Функции ищвщие доставленные и не доставленные письма
error_msg find_received_mails(Post * post, int count_mail);
error_msg find_not_received_mails(Post *post, int count_mail);


error_msg scan_email(Mail * mail);
void clear_buffer();
#endif  // LAB3_MAIL_H