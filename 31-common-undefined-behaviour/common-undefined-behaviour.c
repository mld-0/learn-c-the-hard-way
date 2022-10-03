//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-10-01T23:11:52AEST integer overflow is 'INT_MAX+1', and underflow is 'INT_MIN-1'?
//	}}}
//	TODO: 2022-10-01T23:51:28AEST learn-c-the-hard-way, common-undefined-behaviour, examples of catching UB using tools mentioned (valgrind/-fcatch-undefined-behavior/clang-static-analyser/llvm-klee/c-semantics

//	Undefined behaviour is where C places no requirement on what the compiler must do for a given construct / operation.
//	There is no guarantee of a compiler or runtime error for undefined behaviour. 
//	Any program that relies on undefined behaviour is by definition not portable.
//	A strictly conforming program is one that has no undefined behaviour.
//	A program that behaves differently for different levels of optimization is indicative of undefined behaviour.

//	Unspecified behaviour is where two-or-more behaviours are permitted (eg: order of evaluation)

//	Implementation defined behaviour (eg: whether signed right-shift is arithmetic/logical, whether identical string literals are distinct)

//	Locale-specified behaviour (eg: whether 'islower()' is true for anything other than latin letters)


//	<(book gives a list of 20, using the sort of hard-to-understand language one might expect from cppreference)>
//	1)	An object is reffered to outside its lifetime
//	2)	Conversion to/from an integer type produces a value outside the range that can be represented
//	3)	Two declarations of the same object/function specify types that are not compatible
//	4)	An lvalue of register storage class array type is converted to a pointer, 
//	5)	Attempt to modify a string literal
//	6)	Object value accessed by something other than lvalue of an allowed type
//	7)	Attempt to modify function/operator result, or to access it after the next sequence point
//	8)	Division '/' or modulus '%' by zero
//	9)	Object is assigned to inexactly-overlapping object, or object with incompatible type
//	10)	A constant expression in an intializer is not: an arithmetic expression, null pointer constant, address constant (optionally plus/minus arithemtic expression)
//	11)	Attempt to modify const object though non-const lvalue
//	12)	An external function is inline, but not also defined in the same translation unit
//	13)	Value of unnamed member of structure/union is used
//	14)	Function terminating '}' is reached <(it doesn't return?)>, and its value is used by the caller
//	15)	A file with the same name as a standard header is included anywhere the compiler searches for source files
//	16)	Value passed to character-handling function is neither EOF nor valid as an unsigned char
//	17)	Result of integer arithemetic/conversion cannot be represented <(using the type of that expression/function?)>
//	18)	FILE pointer is used after file is closed
//	19.1)	<(A conversion specification for a formatted output function uses a # or 0 flag with a conversion specifier other than those described)>
//	19.2)	<(An s conversion specifier is encountered by one of the for‐ matted output functions, and the argument is missing the null terminator (unless a precision is specified that does not require null termination))>
//	19.3)	The contents of the array supplied in a call to 'fgets()', 'gets()', 'fgetws()' are used after a read error occurred
//	20.1)	A pointer returned from 'calloc()', 'malloc()', 'realloc()' with zero requested size is used to access an object
//	20.2)	A freed pointer is used
//	20.3)	A pointer not allocated by 'calloc()', 'malloc()', 'realloc()' is freed
//	20.4)	A pointer is freed twice
//	<(book gives many sub-items for each time)>


//	The For-Dummies (tutorialspoint) list of UBs:
//	LINK: https://www.tutorialspoint.com/undefined-behaviour-in-c-and-cplusplus
//	1)	divide by zero
//	2)	uninitalized variable used
//	3)	null pointer dereference
//	4)	accessing outside bounds of array
//	5)	signed overflow/underflow
//	6)	modifying string literal


//	LINK: https://stackoverflow.com/questions/3575350/sequence-points-in-c
//	{{{
//	When a sequence point occurs, it basically means that you are guaranteed that all previous operations are complete. Changing a variable twice without an intervening sequence point is one example of undefined behaviour.

//	Ongoing: 2022-09-09T00:20:17AEST and how many of these do you know?
//	C11 lists sequences points as:
//	Between the evaluation of the function designator, and actual arguments in a function call, and the actual call
//	Between the evaluations of the operands of operators '&&', '||', and ','
//	Between the first and second/third operand of conditional operator '? :'
//	The end of a full declarator
//	Between full expressions:
//		An initializer
//		Expression in an expression statement
//		Controlling expression of 'if' / 'switch'
//		Controlling expression of 'do' / 'while'
//		Each expression in a 'for'
//		Expression in a return statement
//	Immediately before a library function returns
//	After the actions associated with each formatted input/output function conversion specifier
//	Immediately before/after each call to a comparison function
//	Between any call to comparison function and movement of objects passed as arguments to that call

//	}}}


//	LINK: https://www.opensourceforu.com/2018/10/understanding-undefined-behaviour-in-c/
//	{{{

//	When a sequence point occurs, it basically means that you are guaranteed that all previous operations are complete. Changing a variable twice without an intervening sequence point is one example of undefined behaviour.
int f(int x) {
	return x;
}
void changing_variable_twice_without_intervening_sequence_point() {
	int a = 10;
	int b = a++ + a++;				//	undefined
	int c = f(a++) + f(a++);		//	undefined
	printf("a=(%d), b=(%d), c=(%d)\n", a, b, c);
	int A[10];
	int i = 2;
	A[i++] = i;						//	undefined
	A[i] = i++;						//	undefined
}

//	Ongoing: 2022-09-09T00:35:25AEST definition of a 'string literal'
//	<>

//	Ongoing: 2022-09-09T00:30:30AEST silence warnings for a given line?
void modifying_string_literals() {
	//	<(This is why string literals should always be 'const char*'?)>
	char *p1 = "string-literal";				//	string literal is stored in read-only memory

	//	No compiler warning (crashes at -O0, does nothing at -O3)
	p1[0] = 'S';						//	undefined
	*(p1+1) = 'T';						//	undefined
	printf("p1=(%s)\n", p1);

	//	Not a 'string literal' (perfectly valid)
	char p2[] = "array-string";
	p2[0] = 'A';
	*(p2+1) = 'R';
	printf("p2=(%s)\n", p2);
}

//	signed integer overflow is undefined behaviour
void signed_overflow() {
	int x = INT_MAX;
	//	The compiler is allowed to replace 'x + 1 > x' with 'True'
	//	Minerva: no combination of flags allows the first True case 'x+1 > x'
	if (x + 1 > x) {
		printf("x+1 > x\n");
	} else {
		printf("x+1 == x\n");
	}

	//	-fstrict-overflow
	//	Assume signed overflow is undefined
	//	<implications?>

	//	-fno-strict-overflow
	//	Do not assume strict signed overflow rules
	//	<implications?>
}

//	}}}


//	LINK: https://en.cppreference.com/w/c/language/behavior
//	{{{

//	Ongoing: 2022-09-09T01:11:11AEST assigning a negative value to an 'unsigned int' doesn't get a warning?
int check_signed_overflow_wrap(int x) {
	return x+1 > x;
	x = -1;
}


void access_out_of_bounds() {
	//	<>
}

//	<(not the most meaningful/helpful examples?)>
//	<>

//	}}}


//	(about undefined behaviour)
//	LINK: http://blog.llvm.org/2011/05/what-every-c-programmer-should-know.html
//	LINK: http://blog.llvm.org/2011/05/what-every-c-programmer-should-know_14.html
//	LINK: http://blog.llvm.org/2011/05/what-every-c-programmer-should-know_21.html
//	{{{

//	Uninitalized variables are undefined

//	Signed overflow/underflow is undefined
//	'INT_MAX+1' is not guaranteed to be 'INT_MIN'
//	This allows optimiziations like replacing 'x+1 > x' with 'true', and 'x*2/2' with 'x'
//	It is especially important for optimizations that involve the number of iterations of a loop
//	(unsigned makes it harder to rule out a loop not being potentially infinite)
//	('undefined' means the compiler can optimise as if it will never happen)


//	<(Avoid mixing signed and unsigned integers)>
//	<(claim: only use unsigned integers for bit manipulation)>
//	(what about using unsigned integers because something else in the expression is unsigned, i.e: vector.size()?)
//	<(claim: signed integers allow better loop optimization(?))>
//	<(claim: do not mix signed an unsigned integers)>
//	<(Google style guide discourages use of unsigned types)>
//	Expression: 'for (unsigned int n = 10; n >= 0; n --) { }' will loop forever 

//	<C++> Potentially disaterous (and yet, -Wall is not enough to catch it, need to use -Wextra for warnings about comparing signed/unsigned types) ... (the moral of the story is, always use -Wall and -Wextra?)
//vector<int> a = { 1, 2, 3 };
//for (int i = 0; i < a.size()-1; ++i) { }

//	Shifting 'uint32_t' by >=32 is undefined

//	Dereferencing a random value as a pointer is undefined
//	Dereferencing a null pointer is undefined
//	(LLVM will crash if dereferencing a volatile null pointer)

//	Out of bounds array access is out of bounds

//	Dereferencing a pointer as the wrong type is undefined

//	Undefined behaviour can differ at different compiler optimization levels

//	If we derefence a pointer, the compiler can assume that pointer was not none. 
//	That is, for:
void derefence_and_check_for_null(int* p) {
	int x = *p;
	if (p == NULL) { return; }		//	optimized out
	*p = 53;
}
//	The null check can be optimized out by the compiler

//	Since the compiler assumes overflow cannot happen, it is permitted to optimize out checks for it
void check_for_overflow(int size) {
	if (size > size+1) { exit(1); }		//	optimized out
	char* str = malloc(size+1);
}
//	Use 'size == INT_MAX' instead

//	It can be good practice to debug both with and without optimizations

//	There is no reliable way to determine if a large C codebase contains undefined behaviour

//	Checking for undefined behaviour:
//	Use '-Wall -Wextra'
//	valgrind: memcheck tool, 
//		detects uninitalized memory access, leaks, bad frees, and passing overlapping src/dest to functions like 'memcpy()'
//	Clang has experimental '-fcatch-undefined-behaviour'
//		check various undefined behaviour (simple out-of-range errors)
//	Clang supports '-ftrapv': trap signed overflow at runtime
//	Don't ignore compiler warnings, use '-Wall -Wextra'
//	Clang static analyser
//		performs deeper compile time analysis
//	LLVM 'Klee'
//		Try every possible path though a code base (not practical for large projects)
//	C-semantics tool

//	LLVM takes fewer liberties with UB than gcc
//		arithemtic on undefined values produces an undefined value, not undefined behaviour
//		stores-to/calls-through null pointers are trapped
//		some limited attempts are made to deduce what the programmer intended 

//	Some flags to reduce UB at a tradeoff to performance
//		-fwrapv	
//		-fno-strict-aliasing

//	}}}


//	LINK: https://riptutorial.com/c/topic/364/undefined-behavior
//	(Link provides examples for each of:)
//	{{{
//	Accessing memory beyond allocated chunk 
//	Addition or subtraction of pointer not properly bounded 
//	Bit shifting using negative counts or beyond the width of the type 
//	Conversion between pointer types produces incorrectly aligned result 
//	Copying overlapping memory 
//	Data race 
//	Dereferencing a null pointer 
//	Dereferencing a pointer to variable beyond its lifetime 
//	Division by zero 
//	Freeing memory twice 
//	Inconsistent linkage of identifiers 
//	Missing return statement in value returning function 
//	Modify string literal 
//	Modifying a const variable using a pointer 
//	Modifying any object more than once between two sequence points 
//	Modifying the string returned by getenv, strerror, and setlocale functions 
//	Passing a null pointer to printf %s conversion 
//	Read value of pointer that was freed 
//	Reading an uninitialized object that is not backed by memory 
//	Returning from a function that's declared with `_Noreturn` or `noreturn` function specifier 
//	Signed integer overflow 
//	Use of an uninitialized variable 
//	Using fflush on an input stream 
//	Using incorrect format specifier in printf 
//	}}}

//	LINK: https://blog.regehr.org/archives/213
//	LINK: https://blog.regehr.org/archives/226
//	LINK: https://blog.regehr.org/archives/232
//	{{{

//	For undefined operations, e.g: signed overflow, we cannot assume the behaviour will be that of our hardware implementation 

//	'undefined' means that 'printf("%d", j++ <= j)' can print 0, 1, or 42.

//	A function falls into 3 categories: defined for all inputs, undefined for some inputs, undefined for all inputs

//	Example: Function only defined for: (b != 0) && (!((a == INT32_MIN) && (b == -1)))
int32_t unsafe_div_int32_t(int32_t a, int32_t b) {
	return a / b;
}

//	Example: Function only defined for: (a != INT_MAX)
int stupid(int a) {
	//	By ignoring undefined behaviour, the compiler can optimize this to 'return 1'
	return (a+1) > a;
}

//	Prediction: In the long run, unsafe programming languages will not be used by mainstream developers, but rather reserved for situations where high performance and a low resource footprint are critical
//	Suggestions:
//		Enable and heed compiler warnings, preferably using multiple compilers
//		Use static analyzers (like Clang's, Coverity, etc.) to get even more warnings
//		Use compiler-supported dynamic checks; for example, gcc's -ftrapv flag generates code to trap signed integer overflows
//		Use tools like Valgrind to get additional dynamic checks
//		When functions are "type 2" as categorized above, document their preconditions and postconditions
//		Use assertions to verify that functions' preconditions are postconditions actually hold
//		Particularly in C++, use high-quality data structure libraries
//		Basically: be very careful, use good tools, and hope for the best.

//	Undefined code that works today can be broken tomorow as compiler optimization improves

//	Negating INT_MIN is undefined behaviour

//	Claim: Creating a large piece of safety-critical or security-critical code in C or C++ is the programming equivalent of crossing an 8-lane freeway blindfolded.

//	C requires that side-effecting operations be performed in program order
//	However where there are no dependencies between instructions, an undefined expression can be re-ordered to before an expression with side-effects, leading to the side-effecting expression not being exectued, even though the line containing the undefined instruction is after it in program order.

//	Example: calling 'foo(1,0)' may not output text before crashing
void bar() {
	setlinebuf(stdout);
	printf("hello!\n");
}
void foo(int a, int b) {
	bar();
	int x = a % b;
}
//void example_reordering() {
//	foo(1,0);
//}

//	<(A possible solution (doesn't work here?) is to add)>
//		asm volatile ("" : : : "memory");
//	A gcc idiom that does nothing, but is creates a dependency with everything


//	The C99 standard states that:
//	Accessing a volatile object, modifying an object, modifying a file, or calling a function that does any of those operations are all side effects, which are changes in the state of the execution environment
//	(However, no optimizing C/C++ compiler consideres modifying an object to be a side effect)

//	}}}


//	LINK: https://www.yodaiken.com/2021/05/19/undefined-behavior-in-c-is-a-reading-error/
//	{{{

//	[...]

//	OSes written in C are not strictly standards compilent C - they require all manner of flags to specify compiler behaviour

//	}}}


//	C99 List of Undefined Behavior
//	LINK: https://gist.github.com/Earnestly/7c903f481ff9d29a3dd1
//	<(a comprehensive list?)>
//	{{{
//	}}}


//	Clang Undefined Behaviour Sanitizer
//	LINK: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
//	{{{
//	}}}


int main(int argc, char* argv[]) 
{
	changing_variable_twice_without_intervening_sequence_point();
	modifying_string_literals();
	signed_overflow();
	//example_reordering();

	return 0;
}

