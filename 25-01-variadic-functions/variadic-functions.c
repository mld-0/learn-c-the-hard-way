//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdarg.h>
#include <stdio.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-09-03T00:07:36AEST C23 changes how variadic arguments work(?) [...] (when does C23 become relevent?) [...] (since C23) variadic functions are not required to have a first argument(?)
//	Ongoing: 2022-09-03T00:17:43AEST va_arg, undefined unless (things to do with) two types (one being T) (where is the other comming from) [...] example of these exceptions to undefined behaviour(?)
//	Ongoing: 2022-09-03T00:19:44AEST some standard library things (like va_arg/va_copy) are implemented as macros because they have to be / couldn't do what they do as functions? [...] (and in the case of those macros, <goodness-knows> how they work (presumedly they must be implemented in C?)) [...] (the <giveaway> (of something f----) is that 'va_arg' must be supplied with the last non-variadic argument)
//	Ongoing: 2022-09-03T00:21:17AEST (there must be a better 'list of things to remember) vis-a-vis variadic functions in C (cppreference being far to <technical/specific>)
//	Ongoing: 2022-09-03T00:27:41AEST meaning of a 'prototyped function declaration'?
//	Ongoing: 2022-09-03T00:31:10AEST initial argument must contain information about how many variadic arguments are to follow? 
//	Ongoing: 2022-09-03T00:37:13AEST it makes sense for the first argument (the one specifying how many variadic arguments are to follow) should be const?
//	Ongoing: 2022-09-03T00:40:43AEST handling the n=0 / n<0 cases
//	Ongoing: 2022-09-03T00:41:27AEST importance of calling va_end?
//	}}}

//	Pre-C23:

//	A varidadic function is a function that takes a variable number of arguments. 
//	The archetypal variadic function is 'int printf(const char* format, ...)'
//	<(C-style variadic functions are unsafe (hence why C++ provides a new implementation))>

//	Only prototyped function declarations may be variadic. This is indicated by '...', which
//	must appear last and must follow at least one named parameter (until C23).
//	Each variadic argument undergoes default argument promotion.

//	Type:
//	va_list
//			Complete object, storing information needed by va_start/va_copy/va_arg/va_end
//	
//	Functions:
//	void va_start(va_list ap, parmN)
//			ap = instance of va_list, 
//			paramN = named parameter preceding first variable parameter
//			Enables access to variable arguments following the named argument 'paramN'.
//			Behaviour is undefined if 'paramN' is a register/array/function,
//			or a type not compatible with the type that results from default argument promotions.
//
//	T va_arg(va_list ap, T)
//			Macro that expands to an expression of type T that corresponds to next parameter from 'ap'.
//			List must be initalized by va_start/va_copy, and not passed to va_end.
//			If the type (after promotions) is not compatible with T, behaviour is undefined, unless:
//					one type is signed integer, the other is unsigned integer, and value is representable as both
//					one type is a pointer to void and the other is a pointer to a character type
//
//	void va_copy(va_list dest, va_list src)			(since C99)
//			Macro that copies src to dest.
//			dest must be passed to va_end before it can be reinitalized by va_start/va_copy
//
//	void va_end(va_list ap)
//			Cleanup list 'ap' (leaving it no-longer usable)
//			Undefined if 'ap' hasn't been initalized by va_start/va_copy


long AddNumbers(const int n, ...) {
	long result = 0;
	va_list ptr;
	va_start(ptr, n);
	for (int i = 0; i < n; ++i) {
		result += (long) va_arg(ptr, int);
	}
	va_end(ptr);
	return result;
}

int LargestNumber(const int n, ...) {
	int result;
	va_list ptr;
	va_start(ptr, n);
	//result = va_start(ptr, n);		//	web example, incorrect(?)
	result = va_arg(ptr, int);			//	what geeksforgeeks meant(?)
	for (int i = 1; i < n; ++i) {
		int temp = va_arg(ptr, int);
		result = temp > result ? temp : result;
	}
	va_end(ptr);
	return result;
}


int main(int argc, char* argv[]) 
{
	long result_add = AddNumbers(10, 1,2,3,4,5,6,7,8,9,10);
	printf("result_add=(%li)\n", result_add);

	long result_max = LargestNumber(10, 1,2,3,4,5,6,7,8,9,10);
	printf("result_max=(%li)\n", result_max);

	return 0;
}

