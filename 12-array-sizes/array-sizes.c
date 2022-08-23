#include <stdio.h>
//	Ongoing: 2022-08-23T17:35:59AEST 'char s1[x] = "asdf"' is null-terminated, unless x is too small to fit the null-terminator 

int main()
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

	return 0;
}

