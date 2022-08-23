//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <float.h>

void example_datatypes() 
{
	//	datatypes (origional C)
	//			char
	//			short
	//			int
	//			unsigned
	//			float
	//			double
	//	datatypes (added by C90)
	//			signed
	//			void
	//	datatypes (added by C99)
	//			_Bool
	//			_Complex
	//			_Imaginary

	//	Size of C primatives is platform dependent

	//	literal integers are treated as ints, unless they are too large in which case a bigger type is used
	//	literal floats are treated as doubles (suffix with 'f' for a literal float, eg: '2.3f')

	//	size_t sizeof()
	//	<>

	//	On (most?) platforms:
	//		8 bits:		char
	//		16 bits:	short
	//		32 bits:	int, float
	//		64 bits:	long, double

	//	signed types:
	printf("%-20s sizeof=(%lu), [%i, %i]\n", "char", sizeof(char), CHAR_MIN, CHAR_MAX);
	printf("%-20s sizeof=(%lu), [%i, %i]\n", "short", sizeof(short), SHRT_MIN, SHRT_MAX);
	printf("%-20s sizeof=(%lu), [%i, %i]\n", "int", sizeof(int), INT_MIN, INT_MAX);
	printf("%-20s sizeof=(%lu), [%li, %li]\n", "long", sizeof(long), LONG_MIN, LONG_MAX);
	printf("%-20s sizeof=(%lu), [%lli, %lli]\n", "long long", sizeof(long long), LLONG_MIN, LLONG_MAX);

	//	unsigned types:
	printf("%-20s sizeof=(%lu), [%i, %u]\n", "unsigned char", sizeof(unsigned char), 0, UCHAR_MAX);
	printf("%-20s sizeof=(%lu), [%i, %u]\n", "unsigned short", sizeof(unsigned short), 0, USHRT_MAX);
	printf("%-20s sizeof=(%lu), [%i, %u]\n", "unsigned int", sizeof(unsigned int), 0, UINT_MAX);
	printf("%-20s sizeof=(%lu), [%i, %lu]\n", "unsigned long", sizeof(unsigned long), 0, ULONG_MAX);
	printf("%-20s sizeof=(%lu), [%i, %llu]\n", "unsigned long long", sizeof(unsigned long long), 0, ULLONG_MAX);

	//	floating point types:
	//	<(floating point limits?)>
	//	<(floating point epsilon?)>
	printf("%-20s sizeof=(%lu), [%e, %e]\n", "float", sizeof(float), FLT_MIN, FLT_MAX);
	printf("%-20s sizeof=(%lu), [%e, %e]\n", "double", sizeof(double), DBL_MIN, DBL_MAX);
	printf("%-20s sizeof=(%lu), [%Le, %Le]\n", "long double", sizeof(long double), LDBL_MIN, LDBL_MAX);
	printf("\n");

	//	_Bool
	//	<>

	//	Portable types: <stdin.h>
	//		int32_t / uint32_t - signed/unsigned int types guaranteed to be 32 bits
	//	Also:
	//		int_least32_t
	//		int_fast32_t

	//	printf and portable types:
	//	<>

	//	integer overflow/underflow
	//	<>

	//	floating point overflow/underflow
	//	<>

	//	_Complex
	//	<>

	//	_Imaginary
	//	<>

}

void example_printf() 
{
	//	printf()
	//	It is the programmer's responsibility to match printf arguments to format specifiers.
	//	<>

	//	printf format specifiers:
	//	LINK: https://www.cplusplus.com/reference/cstdio/printf/
	//	<>

	//	printing octal | hex
	//	'#' specifies that '0' / '0x' prefixes should be included
	int x = 458;
	printf("x: %i | %#o | %#x\n", x, x, x);

}


int main()
{
	example_datatypes();
	example_printf();

	return 0;
}


