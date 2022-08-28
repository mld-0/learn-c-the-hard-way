//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-28T14:08:26AEST struct alignment, x86 vs ARM
//	Ongoing: 2022-08-28T14:29:52AEST meaning of 'int x:2' (from example where struct of 2 ints (only) has size = 4) <(specify that we use 2/6 bits respectively)>
//	Ongoing: 2022-08-28T14:48:05AEST How to turn 'pragma pack (1)' back off?
//	Ongoing: 2022-08-28T15:20:19AEST (what that works) (to minimise the size of a struct, declare the variables in decending order by size)
//	Ongoing: 2022-08-28T16:18:35AEST checking an address is n-byte aligned -> its address (hex) is divisble by n without remainder?
//	}}}
//	Continue: 2022-08-28T16:46:25AEST cleanup

//	How C/C++ structs are laid out in memory is an implementation detail.
//	Do not rely on the memory layout of a struct being the same on different platforms.
//	Most CPUs have performance penalties for data not aligned to 2/4/8 bytes. Some CPUs do not allow misaligned data.

//	The compiler will pad structs to ensure each member variable is aligned on an appropriate boundry.
//	The size returned by 'sizeof()' includes this padding.
//	Use 'offsetof()' to find the offset of member variables in a struct.

//	<(Generally, 1-byte variables are aligned to 1-byte boundires, 2-byte variables to 2-byte boundries, ect.)>

//	<(POD C++ struct - plain-old-data, a struct containing <only> variables)>

//	For C structs <(/ POD C++ structs)>
//	The address of the struct is the same as the address of the first element (there is no padding at the beginning).
//	The member variables are laid out in the order they are declared.

//	To remove padding from structs, (represneting on-disk structures correctly) use:
//	'#pragma pack (1)'
//	This can cause code bloat and inhibit optimizations (and should only be used for data serialization).
//	This is a Microsoft feature supported by gcc/clang
//	{{{
//	The n value below always is required to be a small power of two and specifies the new alignment in bytes.
//	#pragma pack(n) simply sets the new alignment.
//	#pragma pack() sets the alignment to the one that was in effect when compilation started (see also command line option -fpack-struct[=] see Code Gen Options).
//	#pragma pack(push[,n]) pushes the current alignment setting on an internal stack and then optionally sets the new alignment.
//	#pragma pack(pop) restores the alignment setting to the one saved at the top of the internal stack (and removes that stack entry). Note that #pragma pack([n]) does not influence this internal stack; thus it is possible to have #pragma pack(push) followed by multiple #pragma pack(n) instances and finalized by a single #pragma pack(pop).
//	}}}
//	Continue: 2022-08-28T16:49:32AEST turn '#pragma pack' on/off (on for specific structs, off elsewhere)


//	LINK: http://www.catb.org/esr/structure-packing/
//	<(For a vanilla ISA)>
//	(1 byte) char can start on any byte address
//	(2 byte) short must start on an even address
//	(4 byte) int/float must start on a 4-byte aligned address
//	(8 byte) long/double must start on a 8-byte aligned address

//	<(The value of padding bits is undefined (they are not zeroed))>

//	The order of primative variables in a struct is unlikely to significantly effect size.
//	However, the order of structs (and other nonscalar variables) can.

//	<(Arrays are contiguous in memory)>
//	Array elements are padded up to the following stride address.

//	(In general), a struct instance will have the alignment of its widest scalar member. <(including nested structs?)>

//	When declaring an array of structs, padding may be added after the end of the last variable so that the first address following the structure has the same alignment as the structure.
//	This ensures that each element of an array of that struct is aligned with the width of its widest scalar member.

//	Will be padded with 7 bytes for a total size of 16
struct foo3 {
	char *p;
	char c;
	//	char pad[7];
};

//	Will be padded with 1 byte for a total size of 4
struct foo4 {
	short s;
	char c;
	//	char pad[1];
};

//	If a struct has struct member variables, the inner struct will have the alignment of its longest member variable.
//	<(Largest scalar member = char* = 8-bytes -> 'foo5' is 8-byte aligned? (the reason it is 24bytes regardless of whether 'char c' comes before/after 'foo5_inner')> 
struct foo5 {				//	24 bytes
	char c;
	//	char pad[7];
	struct foo5_inner {		//	16 bytes
		char* p;
		short x;
		//	char pad[6]
	} inner;
};
//	This structure wastes 13 bytes (54%)
//	(This shows) Nested structs can be inefficent users of space

//	Ongoing: 2022-08-28T15:46:59AEST reordering 'foo5' does *not* make it smaller?

//	Bitfield:
//	Declare fields of a given number of bits width
//	Will be packed as tight as possible.
struct foo7 {
	int flip:1;
	int nybble:4;
};
//	Continue: 2022-08-28T15:54:20AEST (what even are bitfields) (and then, how the rules of padding/packing apply)

//	Slop occurs:
//	1)	Where a larger type follows a smaller one
//	2)	Where a struct ends before its stride address

//	Structure reordering:
//	To minimize the size of a struct, declare the variables in decending order by size.
//	<(Alternatively, declare member variables in ascending order by size)>
struct foo10 {		//	sizeof = 24
	char c;
	//	char pad[7];
	struct foo10* p;
	short x;
	//	char pad[6]
};
struct foo11 {		//	sizeof = 16
	struct foo11* p;
	short x;
	char c;
	//	char pad[5]
};

//	Example: The minimium size for a struct can be achieved with orders order than ascending/descending
struct foo13 {		//	sizeof = 40
	int32_t i;
	int32_t i2;
	char octet[8];
	int32_t i3;
	int32_t i4;
	int64_t l;
	int32_t i5;
	int32_t i6;
	//	No padding (100% efficent)
};

//	The C standard does not specify which type is to be used for a given enum.

//	'long double' can be 80/128 bits, and some 80-bit platforms pad it to 96/128

//	Linux on x86 may only require doubles to be 4-byte aligned

//	Sometimes, a certain struct order may be important for readability reasons.
//	It is benifical to align frequently accessed structures with the cache line. The cache line is generally 64/32-bits.
//	Keeping related/co-accessed data adjacent in a structure can help with cache performance.

//	<(To minimize expensive bus traffic, you should arrange your data so that reads come from one cache line and writes go to another in your tighter loops)>

//	Replace multiple boolean flags with 1-bit bitfields, located where there would otherwise be slop.
//	<(This has an access time penalty, which *may* be made up for by reduced cache misses)>

//	In general, look to shorted data field sizes.

//	<(The riskiest form of packing is to use unions)> (Which should be verified with regresion testing)

//	The only really compelling reason to use '#pragma pack' is to match struct laytout to bit-level hardware/protocol requirements.


//	Use '-Wpadded' to generate messages (warnings) about alignment holes/padding.

//	Use 'static_assert' to check assumptions about struct sizes
struct test_struct_1 {
	short s;
	char c;
	//	char pad[1];
};
static_assert(sizeof(struct test_struct_1) == 4, "'sizeof(test_struct_1)!=4', check padding assumptions");

//	'pahole' is a utility for describing padding/aligment/cache-line-boundaries

//	Exception cases exist for exotic hardware.


//	C-structs versus other languages:
//	C++ is C-like, except that classes (using virtual functions / multiple-inheritance?) may ignore the rule that the address of a struct is the address of its first member.
//	Go is similar to C. It does not have an equivalent of the '#pragma pack'. On 32-bit platforms, 64-bit fields only need to be 32-bit aligned. The compiler may re-order variables (but usually doesn't).
//	Rust follows C-like rules if given 'repr(C)'. Otherwise order/padding/packing is determined by the compiler.
//	Swift is exactly C-like. It does not have an equivalent of the '#pragma pack'.


struct S1 {
   int a;
   char b;
   int c;
   double z;
};


//	Example: The order of variables can <change/effect> the size of the resulting struct
//	To minimise the size of a struct, declare the member variables in decending order by size.
struct ST1 		//	sizeof = 24
{
   char ch1;
   short s;
   char ch2;
   long long ll;
   int i;
};
struct ST2 		//	sizeof = 16
{
   long long ll; // @ 0
   int i;        // @ 8
   short s;      // @ 12
   char ch1;     // @ 14
   char ch2;     // @ 15
};


int main(int argc, char* argv[]) 
{
	printf("int+char+int+double=(%lu)\n", sizeof(int)+sizeof(char)+sizeof(int)+sizeof(double));
	printf("sizeof(struct S1)=(%lu)\n", sizeof(struct S1));
	printf("offsetof(struct S1, a)=(%lu)\n", offsetof(struct S1, a));
	printf("offsetof(struct S1, b)=(%lu)\n", offsetof(struct S1, b));
	printf("offsetof(struct S1, c)=(%lu)\n", offsetof(struct S1, c));
	printf("offsetof(struct S1, z)=(%lu)\n", offsetof(struct S1, z));
	printf("\n");

	printf("sizeof(struct ST1)=(%lu)\n", sizeof(struct ST1));
	printf("sizeof(struct ST2)=(%lu)\n", sizeof(struct ST2));
	printf("\n");

	printf("sizeof(struct foo3)=(%lu)\n", sizeof(struct foo3));
	printf("sizeof(struct foo4)=(%lu)\n", sizeof(struct foo4));
	printf("sizeof(struct foo5)=(%lu)\n", sizeof(struct foo5));
	printf("\n");

	printf("sizeof(struct foo10)=(%lu)\n", sizeof(struct foo10));
	printf("sizeof(struct foo11)=(%lu)\n", sizeof(struct foo11));
	printf("sizeof(struct foo13)=(%lu)\n", sizeof(struct foo13));
	printf("\n");

	return 0;
}
