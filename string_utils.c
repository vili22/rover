#include <stdarg.h>
#include "string_utils.h"

static int handle_next_formatter(char *output, char *input, int in_index, int out_index, va_list ap) {

	while(in_index < str_len(input)) {
		if(*(input + in_index) == '%') {
			if(in_index == str_len(input) -1) {
				return -1;
			}
			in_index++;
			if(*(input + in_index)== 'd') {
				int int_num;
				int_num = va_arg(ap, int);
				char digits[20];
				int n_digits = get_digits_i(digits, int_num);
				str_cp(digits, output, out_index);
				out_index += n_digits;
				in_index++;
				return handle_next_formatter(output, input, in_index, out_index, ap);
			} else if(*(input + in_index)== 'l') {
				unsigned long long_num;
				long_num = va_arg(ap, unsigned long);
				char digits[20];
				int n_digits = get_digits_l(digits, long_num);
				str_cp(digits, output, out_index);
				out_index += n_digits;
				in_index++;
				return handle_next_formatter(output, input, in_index, out_index, ap);
			} else if(*(input + in_index) == 'f') {
				float float_num = va_arg(ap, double);
				char digits[20];
				int n_digits = get_digits_f(digits, float_num);
				str_cp(digits, output, out_index);
				out_index += n_digits;
				in_index++;
				return handle_next_formatter(output, input, in_index, out_index, ap);
			}
		} else {
			*(output + out_index) = *(input + in_index);
			in_index++;
			out_index++;
		}
	}
	*(output + out_index) = '\0';
	return out_index;
}

int eb_sprintf(char *output, char *input, ...) {

	va_list ap;
	va_start(ap, input);

	return handle_next_formatter(output, input, 0, 0, ap);
}

int get_digits_i(char* digits, int number) {
	
	char temp_buffer[20];
	int index = 0, is_negative = 0;
	if(number < 0) {
		is_negative = 1;
		number *=-1;
	}

	do {
		temp_buffer[index] = '0' + number%10;
		number = number/10;
		index++;
	} while(number);

	if(is_negative) {
		digits[0] = '-';
	}

	for(int k = 0; k < index ; k++) {
		digits[k + is_negative] = temp_buffer[index-1-k];
	}

	index += is_negative;
	digits[index] = '\0';
	return index;
}

int get_digits_l(char* digits, unsigned long number) {
	
	char temp_buffer[20];
	int index = 0, is_negative = 0;

	do {
		temp_buffer[index] = '0' + number%10;
		number = number/10;
		index++;
	} while(number);

	if(is_negative) {
		digits[0] = '-';
	}

	for(int k = 0; k < index ; k++) {
		digits[k + is_negative] = temp_buffer[index-1-k];
	}

	index += is_negative;
	digits[index] = '\0';
	return index;
}

int get_digits_f(char* digits, float number) {

	int is_negative = 0;
	if(number < 0) {
		number *= -1;
		is_negative = 1;
	}

	int int_part = (int)number;
	char int_digits[20];
	get_digits_i(int_digits, int_part);

	char float_digits[20];
	int float_part = (int)((number - (float)int_part)*1e6);
	get_digits_i(float_digits, float_part);
	//int index = 0;
	//do {
	//   	float_part = float_part * 10;
	//	float_digits[index] = '0' + (int)float_part;
	//	float_part = float_part -(float)(int)float_part;
	//	index++;
	//} while(index < 6);
	//float_digits[index] = '\0';	

	int tot_index = 0;
	if(is_negative) {
		digits[0] = '-';
		tot_index++;
	}

	for(int k = 0; k < str_len(int_digits) ; k++) {
	
		digits[tot_index + k] = int_digits[k];
	}
	tot_index += str_len(int_digits);
	digits[tot_index] = '.';
	tot_index++;
	
	for(int k = 0; k < str_len(float_digits) ; k++) {
	
		digits[tot_index + k] = float_digits[k];
	}
	tot_index += str_len(float_digits);
	digits[tot_index] = '\0';

	return tot_index;
}

void str_cp(char *from, char*to, int index) {

	int ind = 0;
	while(*(from + ind) != '\0') {
		*(to + ind + index) = *(from + ind);
		ind++;		
	}

	*(to + ind + index) = '\0';
}

int str_len(char *str) {

	int ind = 0;
	while(*(str + ind) != '\0') {
		ind++;
	}
	return ind--;
}
