//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <assert.h>
//	Ongoings:
//	{{{
//	Ongoing:2022-08-24T00:10:06AEST any repo/ex15.c examples not in book?
//	Ongoing: 2022-08-24T00:23:16AEST (pointers are just arrays example) implying 'p_names[i]' is equivalent to '*(p_names+i)' (does it go both ways?)
//	Ongoing: 2022-08-24T00:26:34AEST needing a 'char**' (not 'char*') (a 2d array suggest/implies a 2d pointer) (or just for) (the case where it is a array of pointers (not a 2d array in the sense of row/col-major indexing) (where data is <likely> not adjacent to array of pointers to data)) [...] ((or asked another way) is there a way to do it with a 'char*' pointer (we treat differently?))
//	Ongoing: 2022-08-24T00:32:25AEST advantage of using pointer plus index versus incrementing pointer (best practice?)
//	Ongoing: 2022-08-24T00:39:40AEST C and pass-by-value (of-structs/otherwise?)
//	Ongoing: 2022-08-25T00:04:06AEST Writing 'char[]' (instead of 'char varname[]') (in function signiture(?))
//	Ongoing: 2022-08-25T00:23:17AEST 'char* argv[]' is literally equivalent to 'char** argv' (and we have to also pass argc because, as a pointer (not an array), it has no associated size information
//	Ongoing: 2022-08-25T00:26:03AEST (why) C does not allow passing of arrays (considering how maddening it is that 'int[]' means something else (only?) for a function argument)
//	Ongoing: 2022-08-25T00:38:29AEST ('char* argv[]' is literally equivalent to 'char** argv') <(<when/only-when> used as a function parameter)>
//	Ongoing: 2022-08-25T00:42:22AEST (meaning of the address of a pointer) address of the integer (on the stack?) used to store the address
//	Ongoing: 2022-08-25T00:45:51AEST Is adding pointers ever a meaningful thing to do?
//	Ongoing: 2022-08-25T16:54:15AEST (if arrays-as-function-arguments are actually pointers) does it ever make sense to use an array type (instead of a pointer) for a function argument?
//	Ongoing: 2022-08-25T17:45:17AEST (so far) (for all cases where sizeof() has been used where it does not produce the size of an array, the compiler has (noticed and) warned of our incorect usage)
//	}}}

//	<(A pointer <contains/stores/references/points-to> an address in memory)>
//	A pointer <introduces/stores/?> the type (and therefore size) of the data being pointed to
//	C <provides> incrementing/decrementing/adding/subtracting and dereferencing of pointers

//	(the compiler optimises array syntax into pointer operations <(where applicable?)>)
//	(Pointers can (still) (sometimes) have performance benifits over arrays (but) (in most circumstances, (it won't matter and) clarity is <key/king>))

//	Pointer Arithemetic:
//	Adding 1 to an 'int*' pointer adds 1*sizeof(int) to the address being pointed to
//	Subtracting pointers gives the number of multiples of sizeof(int) between the addresses being pointed to

//	Uses of pointers:
//		Store address of memory requested from OS
//		Pass large structs to functions
//		Store the address of a function <(use in/as a dynamic callback)>
//		Scan memory chunk
//		<(bytes from network socket into datastructure (the pointer is the socket or the data structure here?))>
//		Parsing files <(example of a pointer as a position in a file?)>
//	(For most other <uses/tasks/functions> where pointers <get/are> used), <(arrays should be used instead? - (arrays can't be passed to functions?))>


//	Pointer lexicon:
//		dtype *ptr 			A dtype pointer named 'ptr'
//		<(type *ptr is equivalent to type* ptr (and one should prefer the <later?>))>
//		*ptr				Dereferencing a pointer (value being pointed to)
//		*(ptr+i)			Dereferencing element i in array pointed to by ptr
//		&thing				Address of 'thing'
//		ptr++				Increment address being pointed to (add sizeof(dtype) to address)


//	Pointers are not arrays:
//	Pointer variables can be assigned a new value, array variables cannot.
//	(A literal string assigned to 'char*' is readonly, but assigned to 'char[]' it can be modified) <(other types?)>
//	Arrays arguments are always passed as pointers, even when '[]' is used.
//	Pointers can be treated as arrays (used with array indexing syntax)
//	arrays are accessed using direct addressing
//	pointers are accessed using indirect addressing

//	sizeof(array) gives the total size of the array.
//	sizeof(pointer) gives the size of the type of the pointer.

//	&array is an alias for &array[0]
//	&pointer returns the address of 'pointer'

//	direct addressing: <(the effective address is placed in the instruction, no intermediate memory access is required. rarely used nowadays)>
//	indirect addressing: <(the instruction contains the effective address where the address to be accessed is stored (register-indirect/memory-indirect))>

//	effective address: <(memory-location/register (offset to get to a memory location?))>
//	<>


//	Pointers to arrays and passing arrays to functions:
// 	LINK: https://stackoverflow.com/questions/6567742/passing-an-array-as-an-argument-to-a-function-in-c
//	{{{
//	Equivalent:
//		void f(int a[SIZE])
//		void f(int a[])
//		void f(int* a)

//	A value for the size of the array parameter can be provided. It is not <used/enforced>, but it serves to document the minimum size of any array to be passed to the function.

//	Pointers to arrays: Stores the address of the first element of the array, while the length of the array is part of the type of the pointer.
//		int (*pa)[U] = &a;

//	<(A pointer to an array cannot be re-assigned)>

//	Forcing type safety on array parameter (sizes):
//	For an array 'int a[2]', a pointer to that array '&a' has type 'int (*)[2]'
//	If we use this as the type of our parameter, C will produce a warning if we pass an array of the wrong size. Combine with -Werror to force use of correct array size.
void expect_arraysize(int (*a)[10]) {
	//	we can get the size of a this kind of pointer to an array with:
	size_t len_a = sizeof(*a)/sizeof(**a);
}
void test_expect_arraysize() {
	int a1[10] = {0};
	int a2[5] = {0};

	//	'a1' has type 'int*'
	//	'&a1' has type 'int (*)[10]'

	//	'expect_arraysize(a1)' and 'expect_arraysize(&a1)' behave differently - 'a1' is an array and therefore decays into a pointer, whereas '&a1' does not decay as it is already a pointer (to an array 'int (*)[10]')

	//expect_arraysize(a1);			//	warning (incompatible pointer type) 
	//expect_arraysize(&a2);		//	warning (incompatible pointer type) (wrong size)

	expect_arraysize(&a1);			//	correct 

	//	Declaring a 'type-safe' pointer to array
	int (*p_a1)[10] = &a1;
	expect_arraysize(p_a1);
}

//	As a function <argument/parameter>, 'int (*)[]' is equivalent to 'int**' (and does not have size information)
//	Otherwise, 'int (*)[]' does have 

//	In C++, we can also use references to an array 'int (&a)[10]' 

//	}}}


int main(int argc, char* argv[]) 
{
	int ages[] = { 23, 43, 12, 89, 2 };
	int len_ages = sizeof(ages) / sizeof(*ages);
	char* names[] = { "Alan", "Frank", "Mary", "John", "Lisa" };
	int len_names = sizeof(names) / sizeof(*names); 
	assert(len_ages == len_names);

	//	Example: array indexing
	for (int i = 0; i < len_ages; ++i) {
		printf("%s: age=%d, ", names[i], ages[i]);
	}
	printf("\n");


	//	pointer to an array of pointers to c-strings
	char** p_names = names;

	//	pointer to an array of ints
	int* p_ages = ages;


	//	Example: pointer plus index
	for (int i = 0; i < len_ages; ++i) {
		printf("%s: age=%d, ", *(p_names+i), *(p_ages+i));
	}
	printf("\n");

	//	Example: pointers are just arrays <(except when they aren't)>
	for (int i = 0; i < len_ages; ++i) {
		printf("%s: age=%d, ", p_names[i], p_ages[i]);
	}
	printf("\n");

	//	Example: <convoluted for-loop>
	for (p_ages=ages, p_names=names; (p_ages-ages) < len_ages; ++p_ages, ++p_names) {
		printf("%s: age=%d, ", *p_names, *p_ages);
	}
	printf("\n");


	test_expect_arraysize();

	return 0;
}

