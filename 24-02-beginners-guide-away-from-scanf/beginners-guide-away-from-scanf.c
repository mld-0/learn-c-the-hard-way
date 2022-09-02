//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//	LINK: http://sekrit.de/webdocs/c/beginners-guide-away-from-scanf.html

//	int fscanf(FILE* stream, const char* format, ...)
//	char* fgets(char* str, int num, FILE* stream)

//	Summary: Use 'fgets()', not 'scanf()', to read a line of input.
//		scanf does not know the buffer size unless it is given in the format specifier.
//		fgets reads until a newline, scanf reads until whitespace.
//		scanf requires us to know the exact format of the input.
//		When scanf fails to convert fields, it leaves all input data on the stream.
//		Using both fgets and sscanf allows easier error handling than just scanf.
//		Alternatively use strtoll/strtod/strtof to convert numbers.
//		<(scanf leaves behind a newline on the input stream)>.
//		Use 'fread' to read binary data.
//	Buffer size:
//		Pass 'fgets' BUFFER_LEN=n, it will read n-1 bytes
//		For 'scanf' use BUFFER_LEN-1 as field width 

//	Rule 0: don't use scanf without knowing exactly what it does
//	Rule 1: scanf is not for reading input, it is for parsing input
//	Rule 2: always use field widths '%(BUFFER_LEN-1)s' when reading a string
//	Rule 3: scanf format strings have slightly different semantics to printf format strings
//	Rule 4: scanf is a very powerful function, with great power comes great responsibility (read the documentation if using to avoid pitfalls)


void Reading_a_Number_i() {
	//	scanf never reads anything it cannot parse (according to format string). Any data not parsed is left on the input stream.
	//	If the user enters something that is not a number in response to our prompt, the value of 'a' will be undefined.
	int a;
	printf("Enter an int: ");
	//	scanf returns the number of values it successfully parses
	int rc = scanf("%d", &a);
	printf("a=(%d), rc=(%d)\n", a, rc);
}


void Reading_a_Number_ii() {
	//	If the user enters something that is not an int, it is never read, and the data remains on the input stream.
	//	The next call to scanf finds the same unread input, and the result is an infinite loop.
	int a;
	do {
		printf("Enter an int: ");
	} while (scanf("%d", &a) != 1);
	printf("a=(%d)\n", a);
	//	Flushing an input stream is (not a solution as it is) undefined behaviour.
	//	<(Use 'scanf("%*[^\n]")' to discard unread input)>
}


void Reading_a_string_i() {
	//	scanf is unaware of the size of the buffer it is reading into
	//	'%s' will match any length of consecutive non-whitespace
	//	Always specify the field width of the buffer when reading a string with scanf (1 less than the buffer size to account for the null terminator that will be appended)
	char name[40];
	printf("Enter name: ");
	//	note that value returned is number of strings (words) parsed, not number of characters
	int rc = scanf("%39s", name);
	printf("name=(%s), rc=(%d)\n", name, rc);
	//	'%s' matches a sequence of non-whitespace characters - it matches a word, not a line
	//	<('%39[^\n]' is incorrect, as it rejects the newline from pressing enter on the input - nothing is parsed, and the input remains uninitalised)> <(A workaround is ' %39[^\n]', which skips any leading whitespace (including no leading whitespace), but this still has undefined behaviour for an empty input (Ctrl + d))>
	//	<(Use '%[^\n]%*c' to read a line - or better yet, use 'fgets()')>
}


void Reading_a_string_ii() {
	//	We can create our own format string with a variable as our width:
	const int BUFFER_LEN = 40;
	char name[BUFFER_LEN];
	char format[8];
	sprintf(format, "%%%ds", BUFFER_LEN);
	printf("format=(%s)\n", format);
	printf("Enter name: ");
	int rc = scanf(format, name);
	printf("name=(%s), rc=(%d)\n", name, rc);
}


void Reading_a_Line_with_fgets() {
	//	To read a line, use 'fgets(char* str, int n, FILE* stream)' - it reads a given maximum number of characters, but stops at a newline (newline is included)
	//	Return value is either 'str', or NULL upon failure
	//	Ongoing: 2022-09-02T18:04:37AEST segfaults given 0 length input (Ctrl + d)
	const int BUFFER_LEN = 40;
	char name[BUFFER_LEN];
	printf("Enter name: ");
	char* rc = fgets(name, BUFFER_LEN, stdin);
	if (!rc) {
		printf("Failed to read name, rc=(%d)\n", *rc);
		return;
	} 
	//	Remove newline (if found)
	name[strcspn(name, "\n")] = '\0';
	printf("name=(%s)\n", name);
}


void Reading_a_Number_with_fgets() {
	//	Read input line using fgets,
	//	and convert to long with 'long int strtol(const char* nptr, char** endptr, int base)'
	long a;
	char buffer[1024];
	int done = 0;
	do {
		printf("Enter an int: ");
		if (!fgets(buffer, 1024, stdin)) {
			printf("Reading input failed\n");
			return;
		}
		char* endptr;
		errno = 0;
		a = strtol(buffer, &endptr, 10);
		done = 0;
		if (errno == ERANGE) {
			printf("Number is too small/large\n");
		} else if (endptr == buffer) {
			printf("No numerical character read\n");
		} else if (*endptr && *endptr != '\n') {			//	we are relying on string finishing with a newline
			printf("Failed to convert whole input\n");
		} else {
			done = 1;
		}
	} while (!done);
	printf("a=(%ld)\n", a);
	//	Other string->number conversion functions: atoi/atof/strtoll/strtod
}


void Reading_a_Number_correctly_with_scanf() {
	long a;
	int rc;
	printf("Enter an int: ");
	while ((rc = scanf("%ld", &a)) == 0) {		//	neither success (1) nor EOF
		scanf("%*[^\n]");						//	clear what is left ('*' means only match and discard)
		printf("Enter an int: ");
	}
	if (rc == EOF) {
		printf("EOF, Nothing more to read, no numbers found\n");
		return;
	}
	printf("a=(%ld)\n", a);
}


void Reading_a_Line_correctly_with_scanf() {
	char buffer[1024];
	printf("Enter name: ");
	int rc = scanf("%1023[^\n]%*c", buffer);
	if (rc != 1) {										//	expect exactly 1 conversion
		printf("Failed to read name, rc=(%d)", rc);
		return;
	}
	printf("name=(%s)", buffer);
}



int main(int argc, char* argv[]) 
{
	//Reading_a_Number_i();
	//Reading_a_Number_ii();
	//Reading_a_string_iv();

	//Reading_a_Number_with_fgets();
	//Reading_a_Line_with_fgets();

	//Reading_a_Number_correctly_with_scanf();
	//Reading_a_Line_correctly_with_scanf();

	return 0;
}

