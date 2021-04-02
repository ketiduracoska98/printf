#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// Duracoska Keti 332CB

static int write_stdout(const char *token, int length) {
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

// swap function - used to reverse a string
void swap(char *a, char *b) {
	char aux = *a;
	*a = *b;
	*b = aux;
}

// function that reverses a string
char *reverse(char *buffer, int i, int j) {
	while(i < j) {
		swap(&buffer[i++], &buffer[j--]);
	}
	return buffer;
}

// function which convert unsigned int or hex to string 
char *convert(unsigned int num, int base) {
	// characters that can appear in string 
	static char Representation[]= "0123456789abcdef";
	static char buffer[11];
	int i = 0;
	while(num != 0) {
		// get each digit of the number and convert it to string
		buffer[i++] = Representation[num % base];
		num /= base; 
	} 
	buffer[i] = '\0';
	return reverse(buffer, 0, i - 1);
}  

// convert signed numbers in base 10 to string 
char *convert_signed(int num) {
	static char buffer[11];  
	int i = 0, value = abs(num);
	int int_max = 0;
	// number is bigger than max value of int
	if(value < 0) {
		int_max = 1;
		value--;
	} 
	while(value != 0) {
		buffer[i++] = (value % 10) + 48;
		value /= 10; 
	} 
	if(num < 0) {
		buffer[i++] = '-';
	}
	buffer[i] = '\0';
	// reverse the string
	reverse(buffer, 0, i - 1);
	// bigger than max value of int
	if(int_max == 1) {
		buffer[i - 1]++;
	}
	return buffer;
}  

// printf function
int iocla_printf(const char *format, ...) {
	char *str = malloc(sizeof(char) * 8000); 
	char *temp = malloc(sizeof(char) * 8000);
	const char *traverse;
	int i = 0, j;
	// arguments of the function
	va_list args; 
	va_start(args, format); 

	for(traverse = format; *traverse != '\0'; traverse++) { 
		// if specificator is found
		if(*traverse == '%') {
			traverse++;
			if(*traverse == 'd') {
				// signed integers
				temp = convert_signed(va_arg(args, int));
				for(j = 0; j < strlen(temp); j++) {
					str[i++] = temp[j];
				}
			}
			if(*traverse == 'u') {
				// unsigned integers
				temp = convert(va_arg(args, unsigned int), 10);
				for(j = 0; j < strlen(temp); j++) {
					str[i++] = temp[j];
				}
			}
			if(*traverse == 'x') {
				// hex numbers
				temp = convert(va_arg(args, int), 16);
				for(j = 0; j < strlen(temp); j++) {
					str[i++] = temp[j];
				}
			}
			if(*traverse == 'c') {
				// single character
				str[i++] = va_arg(args, int);
			}
			if(*traverse == 's') {
				// string
				temp = va_arg(args, char *);
				for(j = 0; j < strlen(temp); j++) {
					str[i++] = temp[j];
				}
			}
			if(*traverse == '%') {
				str[i++] = '%';
			}
		}
		else {
			// add the rest of the string 
			str[i++] = *traverse;
		}
	} 
	// add the string terminator character
	str[i] = '\0';
	// write the string to stdout
	write_stdout(str, strlen(str));
	va_end(args);
	temp = NULL;
	str = NULL;
	// free the memory
	free(temp);
	free(str);
	return i;
}
