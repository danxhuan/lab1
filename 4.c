#include "4.h"

error_msg create_address_ptr(Address **adrs, char *city, char *street, int house, char *building, int apartment,
                             char *index) {

	if(house <= 0 || apartment <= 0) return INCORRECT_OPTIONS_ERROR;

	*adrs = (Address *)malloc(sizeof(Address));
	Address *address = *adrs;
	error_msg errorMsg = create_address(address, city, street, house, building, apartment, index);
	if(errorMsg) {
		free(*adrs);
		return errorMsg;
	}

	return SUCCESS;
}

void destroy_address_ptr(Address *address) {
	destroy_address(address);
	free(address);
}

void destroy_address(Address *address) {
	destroy_string(&(address->city));
	destroy_string(&(address->street));
	destroy_string(&(address->building));
	destroy_string(&(address->index));
}

void print_address(FILE *stream, Address *address) {
	print_string(stream, &(address->city), "");
	fprintf(stream, " ");
	print_string(stream, &(address->street), "");
	fprintf(stream, " %d ", address->house);
	print_string(stream, &(address->building), "");
	fprintf(stream, " %d ", address->apartment);
	print_string(stream, &(address->index), "");
}

error_msg create_post(Post *post, int capacity_mails, char *city, char *street, int house, char *building,
                      int apartment, char *index) {
	error_msg errorMsg = create_address_ptr(&post->address, city, street, house, building, apartment, index);
	if (errorMsg) return errorMsg;
	post->mails = (Mail *)malloc(sizeof(Mail) * capacity_mails);
	if (!post->mails) return MEMORY_ALLOCATED_ERROR;
	return SUCCESS;
}

void print_post(FILE *stream, Post *post, int count_mails) {
	fprintf(stream, "Post office: ");
	print_address(stream, post->address);
	fprintf(stream, "\nCount mails: %d\n", count_mails);
	for (int i = 0; i < count_mails; ++i) {
		print_mail(stream, &(post->mails[i]));
	}
}

void destroy_post(Post *post, const int count_mails) {
	for (int i = 0; i < count_mails; ++i) {
		destroy_mail(&(post->mails[i]));
	}
	free(post->mails);
	destroy_address_ptr(post->address);
}

error_msg create_address(Address *address, char *city, char *street, int house, char *building, int apartment,
                         char *index) {

	if(house <= 0 || apartment <= 0) return INCORRECT_OPTIONS_ERROR;

	error_msg errorMsg = create_string(&(address->city), city);

	if (errorMsg) return errorMsg;
	errorMsg = create_string(&(address->street), street);
	if (errorMsg) {
		destroy_string(&(address->city));
		return errorMsg;
	}
	address->house = house;
	errorMsg = create_string(&(address->building), building);
	if (errorMsg) {
		destroy_string(&(address->city));
		destroy_string(&(address->street));
		return errorMsg;
	}
	address->apartment = apartment;
	errorMsg = create_string(&(address->index), index);
	if (errorMsg) {
		destroy_string(&(address->city));
		destroy_string(&(address->street));
		destroy_string(&(address->building));
		return errorMsg;
	}
	return SUCCESS;
}

int IsNumberString(char *s) {
	while (*s != '\0') {
		if (*s < '0' || *s > '9') return 0;
		++s;
	}
	return 1;
}

int DayInMonth(int month, int year) {
	int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
		return 29;
	}
	return days[month - 1];
}

int IsCorrectTime(Time *time) {
	if (time->year < 0 || time->year > 9999 || time->month < 1 || time->month > 12 || time->day < 0 ||
	    time->day > DayInMonth(time->month, time->year))
		return 0;
	if (time->hour < 0 || time->hour >= 24 || time->minute < 0 || time->minute >= 60 || time->sec < 0 ||
	    time->sec >= 60)
		return 0;

	return 1;
}

int BiggerTime(Time *t1, Time *t2) {
	if (t1->year > t2->year || (t1->year == t2->year && t1->month > t2->month) ||
	    (t1->year == t2->year && t1->month == t2->month && t1->day > t2->day) ||
	    (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day && t1->hour > t2->hour) ||
	    (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day && t1->hour == t2->hour &&
	     t1->minute > t2->minute) ||
	    (t1->year == t2->year && t1->month == t2->month && t1->day == t2->day && t1->hour == t2->hour &&
	     t1->minute == t2->minute && t1->sec > t2->sec))
		return 0;
	return 1;
}

error_msg create_mail(Mail *mail, char *city, char *street, int house, char *building, int apartment, char *index,
                      double parcel_weight, char *mail_id, Time mail_create_time, Time mail_receipt_time) {

	error_msg errorMsg = create_address((&(*mail).address), city, street, house, building, apartment, index);
	if (errorMsg) return errorMsg;

	// Проверяем на соответствие
	if (SizeString(index) != 6 || parcel_weight < 0 || !IsNumberString(index) || !IsNumberString(mail_id) ||
	    SizeString(mail_id) != 14 || !IsCorrectTime(&mail_create_time) || !IsCorrectTime(&mail_receipt_time) ||
	    !BiggerTime(&mail_create_time, &mail_receipt_time)) {
		destroy_address(&(mail->address));
		return INCORRECT_OPTIONS_ERROR;
	}

	mail->parcel_weight = parcel_weight;
	if(parcel_weight < 1e-16){
		destroy_address(&(mail->address));
		return INCORRECT_OPTIONS_ERROR;
	}
	char tmp[21];
	tmp[0] = '\0';
	sprintf(tmp, "%02d:%02d:%04d %02d:%02d:%02d", mail_create_time.day, mail_create_time.month, mail_create_time.year,
	        mail_create_time.hour, mail_create_time.minute, mail_create_time.sec);

	errorMsg = create_string(&(mail->mail_create_time), tmp);

	if (errorMsg) {
		destroy_address(&(mail->address));
		return errorMsg;
	}
	tmp[0] = '\0';
	sprintf(tmp, "%02d:%02d:%04d %02d:%02d:%02d", mail_receipt_time.day, mail_receipt_time.month,
	        mail_receipt_time.year, mail_receipt_time.hour, mail_receipt_time.minute, mail_receipt_time.sec);

	errorMsg = create_string(&(mail->mail_receipt_time), tmp);

	if (errorMsg) {
		destroy_address(&(mail->address));
		destroy_string(&(mail->mail_create_time));
		return errorMsg;
	}
	errorMsg = create_string(&(mail->mail_id), mail_id);
	if (errorMsg) {
		destroy_address(&(mail->address));
		destroy_string(&(mail->mail_create_time));
		destroy_string(&(mail->mail_receipt_time));
		return errorMsg;
	}
	return SUCCESS;
}

void destroy_mail(Mail *mail) {
	destroy_address(&(mail->address));
	destroy_string(&(mail->mail_create_time));
	destroy_string(&(mail->mail_receipt_time));
	destroy_string(&(mail->mail_id));
}

void print_mail(FILE *stream, Mail *mail) {
	printf("---------------------------------------------------\nMail: ");
	print_string(stream, &(mail->mail_id), "");
	printf(" \nAddress: ");
	print_address(stream, &(mail->address));
	printf(" \nCreate time: ");
	print_string(stream, &(mail->mail_create_time), "");
	printf(" \nReceipt time: ");
	print_string(stream, &(mail->mail_receipt_time), "");
	printf("\n---------------------------------------------------\n");
}

error_msg push_mail_into_post(Post *post, int *count_mails, int *size_post, Mail *mail) {
	for(int i = 0; i < *count_mails; ++i){
		if(strings_equals(&(mail->mail_id), &(post->mails[i].mail_id))){
			destroy_mail(mail);
			return INCORRECT_OPTIONS_ERROR;
		}
	}
	if (*count_mails == *size_post) {
		Mail *tmp = (Mail *)realloc(post->mails, *size_post * 2 * sizeof(Mail));
		*size_post *= 2;
		if (!tmp) {
			destroy_post(post, *count_mails);
			return MEMORY_ALLOCATED_ERROR;
		}
		post->mails = tmp;
	}

	int i = 0;
	while (i < *count_mails &&
	       (string_to_int(&(post->mails[i].address.index)) < string_to_int(&(mail->address.index)) ||
	        (string_to_int(&(post->mails[i].address.index)) == string_to_int(&(mail->address.index)) &&
	         string_to_int(&(post->mails[i].mail_id)) < string_to_int(&(mail->mail_id))))) {
		i++;
	}
	(*count_mails)++;
	for (int j = *count_mails - 1; j > i; --j) {
		post->mails[j] = post->mails[j - 1];
	}

	post->mails[i] = *mail;
	return SUCCESS;
}

error_msg find_mail_in_post(Post *post, Mail *mail, String *mail_id, int count_mails, int *index) {
	for (int i = 0; i < count_mails; ++i) {
		if (strings_equals(&(post->mails[i].mail_id), mail_id)) {
			*mail = (post->mails[i]);
			*index = i;
			return SUCCESS;
		}
	}
	return INCORRECT_OPTIONS_ERROR;
}

error_msg delete_mail_in_post(Post *post, String *mail_id, int *count_mails) {
	int index = 0;
	Mail mail;

	error_msg errorMsg = find_mail_in_post(post, &mail, mail_id, *count_mails, &index);

	if (errorMsg) return errorMsg;

	destroy_mail(&(post->mails[index]));
	for (int i = index; i < *count_mails - 1; ++i) {
		post->mails[i] = post->mails[i + 1];
	}
	(*count_mails)--;
	return SUCCESS;
}

int comp_bigger( const void * val1, const void * val2 ){
	Mail *mail1 = (Mail*)val1;
	Mail *mail2 = (Mail*)val2;
	Time t1;
	Time t2;
	sscanf(mail1->mail_create_time.arr, "%02d:%02d:%04d %02d:%02d:%02d", &(t1.day), &(t1.month),
	       &(t1.year), &(t1.hour), &(t1.minute), &(t1.sec));
	sscanf(mail2->mail_create_time.arr, "%02d:%02d:%04d %02d:%02d:%02d", &(t2.day), &(t2.month),
	       &(t2.year), &(t2.hour), &(t2.minute), &(t2.sec));
	if(BiggerTime(&t1, &t2)){
		return 1;
	}
	return -1;
}

int comp_less( const void * val1, const void * val2 ){
	Mail *mail1 = (Mail*)val1;
	Mail *mail2 = (Mail*)val2;
	Time t1;
	Time t2;
	sscanf(mail1->mail_create_time.arr, "%02d:%02d:%04d %02d:%02d:%02d", &(t1.day), &(t1.month),
	       &(t1.year), &(t1.hour), &(t1.minute), &(t1.sec));
	sscanf(mail2->mail_create_time.arr, "%02d:%02d:%04d %02d:%02d:%02d", &(t2.day), &(t2.month),
	       &(t2.year), &(t2.hour), &(t2.minute), &(t2.sec));
	if(BiggerTime(&t1, &t2)){
		return -1;
	}
	return 1;
}

error_msg find_received_mails(Post *post, int count_mail) {
	time_t real_time = time(NULL);
	struct tm *now = localtime(&real_time);
	Time r_time = {now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec};
	Mail * received_mails = (Mail *)malloc(sizeof(Mail) * count_mail);
	if (!received_mails) return MEMORY_ALLOCATED_ERROR;
	int count_received_mails = 0;
	for (int i = 0; i < count_mail; ++i) {
		Time mail_time;
		sscanf(post->mails[i].mail_receipt_time.arr, "%02d:%02d:%04d %02d:%02d:%02d", &(mail_time.day), &(mail_time.month),
		       &(mail_time.year), &(mail_time.hour), &(mail_time.minute), &(mail_time.sec));
		if(BiggerTime(&mail_time, &r_time)){
			received_mails[count_received_mails++] = post->mails[i];
		}
	}
	qsort(received_mails, count_received_mails, sizeof(Mail), comp_bigger);
	for(int i = 0; i < count_received_mails; ++i){
		print_mail(stdout, &(received_mails[i]));
	}
	free(received_mails);
	return SUCCESS;
}


error_msg find_not_received_mails(Post *post, int count_mail) {
	time_t real_time = time(NULL);
	struct tm *now = localtime(&real_time);
	Time r_time = {now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec};
	Mail * received_mails = (Mail *)malloc(sizeof(Mail) * count_mail);
	if (!received_mails) return MEMORY_ALLOCATED_ERROR;
	int count_received_mails = 0;
	for (int i = 0; i < count_mail; ++i) {
		Time mail_time;
		sscanf(post->mails[i].mail_receipt_time.arr, "%02d:%02d:%04d %02d:%02d:%02d", &(mail_time.day), &(mail_time.month),
		       &(mail_time.year), &(mail_time.hour), &(mail_time.minute), &(mail_time.sec));
		if(!BiggerTime(&mail_time, &r_time)){
			received_mails[count_received_mails++] = post->mails[i];
		}
	}
	qsort(received_mails, count_received_mails, sizeof(Mail), comp_less);
	for(int i = 0; i < count_received_mails; ++i){
		print_mail(stdout, &(received_mails[i]));
	}
	free(received_mails);
	return SUCCESS;
}


error_msg scan_email(Mail * mail){
	char city[100];
	char street[100];
	int house;
	char building[100];
	int apartment;
	char index[100];
	double weight_parcel;
	char mail_id[100];
	Time create_time;
	Time receipt_time;
	int n = scanf("%s %s %d %s %d %s %lf %s %d:%d:%d %d:%d:%d %d:%d:%d %d:%d:%d", city, street, &house,
	              building, &apartment, index, &weight_parcel, mail_id, &(create_time.day),
	              &(create_time.month), &(create_time.year), &(create_time.hour), &(create_time.minute),
	              &(create_time.sec), &(receipt_time.day), &(receipt_time.month), &(receipt_time.year),
	              &(receipt_time.hour), &(receipt_time.minute), &(receipt_time.sec));
	if (n != 20) {
		return INCORRECT_OPTIONS_ERROR;
	}
	getc(stdin);
	return create_mail(mail, city, street, house, building, apartment, index, weight_parcel, mail_id, create_time, receipt_time);
}

void clear_buffer(){
	while ( getchar() != '\n' );
}