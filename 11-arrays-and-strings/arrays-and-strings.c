#include <stdio.h>
#include <string.h>

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
	//	(The resulting string will be read only)
	//	(can't use sizeof(x)/sizeof(*x) on a pointer)
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

