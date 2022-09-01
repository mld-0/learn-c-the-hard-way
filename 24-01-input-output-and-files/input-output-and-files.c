//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "debug-macros.h"
//	Ongoings:
//	{{{
//	Ongoing: 2022-09-01T22:23:06AEST The syntax of putting the name at the beginning and end of a 'typedef struct' or 'typedef enum' declaration (is making me uncomfortable) [...] (review C-typedefs)
//	Ongoing: 2022-09-01T22:25:10AEST (what are the) rules for when we can use a variable as an array length (as an array inside a struct?)
//	Ongoing: 2022-09-01T22:30:10AEST As neat as book debug macros are -> are they really a best example of C error handling (or merely a convinent one) (does it matter?)
//	Ongoing: 2022-09-01T22:27:09AEST Use of '\n' in prompt before user inputs (book has prompt and user input on the same line?) ((when) does it matter?)
//	Ongoing: 2022-09-01T22:29:09AEST (here (with our usage of fgets) we are relying on the fact that our string buffers are zeroed?)
//	Ongoing: 2022-09-01T22:31:57AEST type of our enum (safety/correctness of using an int?)
//	Ongoing: 2022-09-01T22:33:22AEST safety/correctness of our method of iterating over an enum (and finding associated array value)
//	Ongoing: 2022-09-01T22:45:28AEST book is using fgets to read a string (input as a line), fscanf to read numbers
//	Ongoing: 2022-09-01T22:48:54AEST method of reading a string (that don't involve reading an entire line)
//	Ongoing: 2022-09-01T22:52:17AEST C-input is an important topic (and this book chapter provides a poor guide/explanation?)
//	Ongoing: 2022-09-01T23:10:58AEST Use of 'MAX_DATA-1' in 'fgets' is incorrect(?) (it is perfectly acceptable to provide the size of the array, since fgets copies until 'num'-1 characters have been read) (and book usage of '-1' is <unneccessary/wrong?>) [...] (is the book being wrong here on purpouse for us to spot?)
//	Ongoing: 2022-09-01T23:45:07AEST sprintf/snprintf are provided as macros?
//	Ongoing: 2022-09-02T00:29:07AEST (how to) pass data to program from run script (lines via stdin?)
//	}}}
//	Continue: 2022-09-02T00:38:51AEST beginners guide away from scanf
//	Continue: 2022-09-02T00:39:09AEST no-fscanf version
//	Continue: 2022-09-02T00:47:04AEST provide input from script [...] (using file as input with same code?)
//	Continue: 2022-09-02T00:47:17AEST entering input on newline vs same line as input prompt

//	C-IO Functions:
//	{{{
//	int scanf(const char* format, ...)
//			Reads data from stdin, and stores them according to specifiers in 'format' into locations 
//			pointed to by additional arguments (which should be already allocated, and have types
//			matching the 'format' specifiers.
//			Format specifier: %[*][width][length]specifier
//			Returns number of items successfully filled.

//	int fscanf(FILE* stream, const char* format, ...)
//			As per 'scanf()', from input FILE object 'stream'.

//	int sscanf(const char* str, const char* format, ...)
//			As per 'scanf()', from input string 'str'.

//	int vscanf(const char* format, va_list arg)
//			As per 'scan()', using 'va_list' instead of variadic arguments.

//	char* gets(char* str)
//		Very bad (do not use, ever) (does not <consider> input size).
//		(So bad that) compiler should warn us that it is unsafe if we use it.

//	char* fgets(char* str, int num, FILE* stream)
//			Reads characters from 'stream' into 'str', until 'num'-1 characters have been read, or
//			a newline / eof is encountered. Trailing newline is copied into 'str'. 

//	int fgetc(FILE* stream)
//	int getc(FILE* stream)
//			equivalent
//			Returns character at current position in 'stream', and advance position indicator.
//			If at EOF, return EOF and set EOF indicator (check with 'feof').
//			If read fails, return EOF and set errno.

//	int fputc(int character, FILE* stream)
//	int putc(int character, FILE* stream)
//			(equivalent)
//			Write 'character' to 'stream' and advance position indicator.
//			Returns character written on success, otherwise returns EOF and sets errno.

//	int ungetc(int ch, FILE* stream)
//			If ch != EOF, pushes 'ch' (interprited as unsigned char) into input buffer associated with 'stream'.
//			Next read operation from 'stream' will retrieve that character.
//			External device associated with stream is not modified.
//			May fail if called a second time before reading the previous character (size of the pushback buffer varies between systems, Linux/macOS = 4k).
//			Undone by fseek/fsetpos/rewind.
//			If successful, clears EOF. In binary mode, position indicator is decremented by 1.
//			In text mode, the position indicator is updated such that it will be at its previous position after all characters inserted with 'ungetc' are read.
//			Returns 'ch' is successful, otherwise return EOF (and stream remains unchanged).
//			<(Used to implement scanf?)>

//	FILE* fopen(const char* filename, const char* mode)
//			Open the file 'filename', and return the associated stream, according to 'mode'.
//			Stream is fully buffered if it is known to not refer to an interactive device.
//			Returned pointer can be disassociated from fiel with 'fclose' or 'freopen'.
//			All open files are closed on normal program termination.
//			The system support at least 'FOPEN_MAX' files simultaneously.
//			For files opened for read/write, either flush or reposition before reading after writing.
//			On failure, returns NULL and sets errno.
//			<(where have we detailed 'mode' before (same info goes here)?)>

//	FILE* freopen(const char* filename, const char* mode, FILE* stream)
//			Reuse 'stream' to open new file 'filename' or change access 'mode'.
//			If given a 'filename', close current stream before opening that file.
//			Use 'filename' = NULL to change mode of current file.
//			Useful for redirecting predefine streams like stdin/stdout/stderr to a file.
//			Returns 'stream' on success, on failure returns NULL and sets errno.

//	FILE* fdopen(int fd, const char* mode)
//			As per 'fopen', but with a file descriptor 'fd' instead of a filepath.

//	int fclose(FILE* stream)
//			Close the file associated with 'stream'.
//			Returns 0 if successful, or EOF on failure.

//	int fcloseall(void)
//			Closes all open streams belonging to the calling process (all buffers are flushed).
//			(Including stdin/stdout/stderr).
//			Returns 0 if successful, or EOF on failure.

//	int fflush(FILE* stream)
//			Any unwritten data in 'stream' output buffer is written to file.
//			If 'stream' = NULL, all streams are flushed.

//	int fgetpos(FILE* stream, fpos_t* pos)
//			Store the current position of 'stream' in 'pos'.
//			Returns 0 if successful, or return non-zero value and set errno on failure.
//			<(fsetpos? (that accepts an fpos_t pointer))>

//	int fseek(FILE* stream, long int offset, int origin)
//			Set the position indicator associated with 'stream' to new position.
//			In binary mode, new position is offset from origin.
//			In text mode, offset must either equal 0 or a value from 'ftell', and 'origin' shall be SEEK_SET.
//			EOF internal indicator of 'stream' is cleared after successful call, and effects from 'ungetc' are dropped.
//			Allows (performs necessary flushes) for a switch between reading/writing.
//			Values for 'origin' are SEEK_SET (beginning), SEEK_CUR (current), SEEK_END (end).
//			Returns 0 if successful, otherwise return non-zero and set errno.

//	long int ftell(FILE* stream)
//			Returns current valeu of position indicator of the 'stream'.
//			In binary mode, this is number of bytes from file start.
//			In text mode, this number may not be meaningful (but it can be used with 'fseek').
//			If there are characters put back using 'ungetc' still pending of being read, result is undefined.
//			Returns position if succesfull, otherwise return '-1L' and set errno.

//	void rewind(FILE* stream)
//			Sets the position indicator associated with 'stream' to beginning of file.
//			EOF and error internal indcators of 'stream' are cleared after successful call, and effects from 'fgetc' are dropped.
//			Allows (performs necessary flushes) for a switch between reading/writing.

//	int printf(const char* format, ...)
//			Writes string 'format' to stdout, with format-specifiers replaced by additional (variadic) arguments.
//			Format specifier: %[flags][width][.precision][length]specifier
//			Returns total number of characters written, or on failure return a negative value and set errno.

//	int fprintf(FILE* stream, const char* format, ...)
//			As per 'printf', to output FILE object 'stream'.

//	int snprintf(char* s, size_t n, const char* format, ...)
//			As per 'printf', to output buffer 'str' of size 'n'.
//			Any output characters after 'n-1' are discarded.
//			Terminating null is appended after content.
//			Returns number of characters that would have been written if 'n' had been sufficently large, excluding null terminator. The returned value is non-negative and less than 'n' when successful and entire string is written.
//			Returns negative number upon error.

//	int sprintf(char* str, const char* format, ...)
//			Unsafe version of 'snprintf'.
//			<(do not use?)>

//	size_t fwrite(const void* ptr, size_t size, size_t count, FILE* stream)
//			Write 'count' elements, each one of 'size' bytes, from memory 'ptr', to current position in 'stream'.
//			Position indicator of stream is advanced by total number of bytes written.
//			Bytes are written as if 'ptr' was an array of unsigned char, and each element was passed to 'fputc'.
//			Returns number of elements successfully written. If not all bytes could be written, errno will be set.

//	size_t fread(void* ptr, size_t size, size_t count, FILE* stream)
//			Reads 'count' elements, each of of 'size' bytes, from 'stream' into memory 'ptr'.
//			Position indicator of the stream is advanced by total number of bytes read.
//			Returns the number of elements successfully read. If not all bytes could be read, relevent indicator will be set (check with 'ferror' / 'feof').

//	int feof(FILE* stream)
//			Check wither EOF indicator assocatied with 'stream is set.
//			This indicator is cleared by calls to clearerr/rewind/fseek/fsetpos/freopen
//			Returns non-zero if EOF is set, otherwise zero.
//	}}}

//	int fscanf(FILE* stream, const char* format, ...)
//	char* fgets(char* str, int num, FILE* stream)

//	fscanf() is bad (for inputing strings) <(and we should just use fgets() instead?)>
//	fscanf vs fgets:
//			<(fscanf does not allow us to specify buffer size?)>
//			fgets reads up to a newline (including newline)
//			fscanf reads up to whitespace (excluding whitespace)
//	<(fscanf is (only) safe if we specify string buffer maximum size in specifier eg: '%9s')>


void rstrip(char* s) {
	while (isspace((unsigned char)*s)) ++s;
	while (!isspace((unsigned char)*s)) ++s;
	*s = '\0';
}

#define MAX_DATA 	100

typedef enum EyeColor {
	BLUE_EYES, GREEN_EYES, BROWN_EYES, BLACK_EYES, OTHER_EYES
} EyeColor;

const char *EYE_COLOR_NAMES[] = { "Blue", "Green", "Brown", "Black", "Other" };

typedef struct Person {
	int age;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
	EyeColor eyes;
	float income;
} Person;

int read_in_person(Person* p) {
	char *in = NULL;

	printf("Enter first name:\n");
	//printf("Enter first name: ");
	in = fgets(p->first_name, MAX_DATA-1, stdin);
	check( in != NULL, "Failed to read first name");
	rstrip(p->first_name);

	printf("Enter last name:\n");
	//printf("Enter last name: ");
	in = fgets(p->last_name, MAX_DATA-1, stdin);
	check( in != NULL, "Failed to read last name");
	rstrip(p->last_name);

	printf("Enter age:\n");
	//printf("Enter age: ");
	int rc = fscanf(stdin, "%d", &p->age);
	check( rc > 0, "Enter a number for age");

	printf("Eye color choices:\n");
	for (int i = 0; i <= OTHER_EYES; ++i) {
		printf("%d=(%s), ", i, EYE_COLOR_NAMES[i]);
	}
	printf("\n");

	//printf("Enter eye color:\n");
	printf("Enter eye color: ");
	rc = fscanf(stdin, "%d", &p->eyes);
	check( rc > 0, "Enter a number for eye color");
	check( p->eyes <= OTHER_EYES && p->eyes >= 0, "Eye value outside valid range");

	printf("Hourly income:\n");
	//printf("Hourly income: ");
	rc = fscanf(stdin, "%f", &p->income);
	check( rc > 0, "Enter a float for income");

	return 0;
error:
	return -1;
}

int read_in_person_no_fscanf(Person* p) {
	char *in = NULL;

	printf("Enter first name:\n");
	in = fgets(p->first_name, MAX_DATA-1, stdin);
	check( in != NULL, "Failed to read first name");

	printf("Enter last name:\n");
	in = fgets(p->last_name, MAX_DATA-1, stdin);
	check( in != NULL, "Failed to read last name");

	printf("Enter age:\n");
	int rc = fscanf(stdin, "%d", &p->age);
	check( rc > 0, "Enter a number for age");

	printf("Eye color choices:\n");
	for (int i = 0; i <= OTHER_EYES; ++i) {
		printf("%d=(%s), ", i, EYE_COLOR_NAMES[i]);
	}
	printf("\n");

	printf("Enter eye color:\n");
	//rc = fscanf(stdin, "%d", &p->eyes);
	check( rc > 0, "Enter a number for eye color");
	check( p->eyes <= OTHER_EYES && p->eyes >= 0, "Eye value outside valid range");

	printf("Hourly income:\n");
	//rc = fscanf(stdin, "%f", &p->income);
	check( rc > 0, "Enter a float for income");

	return 0;
error:
	return -1;
}

void output_person(Person* p) {
	printf("first_name=(%s)\n", p->first_name);
	printf("last_name=(%s)\n", p->last_name);
	printf("age=(%d)\n", p->age);
	printf("eyes=(%s)\n", EYE_COLOR_NAMES[p->eyes]);
	printf("income=(%f)\n", p->income);
}


int main(int argc, char* argv[])
{
	int rc;

	Person you = {.age = 0};
	rc = read_in_person(&you);
	if (rc < 0) return 1;
	output_person(&you);

	return 0;
}
