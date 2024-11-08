//
// Created by Данир on 08.11.2024.
//

#ifndef INC_7_H
#define INC_7_H
#include <stdlib.h>
#include <stdio.h>
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


typedef struct String {
	int capacity;
	int size;
	char* arr;
} String;

error_msg str_to_k(const char **string, char *separator, char *result);
error_msg my_strcat(char *a, const char *b);
int string_cmp(const char *a, const char *b);
error_msg strcopy(const char *a, char *result, int start, int end);
int SizeString(const char *a);
int string_compare(const char *s1, const char*s2);
int strtoint(const char * a);

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


typedef struct Date {
	unsigned int day;
	unsigned int month;
	unsigned int year;
} Date;

typedef struct Liver {
	String last_name;
	String name;
	String patronymic;
	Date birthday;
	char gender;
	double income;
} Liver;

typedef struct Node {
	struct Node* next;
	Liver data;
} Node;

typedef struct SpecialNode{
/*
 * 0 - удаление
 * 1 - вставка
 * 2 - изменение
 */
	Liver liver;
	int instruction;
	struct SpecialNode * next;
}SpecialNode;

// Функции для односвязного John Smith 15списка
error_msg create_node(Node** node, Liver data);
void destroy_node(Node ** head);
void delete_node(Node* prev, Node* cur);
void destroy_LinkedList(Node* head);
void add_node(Node* prev, Node* cur);
error_msg push_node_end(Node** head, Liver data);
error_msg push_node_start(Node** head, Liver data);
Node* find_node_by_index(Node* head, int index);
error_msg push_node_in_correct_order(Node ** head, Liver * cur);
void print_LinkedList(FILE* stream, Node* head, char* separator);

// Функции для специального односвязного списка(по сути стэк)
error_msg create_special_node(SpecialNode** node, Liver data, int instruction);
void destroy_special_node(SpecialNode ** head);
void destroy_LinkedList_with_special_node(SpecialNode * head);
error_msg push_special_node_start(SpecialNode ** head, Liver data, int instruction);

error_msg read_livers_from_file(FILE* in, Node** head);
int eq_livers(Liver * l1, Liver* l2);
void print_liver(FILE * stream, Liver * liver);
int read_liver(FILE* stream, Liver* liver);
error_msg create_liver(Liver* liver);
error_msg delete_liver(Node ** head, Liver * liver);
error_msg copy_liver(const Liver * dst, Liver * src);
Node * find(Node ** head, Liver * liver);
error_msg change_liver(Node ** head, Liver * liver, Liver * new_liver);
void destroy_liver(Liver * liver);

unsigned int DayInMonth(unsigned int month, unsigned int year);
int IsCorrectDate(Date* time);
int BiggerDate(Date* t1, Date* t2);

void clear_buffer();

int IsCorrectNameLastName(String *s);


#endif //INC_7_H
