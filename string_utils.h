#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

int eb_sprintf(char *output, char *input, ...);

int get_digits_i(char* buffer, int number);

int get_digits_l(char* buffer, unsigned long number);

int get_digits_f(char* buffer, float number);

void str_cp(char *from, char*to, int index);

int str_len(char *str);
#endif

