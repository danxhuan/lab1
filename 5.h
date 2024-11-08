//
// Created by Данир on 08.11.2024.
//
#ifndef INC_5_H
#define INC_5_H

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

typedef struct IntVector {
    int capacity;
    int size;
    int* arr;
} IntVector;

IntVector * create_int_vector(int capacity);
error_msg push_end_intvector(IntVector* vector, int new_element);
error_msg get_intvector(IntVector* vector, int index, int* value);
void destroy_int_vector(IntVector* vector);
error_msg at_intvector(IntVector* vector, int index, int new_element);
int size_intvector(IntVector* vector);
void print_intvector(FILE * stream, IntVector* vector, char * separator);

typedef struct StringVector {
    char **data;
    int size;
    int capacity;
} StringVector;

error_msg strcopy(const char *a, char *result, int start, int end);
int string_cmp(const char *a, const char *b);
error_msg str_to_k(const char **string, char *separator, char * result);
int SizeString(const char *a);
error_msg my_strcat(char *a, const char *b);

StringVector *create_string_vector(int size);
error_msg resize_string_vector(StringVector *vec);
error_msg push_end_string_vector(StringVector *vec, char *string);
error_msg at_string_vector(StringVector *vec, int index, char *res);
error_msg get_string_vector(StringVector *vec, int index, char **res);
void print_string_vector(FILE * stream, StringVector *vec, char *separator);
void destroy_string_vector(StringVector *vec);


typedef struct Student {
    unsigned int id;
    String name;
    String last_name;
    String group;
    unsigned char* grades;
} Student;

typedef struct Students {
    Student* students;
    int count_students;
    int capacity_students;
} Students;

error_msg GetPaths(int argc, char** argv, const char** path1, const char** path2);

int check_student_id(Students* students, Student* student);
error_msg create_student(Student* student);
void destroy_student(Student* student);
int read_student(FILE* stream, Student* student);
void print_student(FILE* stream, Student* student);
error_msg read_students(FILE* stream, Students* students);
void print_students(FILE* stream, Students* students);
void destroy_students(Students* students);
void clear_student(Student* student);

int find_student_by_id(Students* students, unsigned int id);
int find_students_by_last_name(Students* students, IntVector* vec, String* last_name);
int find_students_by_name(Students* students, IntVector* vec, String* name);
int find_students_by_group(Students* students, IntVector* vec, String* group);

void sort_by_id(Students * students);
void sort_by_name(Students * students);
void sort_by_last_name(Students * students);
void sort_by_group(Students * students);

int compare_by_id(const void * a, const void * b);
int compare_by_name(const void * a, const void * b);
int compare_by_last_name(const void * a, const void * b);
int compare_by_group(const void * a, const void * b);

int filter_students_by_average_score(Students * students, IntVector * result);
double calculate_average_score_students(Students * students);
double calculate_average_score_student(Student * student);

void clear_buffer();

#endif INC_5_H