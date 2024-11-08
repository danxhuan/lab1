#include "7.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>



error_msg create_node(Node **node, Liver data) {
	*node = (Node *)calloc(1, sizeof(Node));
	if (!*node) {
		return MEMORY_ALLOCATED_ERROR;
	}
	(*node)->data = data;
	return SUCCESS;
}


void destroy_node(Node ** head){
	destroy_liver(&((*head)->data));
	free(*head);
	*head = NULL;
}

void delete_node(Node *prev, Node *cur) {
	prev->next = cur->next;
	destroy_string(&(cur->data.name));
	destroy_string(&(cur->data.last_name));
	destroy_string(&(cur->data.patronymic));
	cur->next = NULL;
	free(cur);
}

void destroy_LinkedList(Node *head) {
	Node *tmp;
	while (head) {
		tmp = head;
		head = head->next;
		destroy_liver(&(tmp->data));
		tmp->next = NULL;
		free(tmp);
	}
}

error_msg push_node_end(Node **head, Liver data) {
	Node *tmp = *head;
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

error_msg push_node_start(Node **head, Liver data) {
	Node *t;
	error_msg errorMsg = create_node(&t, data);
	if (errorMsg) {
		return errorMsg;
	}
	t->next = (*head);
	*head = t;
	return SUCCESS;
}

Node *find_node_by_index(Node *head, int index) {
	Node *moving_head = head;
	int i = 0;
	while (moving_head && i++ < index) {
		moving_head = moving_head->next;
	}
	if (moving_head) {
		return moving_head;
	}
	return NULL;
}

void print_liver(FILE * stream, Liver * liver){
	print_string(stream, &(liver->last_name), "");
	putc(' ', stream);
	print_string(stream, &(liver->name), "");
	putc(' ', stream);
	if(liver->patronymic.size > 0){
		print_string(stream, &(liver->patronymic), "");
		putc(' ', stream);
	}
	fprintf(stream, "%d.%d.%d %c %f\n", liver->birthday.day, liver->birthday.month,
	        liver->birthday.year, liver->gender, liver->income);
}

void print_LinkedList(FILE *stream, Node *head, char *separator) {
	while (head) {
		print_liver(stream, &(head->data));
		printf("%s", separator);
		head = head->next;
	}
}

unsigned int DayInMonth(unsigned int month, unsigned int year) {
	int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
		return 29;
	}
	return days[month - 1];
}

int IsCorrectDate(Date *time) {
	if (time->year > 9999 || time->month < 1 || time->month > 12 ||
	    time->day > DayInMonth(time->month, time->year))
		return 0;

	return 1;
}

int BiggerDate(Date *t1, Date *t2) {
	if (t1->year > t2->year || (t1->year == t2->year && t1->month > t2->month) ||
	    (t1->year == t2->year && t1->month == t2->month && t1->day > t2->day))
		return 0;
	return 1;
}

int IsCorrectNameLastName(String *s) {
	for (int i = 0; i < s->size; ++i) {
		if (!((s->arr[i] >= 'a' && s->arr[i] <= 'z') || (s->arr[i] >= 'A' && s->arr[i] <= 'Z'))) {
			return 0;
		}
	}
	return 1;
}

error_msg read_livers_from_file(FILE *in, Node **head) {
	int n = 0;
	error_msg errorMsg;
	do {
		Liver liver;
		errorMsg = create_liver(&(liver));
		if (errorMsg) {
			return errorMsg;
		}
		n = read_liver(in, &(liver));
		if (n == 0) {
			destroy_liver(&liver);
			break;
		}
		if (n != 6 || !IsCorrectNameLastName(&(liver.name)) || !IsCorrectNameLastName(&(liver.last_name)) ||
		    !IsCorrectNameLastName(&(liver.patronymic)) || !IsCorrectDate(&(liver.birthday)) ||
		    (liver.gender != 'W' && liver.gender != 'M')) {
			destroy_LinkedList(*head);
			destroy_liver(&liver);
			return INCORRECT_OPTIONS_ERROR;
		}
//		print_liver(stdout, &liver);
		errorMsg = push_node_in_correct_order(head, &liver);
		if (errorMsg) {
			destroy_liver(&liver);
			return errorMsg;
		}

	} while (1);
	return SUCCESS;
}

error_msg push_node_in_correct_order(Node **head, Liver *cur) {
	Node *moving_head = *head;
	error_msg errorMsg;
	if (!moving_head) {
		errorMsg = create_node(head, *cur);
		if (errorMsg) {
			return errorMsg;
		}
	}else if(!BiggerDate(&(moving_head->data.birthday), &(cur->birthday))){
		Node * node;
		errorMsg = create_node(&node, *cur);
		if(errorMsg){
			return errorMsg;
		}
		node->next = moving_head;
		*head = node;

	} else {
		while (moving_head->next && BiggerDate(&(moving_head->next->data.birthday), &(cur->birthday))) {
			moving_head = moving_head->next;
		}
		Node * node;
		errorMsg = create_node(&node, *cur);
		if(errorMsg){
			return errorMsg;
		}
		node->next = moving_head->next;
		moving_head->next = node;
	}
	return SUCCESS;
}

int read_liver(FILE *stream, Liver *liver) {
	int n;
	int count = 0;
	n = read_string(stream, &(liver->last_name));
	if (n == 0) {
		return 0;
	}
	count += 1;

	n = read_string(stream, &(liver->name));
	if (n == 0) {
		return count;
	}
	count += 1;

	n = read_string(stream, &(liver->patronymic));
	if (n == 0) {
		return count;
	}

	count += 1;
	if (liver->patronymic.arr[0] >= '0' && liver->patronymic.arr[0] <= '9') {
		n = sscanf(liver->patronymic.arr, "%d.%d.%d", &(liver->birthday.day), &(liver->birthday.month),
		           &(liver->birthday.year));
		if (n != 3) {
			return count;
		}
		clear_string(&(liver->patronymic));
		count += 1;
	} else {
		n = fscanf(stream, "%d.%d.%d", &(liver->birthday.day), &(liver->birthday.month), &(liver->birthday.year));
		if (n != 3) {
			return count;
		}
		count += 1;
	}

	char c;
	while ((c = getc(stream)) != EOF && c == ' ');
	if (c == EOF || (c != 'W' && c != 'M')) {
		return count;
	}
	liver->gender = c;
	count += 1;

	n = fscanf(stream, "%lf", &(liver->income));
	if (n != 1 || liver->income < 0) {
		return count;
	}
	count += 1;

	if(!feof(stream)){
		getc(stream);
	}
	return count;
}

error_msg create_liver(Liver *liver) {
	error_msg errorMsg = create_string(&(liver->last_name), "");
	if (errorMsg) {
		return errorMsg;
	}

	errorMsg = create_string(&(liver->name), "");
	if (errorMsg) {
		destroy_string(&(liver->last_name));
		return errorMsg;
	}

	errorMsg = create_string(&(liver->patronymic), "");
	if (errorMsg) {
		destroy_string(&(liver->last_name));
		destroy_string(&(liver->name));
		return errorMsg;
	}
	return SUCCESS;
}

void destroy_liver(Liver *liver) {
	destroy_string(&(liver->last_name));
	destroy_string(&(liver->name));
	destroy_string(&(liver->patronymic));
}


int eq_livers(Liver * l1, Liver* l2){
	if(strings_equals(&(l1->last_name), &(l2->last_name)) && strings_equals(&(l1->name), &(l2->name)) &&
	    strings_equals(&(l1->patronymic), &(l2->patronymic)) && l1->gender == l2->gender && l1->birthday.day == l2->birthday.day &&
	    l1->birthday.month == l2->birthday.month && l1->birthday.year == l2->birthday.year && l1->income == l2->income)
		return 1;
	return 0;
}

error_msg delete_liver(Node ** head, Liver * liver){
	Node *moving_head = *head;
	if(eq_livers(&(moving_head->data), liver)){
		Node * tmp = *head;
		*head = (*head)->next;
		destroy_node(&tmp);
		return SUCCESS;

	}else{
		while (moving_head->next) {
			if (eq_livers(&(moving_head->next->data), liver)) {
				Node * tmp = moving_head->next;
				moving_head->next = moving_head->next->next;
				destroy_node(&tmp);
				return SUCCESS;
			}
			moving_head = moving_head->next;
		}
	}
	return INCORRECT_OPTIONS_ERROR;
}


Node *find(Node ** head, Liver * liver){
	Node * moving_head = *head;
	while (moving_head){
		if(eq_livers(&(moving_head->data), liver)){
			return moving_head;
		}
		moving_head = moving_head->next;
	}
	return NULL;
}

error_msg change_liver(Node ** head, Liver * liver, Liver * new_liver){
	Node * node = find(head, liver);
	if(!node){
		return INCORRECT_OPTIONS_ERROR;
	}
	node->data = *new_liver;
	return SUCCESS;
}
void clear_buffer(){
	while ( getchar() != '\n' );
}

error_msg create_special_node(SpecialNode **node, Liver data, int instruction) {
	*node = (SpecialNode *)calloc(1, sizeof(SpecialNode));
	if (!*node) {
		return MEMORY_ALLOCATED_ERROR;
	}
	(*node)->liver = data;
	(*node)->instruction = instruction;
	return SUCCESS;
}

void destroy_special_node(SpecialNode ** head){
	destroy_liver(&((*head)->liver));
	free(*head);
	*head = NULL;
}


void destroy_LinkedList_with_special_node(SpecialNode *head) {
	SpecialNode *tmp;
	while (head) {
		tmp = head;
		head = head->next;
		destroy_special_node(&tmp);
	}
}


error_msg push_special_node_start(SpecialNode ** head, Liver data, int instruction){
	SpecialNode *t;
	error_msg errorMsg = create_special_node(&t, data, instruction);
	if (errorMsg) {
		return errorMsg;
	}
	if(!*head){
		*head = t;
	}
	else{
		t->next = *head;
		*head = t;
	}
	return SUCCESS;
}


error_msg copy_liver(const Liver * dst, Liver * src){
	error_msg errorMsg = create_liver(src);
	if(errorMsg){
		return errorMsg;
	}
	errorMsg = mstrcopy(&(dst->last_name),&( src->last_name), 0, dst->last_name.size);
	if(errorMsg){
		destroy_liver(src);
		return errorMsg;
	}
	errorMsg = mstrcopy(&(dst->name),&( src->name), 0, dst->name.size);
	if(errorMsg){
		destroy_liver(src);
		return errorMsg;
	}

	errorMsg = mstrcopy(&(dst->patronymic),&(src->patronymic), 0, dst->patronymic.size);
	if(errorMsg){
		destroy_liver(src);
		return errorMsg;
	}
	src->birthday = dst->birthday;
	src->gender = dst->gender;
	src->income = dst->income;
	return SUCCESS;

}
