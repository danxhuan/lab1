#include "5.h"


int to_what_stream(FILE *output_file, FILE **stream, const char **MSG) {
	//  куда вывод?
	printf("%s", MSG[3]);
	char format[10];
	int n = scanf("%s", format);
	if (n == 0 || (!string_cmp(format, "0") && !string_cmp(format, "1"))) {
		printf("%s", MSG[2]);
		clear_buffer();
		return -1;
	} else if (string_cmp(format, "1")) {
		// в трассировочный файл
		*stream = output_file;
	}
	return 0;
}

int find_by_name(FILE *output_file, String *command, Students *students, const char **MSG) {
	int n;
	error_msg errorMsg;
	// Куда выводить
	FILE *stream = stdout;
	int what_stream = to_what_stream(output_file, &stream, MSG);
	if (what_stream == -1) {
		return -1;
	}
	String name;

	IntVector *result = create_int_vector(1);
	if (!result) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		return print_error(MEMORY_ALLOCATED_ERROR);
	}
	errorMsg = create_string(&name, "");
	if (errorMsg) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		destroy_int_vector(result);
		return print_error(errorMsg);
	}

	getc(stdin);
	printf("Write name: ");
	n = read_string(stdin, &name);
	if (n == 0) {
		printf("%s", MSG[2]);
		clear_buffer();
		return -1;
	}
	n = find_students_by_name(students, result, &name);
	int index;
	for (int i = 0; i < n; ++i) {
		errorMsg = get_intvector(result, i, &index);
		if (errorMsg) {
			destroy_string(command);
			fclose(output_file);
			destroy_students(students);
			destroy_int_vector(result);
			destroy_string(&name);
			return print_error(errorMsg);
		}
		print_student(stream, &((*students).students[index]));
	}
	destroy_string(&name);
	destroy_int_vector(result);
	return 0;
}

int find_by_last_name(FILE *output_file, String *command, Students *students, const char **MSG) {
	int n;
	error_msg errorMsg;
	// Куда выводить
	FILE *stream = stdout;
	int what_stream = to_what_stream(output_file, &stream, MSG);
	if (what_stream == -1) {
		return -1;
	}
	String last_name;

	IntVector *result = create_int_vector(1);
	if (!result) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		return print_error(MEMORY_ALLOCATED_ERROR);
	}
	errorMsg = create_string(&last_name, "");
	if (errorMsg) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		destroy_int_vector(result);
		return print_error(errorMsg);
	}

	getc(stdin);
	printf("Write last name: ");
	n = read_string(stdin, &last_name);
	if (n == 0) {
		printf("%s", MSG[2]);
		clear_buffer();
		return -1;
	}
	n = find_students_by_last_name(students, result, &last_name);
	int index;
	for (int i = 0; i < n; ++i) {
		errorMsg = get_intvector(result, i, &index);
		if (errorMsg) {
			destroy_string(command);
			fclose(output_file);
			destroy_students(students);
			destroy_int_vector(result);
			destroy_string(&last_name);
			return print_error(errorMsg);
		}
		print_student(stream, &((*students).students[index]));
	}
	destroy_string(&last_name);
	destroy_int_vector(result);
	return 0;
}

int find_by_group(FILE *output_file, String *command, Students *students, const char **MSG) {
	int n;
	error_msg errorMsg;
	// Куда выводить
	FILE *stream = stdout;
	int what_stream = to_what_stream(output_file, &stream, MSG);
	if (what_stream == -1) {
		return -1;
	}
	String group;

	IntVector *result = create_int_vector(1);
	if (!result) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		return print_error(MEMORY_ALLOCATED_ERROR);
	}
	errorMsg = create_string(&group, "");
	if (errorMsg) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		destroy_int_vector(result);
		return print_error(errorMsg);
	}

	getc(stdin);
	printf("Write group: ");
	n = read_string(stdin, &group);
	if (n == 0) {
		printf("%s", MSG[2]);
		clear_buffer();
		return -1;
	}
	n = find_students_by_group(students, result, &group);
	int index;
	for (int i = 0; i < n; ++i) {
		errorMsg = get_intvector(result, i, &index);
		if (errorMsg) {
			destroy_string(command);
			fclose(output_file);
			destroy_students(students);
			destroy_int_vector(result);
			destroy_string(&group);
			return print_error(errorMsg);
		}
		print_student(stream, &((*students).students[index]));
	}
	destroy_string(&group);
	destroy_int_vector(result);
	return 0;
}

int find_good_students(FILE *output_file, String *command, Students *students, const char **MSG) {
	int n;
	error_msg errorMsg;
	// Куда выводить
	FILE *stream = stdout;
	int what_stream = to_what_stream(output_file, &stream, MSG);
	if (what_stream == -1) {
		return -1;
	}

	IntVector *result = create_int_vector(1);
	if (!result) {
		destroy_string(command);
		fclose(output_file);
		destroy_students(students);
		return print_error(MEMORY_ALLOCATED_ERROR);
	}

	getc(stdin);
	n = filter_students_by_average_score(students, result);
	if (n == 0) {
		printf("%s", MSG[2]);
		clear_buffer();
		return -1;
	}
	int index;
	for (int i = 0; i < n; ++i) {
		errorMsg = get_intvector(result, i, &index);
		if (errorMsg) {
			destroy_string(command);
			fclose(output_file);
			destroy_students(students);
			destroy_int_vector(result);
			return print_error(errorMsg);
		}
		print_student(stream, &((*students).students[index]));
	}
	destroy_int_vector(result);
	return 0;
}

int main(int argc, char **argv) {
	const char *MSG[] = {"Enter the command:\n",
	                     "Unrecognized command\n",
	                     "Incorrect data\n",
	                     "If tou want to write result in the trace file, write 1or 0\n",
	                     "If you need help: write help\nYou should write one of these commands:\n",
	                     "Program can get command with indices\n",
	                     "Write "};

	const char *COMMAND[] = {"menu\n",          "find by id\n",        "find by name\n", "find by last name\n",
	                         "find by group\n", "sort by id\n",        "sort by name\n", "sort by last name\n",
	                         "sort by group\n", "find good students\n", "write"};

	const char *path1;
	const char *path2;
	error_msg errorMsg = GetPaths(argc, argv, &path1, &path2);

	if (errorMsg) {
		return print_error(errorMsg);
	}

	FILE *input_file = fopen(path1, "r");
	if (!input_file) {
		return print_error(INPUT_FILE_ERROR);
	}

	Students students;

	errorMsg = read_students(input_file, &students);
	if (errorMsg) {
		fclose(input_file);
		return print_error(errorMsg);
	}
	fclose(input_file);



	FILE *output_file = fopen(path2, "w");
	if (!output_file) {
		return print_error(OUTPUT_FILE_ERROR);
	}

	String command;
	errorMsg = create_string(&command, "");
	if (errorMsg) {
		fclose(output_file);
		return print_error(errorMsg);
	}


	// Первое сообщение
	printf("%s", MSG[4]);
	for (int i = 0; i < 10; ++i) {
		printf("%d. %s", i + 1, COMMAND[i]);
	}
	printf("%s", MSG[5]);


	while (1) {
		command.size = 0;
		printf("> ");
		int err = read_line(stdin, &command);
		if (!err) break;
		if (string_cmp(command.arr, "\n")) {
			continue;
		}
		if (string_cmp(command.arr, COMMAND[0]) || string_cmp(command.arr, "1\n")) {
			printf("%s", MSG[4]);
			for (int i = 0; i < 10; ++i) {
				printf("%d. %s", i + 1, COMMAND[i]);
			}
			printf("%s", MSG[5]);

			// Поиск по id
		} else if (string_cmp(command.arr, COMMAND[1]) || string_cmp(command.arr, "2\n")) {
			// Куда выводить
			FILE *stream = stdout;
			int what_stream = to_what_stream(output_file, &stream, MSG);
			if (what_stream == -1) {
				continue;
			}

			// Получаем id
			unsigned int required_id;
			printf("%s%s: ", MSG[6], "id");
			int n = scanf("%u", &required_id);
			if (n != 1) {
				printf("%s", MSG[2]);
				clear_buffer();
				continue;
			}
			getc(stdin);
			int index_student = find_student_by_id(&students, required_id);

			// Нет такого
			if (index_student == -1) {
				printf("%s", MSG[2]);
				clear_buffer();
				continue;
			}
			print_student(stream, &(students.students[index_student]));
			double average_score = calculate_average_score_student(&(students.students[index_student]));
			fprintf(stream, "Average score: %f\n", average_score);
			// Поиск имени
		} else if (string_cmp(command.arr, COMMAND[2]) || string_cmp(command.arr, "3\n")) {
			int n = find_by_name(output_file, &command, &students, MSG);
			if (n == -1) continue;
			if (n != 0) return n;
			// Поиск фамилии

		} else if (string_cmp(command.arr, COMMAND[3]) || string_cmp(command.arr, "4\n")) {
			int n = find_by_last_name(output_file, &command, &students, MSG);
			if (n == -1) continue;
			if (n != 0) return n;
			//груп поиск
		} else if (string_cmp(command.arr, COMMAND[4]) || string_cmp(command.arr, "5\n")) {
			int n = find_by_group(output_file, &command, &students, MSG);
			if (n == -1) continue;
			if (n != 0) return n;

			//    Сортировка по id
		} else if (string_cmp(command.arr, COMMAND[5]) || string_cmp(command.arr, "6\n")) {
			// Куда выводить
			FILE *stream = stdout;
			int what_stream = to_what_stream(output_file, &stream, MSG);
			if (what_stream == -1) {
				continue;
			}
			getc(stdin);
			sort_by_id(&students);
			print_students(stream, &students);
			//	Сортировка по имени
		} else if (string_cmp(command.arr, COMMAND[6]) || string_cmp(command.arr, "7\n")) {
			// Куда выводить
			FILE *stream = stdout;
			int what_stream = to_what_stream(output_file, &stream, MSG);
			if (what_stream == -1) {
				continue;
			}
			getc(stdin);
			sort_by_name(&students);
			print_students(stream, &students);
			//сортировка по фамилии
		} else if (string_cmp(command.arr, COMMAND[7]) || string_cmp(command.arr, "8\n")) {
			// Куда выводить
			FILE *stream = stdout;
			int what_stream = to_what_stream(output_file, &stream, MSG);
			if (what_stream == -1) {
				continue;
			}
			getc(stdin);
			sort_by_last_name(&students);
			print_students(stream, &students);
			//Сортировка по группе
		} else if (string_cmp(command.arr, COMMAND[8]) || string_cmp(command.arr, "9\n")) {
			// Куда выводить
			FILE *stream = stdout;
			int what_stream = to_what_stream(output_file, &stream, MSG);
			if (what_stream == -1) {
				continue;
			}
			getc(stdin);
			sort_by_group(&students);
			print_students(stream, &students);
			//Фильтровка по среднему баллу
		} else if (string_cmp(command.arr, COMMAND[9]) || string_cmp(command.arr, "10\n")) {
			int n = find_good_students(output_file, &command, &students, MSG);
			if (n == -1) {
				continue;
			}
			if (n != 0) return n;
		} else {
			printf("%s", MSG[1]);
		}
	}
	destroy_string(&command);
	fclose(output_file);
	destroy_students(&students);
}