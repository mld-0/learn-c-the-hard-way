#include <stdio.h>

//	C does not have a true boolean type.
//	Any integer that is '0' is False, or is otherwise True.

//	argv[0] contains the program name (or alternatively whatever the 'execve' system call provides)
//	If argv[0][0] is null, then the name is not available.
int main(int argc, char* argv[]) 
{
	printf("argc=(%i)\n", argc);

	for (int i = 0; i < argc; ++i) {
		printf("argv[%i]=(%s)\n", i, argv[i]);
	}

	return 0;
}

