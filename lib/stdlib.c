#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int atoi(const char *s)
{
	int res = 0;
	char sign = ' ';

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while ('0' <= *s && *s <= '9') {
		res = res * 10 + (*s - '0');
		s++;

	}

	if (sign == '-') {
		res = res * -1;
	}

	return res;
}

char *itoa(int value)
{
	static char buf[12];
	int i = 0; // current index
	int neg = 0; // True = 1, false = 0

	// Handle if value is 0
	if (value == 0) {
		buf[i++] = '0';
		buf[i] = '\0';
		return buf;
	}
	
	// Check if negative
	if (value < 0) {
		neg = 1;
		value = -value;
	}

	// Process digits from least to most significant
	while (value !=0) {
		int rem = value % 10;
		buf[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		value = value / 10;
	}

	// Add negative sign if number is neg
	if (neg == 1) {
		buf[i++] = '-';
	}

	buf[i] = '\0'; // Null-terminate the string

	// Reverse string, as it is currently backwards
	int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buf[start];
        buf[start] = buf[end];
        buf[end] = temp;
        start++;
        end--;
    }

	// Return static buffer
	return buf;
}

int btoi(char number){
    int ten_digit = (number>>4) & 0x0F; //Shifts to the right and masks the first four bits
    int one_digit = number & 0x0F; //Masks the first four bits
    return ten_digit * 10 + one_digit;
}

char itob(int number){
    int ten_digit = number / 10;
    int one_digit = number % 10;
    return (ten_digit<<4) | one_digit;
}

char *itoh(int dec_num) {
	static char hex_str[20];
	int curr_num = dec_num;
	int count =0;

	// Count how long hex string will be
	do {
		count++;
		curr_num /= 16;
	} while (curr_num >= 1);
	curr_num = dec_num;

	// Convert to hex
	for (int i = count - 1 + 2; i >= 2; i--) {
		int curr_rem = curr_num % 16;
		curr_num /= 16;

		// set character at str index to correct hex character
		hex_str[i] = hex_of_int(curr_rem);
	}

	// Null terminate string
	hex_str[count+2] = '\0';

	// Prefix with '0x'
	hex_str[0] = '0';
	hex_str[1] = 'x';


	return hex_str;
}

char hex_of_int(int num) {
	switch(num)
	{
		case 15:
			return 'F';
		case 14:
			return 'E';
		case 13:
			return 'D';
		case 12:
			return 'C';
		case 11:
			return 'B';
		case 10:
			return 'A';
		case 9:
			return '9';
		case 8:
			return '8';
		case 7:
			return '7';
		case 6:
			return '6';
		case 5:
			return '5';
		case 4:
			return '4';
		case 3:
			return '3';
		case 2:
			return '2';
		case 1:
			return '1';
		case 0:
			return '0';
		default:
			return '?';
	}
}

// returns an integer of a hex digit
int int_of_hex(char digit)
{
	switch(digit)
	{
		case 'F':
		case 'f':
			return 15;
		case 'E':
		case 'e':
			return 14;
		case 'D':
		case 'd':
			return 13;
		case 'C':
		case 'c':
			return 12;
		case 'B':
		case 'b':
			return 11;
		case 'A':
		case 'a':
			return 10;
		case '9':
			return 9;
		case '8':
			return 8;
		case '7':
			return 7;
		case '6':
			return 6;
		case '5':
			return 5;
		case '4':
			return 4;
		case '3':
			return 3;
		case '2':
			return 2;
		case '1':
			return 1;
		case '0':
			return 0;
		default:
			return -1;
	}
	//tried atoi. It expects const char*, not char. So here's a switch statement triple the necessary size. 
	// return -1;
}

int power_int(int base, int power){
	int result = 1;

    for (int i = 0; i < power; i++) {
        result *= base;
	}

    return result;
}

int htoi(char *hex_num) {
	int total = 0;
	size_t length = strlen(hex_num);
	int power = 0;
	
	// Check first two characters to ensure prepened with "0x"
	if (hex_num[0] != '0' || hex_num[1] != 'x') {
		return -1;
	}

	// From last character to index 2
	for (int i = length - 1; i >= 2; i--)
	{
		char currDigit = hex_num[i];
		int conversion = int_of_hex(currDigit);

		if (conversion == -1) {
			return -1;
		}
		
	 	total += conversion * power_int(16, power);
		power++;
	}
	return total;
}
