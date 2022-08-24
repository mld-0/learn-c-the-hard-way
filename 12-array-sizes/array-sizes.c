//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-23T17:35:59AEST 'char s1[x] = "asdf"' is null-terminated, unless x is too small to fit the null-terminator 
//	Ongoing: 2022-08-23T17:42:17AEST Purpouse of 'argc' if we can sizeof argv? (difference between 'char* argv[]' and 'char** argv'(?) -> 'char*[]' is considered equivalent to 'char**' when used in a function argument(?)) [...] (can't use 'char**' with {"abc", "def", "hij"}?) [...] <- arrays are pointers <only?> for <parameters/arguments>
//	}}}

//	An array size may be an expression, provided it evaluates to a constant.
//	If array size is not given, the number of initalization values is used.

//	<(If we initalize at least one element, all remaining uninitalized elements are set to 0)>

//	Multi-dimensional arrays:
//	m[3][2] is stored in consecutive memory locations as { m[0][0], m[0][1], m[1][0], m[1][1], m[2][0], m[2][1] }
//	<>


int main(int argc, char* argv[])
{
	int areas[] = {10, 12, 13, 14, 20};
	size_t len_areas = sizeof(areas) / sizeof(*areas);
	printf("len_areas=(%lu)\n", len_areas);

	char name1[] = "Zed";						//	Includes null-terminator for array len=4
	char name2[] = { 'Z', 'e', 'd', '\0' };		//	Equivalent char-array

	size_t len_name1 = sizeof(name1) / sizeof(*name1);
	size_t len_name2 = sizeof(name1) / sizeof(*name1);

	printf("name1=(%s), name2=(%s)\n", name1, name2);
	printf("len_name1=(%lu)\n", len_name1);
	printf("len_name2=(%lu)\n", len_name2);

	//	This is an array, each element of which is a char*
	//	<(if declared as an argument, becomes equivalent to char**)>
	char* states[] = { "California", "Oregon", "Washington", "Texas", "Florida", };
	size_t len_states = sizeof(states) / sizeof(*states);
	printf("len_states=(%lu)\n", len_states);

	//	error/invalid/<bad?> <(incompatiable pointer types)>
	//char** states2 = { "California", "Oregon", "Washington", "Texas", "Florida", };

	return 0;
}

