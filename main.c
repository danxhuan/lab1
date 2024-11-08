#include "7.h"
#include "7.c"

int FindLiver(Node** head, const char* MSG[]) {
	Liver liver;
	error_msg errorMsg;
	int n;
	errorMsg = create_liver(&liver);
	if (errorMsg) {
		clear_buffer();
		return errorMsg;
	}
	printf("%s", MSG[3]);
	n = read_liver(stdin, &liver);
	if (n != 6 || !IsCorrectNameLastName(&(liver.name)) || !IsCorrectNameLastName(&(liver.last_name)) ||
	    !IsCorrectNameLastName(&(liver.patronymic)) || !IsCorrectDate(&(liver.birthday)) ||
	    (liver.gender != 'W' && liver.gender != 'M')) {
		printf("%s", MSG[7]);
		destroy_liver(&liver);
		return -1;
	}

	Node* find_liver = find(head, &liver);
	if (!find_liver) {
		printf("%s", MSG[7]);
		destroy_liver(&liver);
		return -1;
	}
	print_liver(stdout, &(find_liver->data));
	destroy_liver(&liver);
	return SUCCESS;
}

int DeleteLiver(Node** head, const char* MSG[], SpecialNode** head_stack) {
	Liver liver;
	error_msg errorMsg;
	int n;
	errorMsg = create_liver(&liver);
	if (errorMsg) {
		clear_buffer();
		return errorMsg;
	}
	printf("%s", MSG[3]);
	n = read_liver(stdin, &liver);
	if (n != 6 || !IsCorrectNameLastName(&(liver.name)) || !IsCorrectNameLastName(&(liver.last_name)) ||
	    !IsCorrectNameLastName(&(liver.patronymic)) || !IsCorrectDate(&(liver.birthday)) ||
	    (liver.gender != 'W' && liver.gender != 'M')) {
		clear_buffer();
		printf("%s", MSG[7]);
		destroy_liver(&liver);
		return -1;
	}
	errorMsg = delete_liver(head, &liver);
	if (errorMsg == INCORRECT_OPTIONS_ERROR) {
		printf("%s", MSG[7]);
		destroy_liver(&liver);
		return -1;
	} else if (errorMsg) {
		clear_buffer();
		printf("%s", MSG[7]);
		return errorMsg;
	}
	errorMsg = push_special_node_start(head_stack, liver, 0);
	if (errorMsg) {
		clear_buffer();
		printf("%s", MSG[7]);
		return errorMsg;
	}

	return SUCCESS;
}

int download_to_file(Node* head, const char* MSG[]) {
	error_msg errorMsg;
	String out_filename;
	errorMsg = create_string(&out_filename, "");
	if (errorMsg) {
		return errorMsg;
	}
	printf("%s", MSG[6]);
	read_string(stdin, &out_filename);
	FILE* out = fopen(out_filename.arr, "w");
	if (!out) {
		destroy_string(&out_filename);
		return OUTPUT_FILE_ERROR;
	}
	print_LinkedList(out, head, "");
	destroy_string(&out_filename);
	fclose(out);
	return SUCCESS;
}

int revert_livers(Node** head, SpecialNode** stack, const int n) {
	SpecialNode* moving_head = *stack;
	error_msg errorMsg;
	Liver new_liver;
	Liver last_liver;
	for (int i = 0; i < n / 2 + 1; ++i) {
		switch (moving_head->instruction) {
			case 0:
				errorMsg = copy_liver(&(moving_head->liver), &last_liver);
				if (errorMsg) {
					return errorMsg;
				}
				errorMsg = push_node_in_correct_order(head, &last_liver);
				if (errorMsg) {
					return errorMsg;
				}
				break;
			case 1:
				errorMsg = delete_liver(head, &(moving_head->liver));
				if (errorMsg) {
					return errorMsg;
				}
				break;
			case 2:
				errorMsg = copy_liver(&(moving_head->liver), &last_liver);
				if (errorMsg) {
					return errorMsg;
				}
				moving_head = moving_head->next;
				errorMsg = copy_liver(&(moving_head->liver), &new_liver);
				if (errorMsg) {
					destroy_liver(&last_liver);
					destroy_liver(&new_liver);
					return errorMsg;
				}
				errorMsg = delete_liver(head, &last_liver);
				if (errorMsg) {
					destroy_liver(&last_liver);
					destroy_liver(&new_liver);
					return errorMsg;
				}
				errorMsg = push_node_in_correct_order(head, &new_liver);
				if (errorMsg) {
					destroy_liver(&last_liver);
					return errorMsg;
				}
				destroy_liver(&last_liver);
				break;
			default:
				return INCORRECT_OPTIONS_ERROR;
		}
		moving_head = moving_head->next;
	}
	return SUCCESS;
}

int AddLiver(Node** head, const char* MSG[], SpecialNode** stack) {
	Liver liver;
	error_msg errorMsg;
	int n;
	errorMsg = create_liver(&liver);
	if (errorMsg) {
		clear_buffer();
		return errorMsg;
	}
	printf("%s", MSG[3]);
	n = read_liver(stdin, &liver);
	if (n != 6 || !IsCorrectNameLastName(&(liver.name)) || !IsCorrectNameLastName(&(liver.last_name)) ||
	    !IsCorrectNameLastName(&(liver.patronymic)) || !IsCorrectDate(&(liver.birthday)) ||
	    (liver.gender != 'W' && liver.gender != 'M')) {
		printf("%s", MSG[7]);
		destroy_liver(&liver);
		return -1;
	}
	errorMsg = push_node_in_correct_order(head, &liver);
	if (errorMsg) {
		destroy_liver(&liver);
		return errorMsg;
	}

	Liver liver1;
	errorMsg = copy_liver(&liver, &liver1);
	if (errorMsg) {
		destroy_liver(&liver);
		return errorMsg;
	}

	errorMsg = push_special_node_start(stack, liver1, 1);
	if (errorMsg) {
		destroy_liver(&liver);
		destroy_liver(&liver1);
		return errorMsg;
	}
	return SUCCESS;
}

int Change(Node** head, const char* MSG[], SpecialNode** stack) {
	Liver old_liver, new_liver;
	error_msg errorMsg;
	int n;
	errorMsg = create_liver(&old_liver);
	if (errorMsg) {
		return errorMsg;
	}
	printf("%s", MSG[3]);
	n = read_liver(stdin, &old_liver);
	if (n != 6 || !IsCorrectNameLastName(&(old_liver.name)) || !IsCorrectNameLastName(&(old_liver.last_name)) ||
	    !IsCorrectNameLastName(&(old_liver.patronymic)) || !IsCorrectDate(&(old_liver.birthday)) ||
	    (old_liver.gender != 'W' && old_liver.gender != 'M')) {
		printf("%s", MSG[7]);
		destroy_liver(&old_liver);
		return -1;
	}


	errorMsg = create_liver(&new_liver);
	if (errorMsg) {
		destroy_liver(&old_liver);
		return errorMsg;
	}
	printf("%s", MSG[8]);
	n = read_liver(stdin, &new_liver);
	if (n != 6 || !IsCorrectNameLastName(&(new_liver.name)) || !IsCorrectNameLastName(&(new_liver.last_name)) ||
	    !IsCorrectNameLastName(&(new_liver.patronymic)) || !IsCorrectDate(&(new_liver.birthday)) ||
	    (new_liver.gender != 'W' && new_liver.gender != 'M')) {
		printf("%s", MSG[7]);
		destroy_liver(&new_liver);
		destroy_liver(&old_liver);
		return -1;
	}


	errorMsg = delete_liver(head, &old_liver);
	if (errorMsg) {
		printf("%s", MSG[7]);
		destroy_liver(&new_liver);
		destroy_liver(&old_liver);
		return -1;
	}
	errorMsg = push_node_in_correct_order(head, &new_liver);
	if (errorMsg) {
		destroy_liver(&new_liver);
		destroy_liver(&old_liver);
		return errorMsg;
	}

	errorMsg = push_special_node_start(stack, old_liver, 2);
	if (errorMsg) {
		destroy_liver(&new_liver);
		destroy_liver(&old_liver);
		return errorMsg;
	}

	Liver new_liver_copy;
	errorMsg = copy_liver(&new_liver, &new_liver_copy);
	if(errorMsg){
		destroy_liver(&new_liver);
		destroy_liver(&old_liver);
		return errorMsg;
	}

	errorMsg = push_special_node_start(stack, new_liver_copy, 2);
	if (errorMsg) {
		destroy_liver(&new_liver);
		destroy_liver(&old_liver);
		return errorMsg;
	}
	return SUCCESS;
}

int main() {
	const char* MSG[] = {"Write command\n",
	                     "Incorrect command\n",
	                     "Write the find liver",
	                     "Write the liver: ",
	                     "If you need help, write: help\n",
	                     "Program can get command with indices\n",
	                     "Write filename: ",
	                     "Incorrect data\n",
	                     "Write the new liver: "};
	const char* COMMANDS[] = {"help\n",      "find liver\n", "delete liver\n",        "change liver\n",
	                          "add liver\n", "undo\n",       "Download to the file\n"};

	// Читаем файл
	String filename;
	error_msg errorMsg = create_string(&filename, "");
	if (errorMsg) {
		return print_error(errorMsg);
	}

	printf("Write name of input file: ");
	read_string(stdin, &filename);
	FILE* in = fopen(filename.arr, "r");
	if (!in) {
		destroy_string(&filename);
		return print_error(INPUT_FILE_ERROR);
	}

	// Записываем все в односвязный список
	Node* head = NULL;
	errorMsg = read_livers_from_file(in, &head);
	if (errorMsg) {
		fclose(in);
		destroy_string(&filename);
		return print_error(errorMsg);
	}
	fclose(in);
	destroy_string(&filename);

	// Создаем строку команды
	int count_n = 0;
	String command;
	errorMsg = create_string(&command, "");
	if (errorMsg) {
		destroy_LinkedList(head);
		return print_error(errorMsg);
	}

	// Создаем специальный стек для хранения команд
	SpecialNode* special_stack = NULL;

	printf("%s\n", MSG[4]);
	while (1) {
		clear_string(&command);
		printf("> ");
		int n = read_line(stdin, &command);
		if (string_cmp(command.arr, "\n")) {
			continue;
		}
		if (n == 0) {
			break;
		}

		if (string_cmp(command.arr, COMMANDS[0]) || string_cmp(command.arr, "1\n")) {
			printf("You need write one of these commands:\n");
			for (int i = 0; i < 7; ++i) {
				printf("%d. %s", i + 1, COMMANDS[i]);
			}
		}

		else if (string_cmp(command.arr, COMMANDS[1]) || string_cmp(command.arr, "2\n")) {
			int er = FindLiver(&head, MSG);
			if (er == -1) {
				continue;
			} else if (er) {
				destroy_LinkedList_with_special_node(special_stack);
				destroy_LinkedList(head);
				destroy_string(&command);
				return print_error(er);
			}
		}else if (string_cmp(command.arr, COMMANDS[2]) || string_cmp(command.arr, "3\n")) {
			int er = DeleteLiver(&head, MSG, &special_stack);
			if (er == -1) {
				continue;
			} else if (er) {
				destroy_LinkedList_with_special_node(special_stack);
				destroy_LinkedList(head);
				destroy_string(&command);
				return print_error(er);
			}
			count_n += 1;
		}else if (string_cmp(command.arr, COMMANDS[3]) || string_cmp(command.arr, "4\n")) {
			int er = Change(&head, MSG, &special_stack);
			if (er == -1) {
				continue;
			} else if (er) {
				destroy_LinkedList_with_special_node(special_stack);
				destroy_LinkedList(head);
				destroy_string(&command);
				return print_error(er);
			}
			count_n += 1;
		} else if (string_cmp(command.arr, COMMANDS[4]) || string_cmp(command.arr, "5\n")) {
			int er = AddLiver(&head, MSG, &special_stack);
			if (er == -1) {
				continue;
			} else if (er) {
				destroy_LinkedList_with_special_node(special_stack);
				destroy_LinkedList(head);
				destroy_string(&command);
				return print_error(er);
			}
			count_n += 1;
		}

		else if (string_cmp(command.arr, COMMANDS[5]) || string_cmp(command.arr, "6\n")) {
			errorMsg = revert_livers(&head, &special_stack, count_n);
			if (errorMsg) {
				destroy_LinkedList(head);
				destroy_LinkedList_with_special_node(special_stack);
				destroy_string(&command);
				return print_error(errorMsg);
			}
			count_n = 0;
		}

		else if (string_cmp(command.arr, COMMANDS[6]) || string_cmp(command.arr, "7\n")) {
			errorMsg = download_to_file(head, MSG);
			if (errorMsg) {
				destroy_LinkedList_with_special_node(special_stack);
				clear_buffer();
				destroy_LinkedList(head);
				destroy_string(&command);
				return print_error(errorMsg);
			}
		} else {
			printf("%s", MSG[1]);
		}
	}

	destroy_LinkedList(head);
	destroy_LinkedList_with_special_node(special_stack);
	destroy_string(&command);
	return 0;
}