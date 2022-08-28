//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-27T22:44:29AEST consiquence of passing 'NULL' where function pointer is expected (crash with no error msg?) (and what debugger says about it) 
//	Ongoing: 2022-08-28T17:36:35AEST validating a function pointer (can we do anything more than check if for null, use it, and hope?)
//	Ongoing: 2022-08-28T17:46:24AEST collection of pointers to functions of differing types?
//	}}}
//	Continue: 2022-08-28T17:06:50AEST debugger output when passing NULL as function pointer for callback

int sorted_order(int,int);

//	C functions are pointers to a spot where some code exists.
//	Pointers to functions allow callbacks to functions, or simulating classes/objects.
//	<(pointers to functions are how objects are implemented?)>
//	<(emulating OOP in a non-OO language is a bad idea)>

//	Format of a function pointer:
//		int (*pointer_name)(int a, int b)
//	(place parenthesis around function name, and place '*' before name inside parenthesis).

//	Pointer to a function that returns a pointer:
//		char* (*coolness_cb)(int level)
//	or
//		char *(*coolness_cb)(int level)

//	<(Use 'typedef' to declare a function pointer with a give signature as a type)> 
typedef int (*compare_cb)(int a, int b);
typedef int* (*sort_f)(int* numbers, int count, compare_cb cmp);
//	Alternatively, to declare a single function pointer with a given signature:
int (*pointer_name)(int a, int b) = sorted_order;

//	Function pointers can be used just like the functions they point to.

//	Continue: 2022-08-28T17:49:42AEST function pointers resource (see below)
//	LINK: https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work
//	<>

//	Array of function pointers:
//	<>


void die(const char* msg) {
	if (errno) {
		perror(msg);
	} else {
		fprintf(stderr, "Error (no errno): %s\n", msg);
	}
	exit(1);
}

int* bubble_sort(int* numbers, int count, compare_cb cmp) {
	int temp = 0;
	int* target = malloc(count*sizeof(int));
	if (!target) {
		die("Memory error");
	}
	memcpy(target, numbers, count*sizeof(int));
	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count - 1; ++j) {
			if (cmp(target[j], target[j+1]) > 0) {
				temp = target[j+1];
				target[j+1] = target[j];
				target[j] = temp;
			}
		}
	}
	return target;
}

int sorted_order(int a, int b) {
	return a - b;
}

int reverse_order(int a, int b) {
	return b - a;
}

int strange_order(int a, int b) {
	if (a == 0 || b == 0) {
		return 0;
	} 
	return a % b;
}

void test_sorting(int* numbers, int count, sort_f sort, compare_cb cmp) {
	if (!sort) {
		die("Invalid sort function");
	}
	int* sorted = sort(numbers, count, cmp);
	if (!sorted) {
		die("Sort function failed to return");
	}
	for (int i = 0; i < count; ++i) {
		printf("%d, ", sorted[i]);
	}
	printf("\n");
	free(sorted);
}

//	Example: We can access the instructions at the function pointer as bytes
void print_function_pointer(compare_cb cmp) {
	unsigned char* data = (unsigned char*) cmp;
	int len_function_bytes = 25; 	//	most likely not correct (no way to determine without examining compiler output?)
	for (int i = 0; i < len_function_bytes; ++i) {
		printf("%02x:", data[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[]) 
{
	//print_function_pointer(sorted_order);

	if (argc < 2) {
		fprintf(stderr, "Usage: ./pointers-to-functions [numbers]");
		exit(2);
	}

	int count = argc - 1;
	char** inputs = argv + 1;
	int* numbers = malloc(count * sizeof(int));
	if (!numbers) {
		die("Memory error");
	}
	printf("input:\n");
	for (int i = 0; i < count; ++i) {
		numbers[i] = atoi(inputs[i]);
		printf("%i, ", numbers[i]);
	}
	printf("\n");

	compare_cb f = sorted_order;
	sort_f bubble = bubble_sort;

	printf("sorted_order:\n");
	test_sorting(numbers, count, bubble, f);
	printf("reverse_order:\n");
	test_sorting(numbers, count, bubble, reverse_order);
	printf("strange_order:\n");
	test_sorting(numbers, count, bubble, strange_order);

	free(numbers);
	return 0;
}

