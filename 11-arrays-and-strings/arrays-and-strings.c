//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <string.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-25T00:07:09AEST attempting to modify a C-string assigned to 'char*' produces a crash with (no error seen in vim-exe) 'bus error' 
//	Ongoing: 2022-08-25T00:08:41AEST Address of a literal C-string assigned to 'char*' vs assigned to 'char[]' -> '0x100923f84' vs '0x16d531dbc'
//	Ongoing: 2022-08-25T00:10:31AEST Printing a pointer value with 'printf("%lx\n", cstr1)' (where cstr1 is a char*) -> a pointer is a 64bit value (so how do I make the wrong-data-type-warning go away?)
//	}}}

//	A C-string is an array of bytes, termianted with '\0' (null-byte).
//	(This can be very problematic).

int main()
{
	int numbers[4] = { 1,2 }; 		//	If we supply at least 1 value, any values not supplied are initalized to 0
	printf("numbers: ");
	for (int i = 0; i < 4; ++i) {
		printf("%i, ", numbers[i]);
	}
	printf("\n");

	char name[4] = { 'a' };			//	String is initalized as "a\0\0\0"
	printf("name=(%s)\n", name);

	//	Book recomendation: use 'char* x' not 'char x[]' for C-strings initalized with quotes
	//	Stackoverflow recomendation: Use 'const char*' for C-string literals
	//	(The resulting string will be read only)
	//	(can't use sizeof(x)/sizeof(*x) on a pointer) (see item 15 for arrays vs pointers)
	char* cstr1 = "asdf";
	int len_cstr1 = strlen(cstr1);
	printf("len_cstr1=(%i)\n", len_cstr1);

	//	Convert char[4] into int
	char bytes[4] = { 'a', 's', 'd', 'f' };
	int value;
	memcpy(&value, bytes, sizeof(int));
	printf("value=(%d)\n", value);

	return 0;
}

