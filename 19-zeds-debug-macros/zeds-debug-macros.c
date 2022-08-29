//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <stdlib.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-28T18:02:27AEST (we have removed '\<newline>'-s from macros (made on line from multiple))
//	Ongoing: 2022-08-28T18:10:14AEST does test code demonstrate difference between 'check_debug' / 'check' (the former proporting not to report 'common' errors?)
//	}}}

//	C manages errors by returning error codes and setting global 'errno'.
//	The simplest way to handle an error is to print a helpful message, die, and leave cleaning up to the OS.
//	However, dying is not always a feasible way to handle an error.

//	C-Macros:
//	C replaces macros with the expanded version of their definition, and will do so recursively.
//	<stand-ins> __FILE__, __func__, __LINE__
//	<>
//	Use '...' to recieve and ##__VA_ARGS__ to pass variadic arguments 

//	'check' debug macros relying on calling function having an 'error' label, as they goto said label upon failure.

//	Disable all debug messages
//	(alternatively use 'CFLAGS=-DNDEBUG')
//#define NDEBUG

//	Book author debug macro(s):
#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

//	If we compile with 'NDEBUG' defined, then no debug messages are output
#ifdef NDEBUG	
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%s:%d: " M "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#endif

//	Get safe/readable version of errno
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

//	error:
#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%s:%d: errno: %s) " M "\n", __FILE__, __func__, __LINE__, clean_errno(), ##__VA_ARGS__)

//	warn:
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%s:%d: errno: %s) " M "\n", __FILE__, __func__, __LINE__, clean_errno(), ##__VA_ARGS__)

//	info:
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%s:%d) " M "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

//	check: log error 'M' if condition 'A' is not true
#define check(A, M, ...) if (!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

//	sentinel: used to indicate <code/conditions> that should not run
#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

//	check_mem: error if pointer 'A' is invalid
#define check_mem(A) check((A), "Out of memory.")

//	check_debug: like 'check' 
//	<(but doesn't bother reporting common errors (how does that work?))> <- this is a lie? (only difference is s/log_err/debug/?)
#define check_debug(A, M, ...) if (!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif // __dbg_h__



void test_debug() {
	debug("I notice you have Brown Hair.");
	debug("I am %d years old.", 37);
}

void test_log_err() {
	log_err("I believe everything is broken.");
	log_err("There are %d Problems in %s.", 0, "space");
}

void test_log_warn() {
	log_warn("You can safely ignore this.");
	log_warn("Maybe consider looking at this: %s.", "[relevant-file]");
}

void test_log_info() {
	log_info("Well I did something mundane.");
	log_info("It happened %f times today.", 1.3f);
}

int test_check_open_file(char* filename) {
	FILE* input = NULL;
	char* block = NULL;

	block = malloc(100);
	check_mem(block);

	input = fopen(filename, "r");
	check(input, "Failed to open filename=(%s)", filename);

	free(block);
	fclose(input);
	return 0;

error:
	if (block) free(block);
	if (input) fclose(input);
	return -1;
}

int test_sentinel(int code) {
	char* temp = malloc(100);
	check_mem(temp);

	switch (code) {
		case 1:
			log_info("It worked.");
			break;
		default:
			sentinel("default-switch case should not run.");
	};

	free(temp);
	return 0;

error:
	if (temp) free(temp);
	return -1;
}

int test_check_mem() {
	char* test = NULL;
	check_mem(test);

	free(test);
	return 1;

error:
	return -1;
}

int test_check_debug() {
	int i = 0;
	check_debug(i != 0, "Number should not be 0.");

	return 0;

error:
	return -1;
}




int main(int argc, char* argv[])
{
	check(argc >= 2, "Need an argument.");

	test_debug();
	test_log_err();
	test_log_warn();
	test_log_info();

	check(test_check_open_file("zeds-debug-macros.c") == 0, "File 'zeds-debug-macros.c' should exist");
	check(test_check_open_file(argv[1]) == -1, "File 'argv[1]' should not exist");

	check(test_sentinel(1) == 0, "Should succeed");
	check(test_sentinel(10) == -1, "Should fail");

	check(test_check_mem() == -1, "Should fail");
	check(test_check_debug() == -1, "Should fail");

	return 0;

error:
	return 1;
}

