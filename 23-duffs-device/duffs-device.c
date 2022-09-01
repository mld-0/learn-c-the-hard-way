//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "debug-macros.h"
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-29T21:55:42AEST safeness of 'normal_copy', <>
//	Ongoing: 2022-08-29T21:59:42AEST 'duffs_device' purpose of extra brackets - exclude return from body of function(?)
//	Ongoing: 2022-08-29T23:08:09AEST compile with -Ofast, (and suddenly) 'normal_copy' is (marginally) beating memcpy? [...] memcpy is fast regardless of optimisation level
//	Ongoing: 2022-08-29T23:08:58AEST with -O0, duffs_device outperforms normal_copy
//	Ongoing: 2022-08-29T23:21:00AEST Minerva, with -Ofast, duffs_device is significantly faster for heap memory than stack memory (also more than 1000000 causes a crash) ... <minimal/no> difference with -O0 ... memcpy just as fast for heap/stack memory [...] is this a problem of 'very large data doesn't belong in the stack' -> is there a heap/stack difference for small arrays? <- (yes, for arrays large enough for the copy operation to register reliably as >1ms) [...] (for Cronos, performance for heap/stack arrays is <much> more similar) [...] duffs_device is much faster on Minerva vs Cronos 6ms vs 100ms (memcpy is 4ms vs 7ms) [...] rpiOpalWillow runs 86ms for duffs_device and 20ms for memcpy -> (is this a matter of Zen 1 being bad at duffs_device or x86 being bad at duffs_device?) [...] (but, even though duffs_device runs in 100ms for most tests, for the occasional test run it runs in 40ms) (one assumes this has to be cache related?) [...] (hypothesis: ARM is better at copying memory byte-by-byte, ((presumably) memcpy copies larger chunks at a time?)) [...] (Minerva is the only platform which can run duffs_device in comparable time to normal_copy) ... (and something else telling we have been missing - 'disentangled_device' outperforms 'duffs_device' on rpiOpalWillow/Cronos) [...] (hypothesis: compiler is better at optimising than we are (and Minerva (through some act of witchcraft) runs duffs_device (as fast as normal_copy))) [...] Diana results: 'normal_copy' outperforms memcpy, 'disentangled_device' is the slowest result, duffs_device = 8ms [...] (consider: we are using gcc on Linux / clang on macOS) -> repeat tests with clang [...] Cronos: memcpy (8ms), normal_copy (7ms) duffs_device is fast (19ms), zeds_device (74ms) / disentangled_device are not(?), rpiOpalWillow: memcpy (21), normal_copy (21), duffs_device (25ms) is fast, disentangled_device (31ms) is slightly slower 
//	Ongoing: 2022-09-01T21:17:50AEST Ubuntu has a (much) older verison of clang(?) 10.0.0-4ubuntu1 vs 13.1.6
//	Ongoing: 2022-09-01T21:35:51AEST how to neaten each testcase 'set-call-time-verify-output' block?
//	}}}
//	Continue: 2022-08-29T22:38:16AEST strncpy, (and other) alternatives to memcpy
//	Continue: 2022-09-01T21:06:45AEST complete run/makefile (how best to pass in -O0/-Ofast to make from '_run.sh'?)

//	void* memcpy(void* dest, void* src, size_t n)
//			copy n-bytes from 'src' to 'dest' (which must not overlap)
//			(Generally) optimal way to copy memory. Use 'memmove' for overlapping memory.
//			<(return value?)>
//
//	void* memset(void* str, int c, size_t n)
//			Copy character 'c' to 'n' bytes at 'str'
//			('c' is <treated-as/converted-to> an unsigned char).
//			Returns 'str' (pointer to memory given as argument).
//
//	clock_t clock(void)
//			Returns processor time used by program since launch.
//			Divided by 'CLOCKS_PER_SEC' to convert to seconds.
//			<(may be reset by 'system()' calls)>.


//	Duffs device is a loop unrolling technique for C which involves interleaving the do-while loop and switch-statement.
//	Do not expect Duffs device to outperform 'memcpy'. (It is much better optimised by clang than gcc).
//	It is better practice to write code for clarity and leave such optimisations to the compiler.


//	Results (heap-data, clang, -Ofast, 1MB): 
//	{{{
//	Minerva: memcpy=(17), normal_copy=(17), duffs_device=(57), zeds_device=(56), disentangled_device=(58)
//	Diana: memcpy=(30), normal_copy=(29), duffs_device=(106), zeds_device=(90), disentangled_device=(312)
//	Cronos: memcpy=(78), normal_copy=(92), duffs_device=(192), zeds_device=(740), disentangled_device=(748)
//	rpiOpalWillow: memcpy=(515), normal_copy=(773), duffs_device=(741), zeds_device=(851), disentangled_device=(776)
//	}}}
//	Results (heap-data, clang, -O0, 1MB): 
//	{{{
//	Minerva: memcpy=(16), normal_copy=(969), duffs_device=(661), zeds_device=(696), disentangled_device=(1190)
//	Diana: memcpy=(41), normal_copy=(1603), duffs_device=(924), zeds_device=(866), disentangled_device=(964)
//	Cronos: memcpy=(70), normal_copy=(3000), duffs_device=(2997), zeds_device=(3139), disentangled_device=(3122)
//	rpiOpalWillow: memcpy=(466), normal_copy=(10687), duffs_device=(4204), zeds_device=(4169), disentangled_device=(4239)
//	}}}


int normal_copy(char* from, char* to, int count) {
	int i = 0;
	for (i = 0; i < count; ++i) {
		to[i] = from[i];
	}
	return i;
}

int duffs_device(char* from, char* to, int count) {
	{
		int n = (count + 7) / 8;
		switch (count % 8) {
			case 0:
				do {
					*to++ = *from++;
					case 7:
					*to++ = *from++;
					case 6:
					*to++ = *from++;
					case 5:
					*to++ = *from++;
					case 4:
					*to++ = *from++;
					case 3:
					*to++ = *from++;
					case 2:
					*to++ = *from++;
					case 1:
					*to++ = *from++;
				} while (--n > 0);
		};
	}
	return count;
}

int zeds_device(char* from, char* to, int count) {
	{
		int n = (count + 7) / 8;
		switch (count % 8) {
			case 0:
again:		*to++ = *from++;
			case 7:
			*to++ = *from++;
			case 6:
			*to++ = *from++;
			case 5:
			*to++ = *from++;
			case 4:
			*to++ = *from++;
			case 3:
			*to++ = *from++;
			case 2:
			*to++ = *from++;
			case 1:
			*to++ = *from++;
			if (--n > 0) {
				goto again;
			}
		}
	}
	return count;
}

//	Ongoing: 2022-08-29T22:32:39AEST disentangled_device version on Wikipedia uses '*to' not '*to++' (which is incorrect?) (is this the mistake the book talked about?) [...] same is true of origional version(s)
int disentangled_device(char* from, char* to, int count) {
	{
		int n = (count + 7) / 8;
		switch (count % 8) {
			case 0: *to++ = *from++;
			case 7: *to++ = *from++;
			case 6: *to++ = *from++;
			case 5: *to++ = *from++;
			case 4: *to++ = *from++;
			case 3: *to++ = *from++;
			case 2: *to++ = *from++;
			case 1: *to++ = *from++;
		}
		while (--n > 0) {
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
			*to++ = *from++;
		}
	}
	return count;
}

int valid_copy(char* data, int count, char expects) {
	for (int i = 0; i < count; ++i) {
		if (data[i] != expects) {
			log_err("[%d] %c != %c", i, data[i], expects);
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[]) 
{
	const int BUFFER_LEN = 1000000;
	char* from = malloc(BUFFER_LEN);
	char* to = malloc(BUFFER_LEN);

	int rc = 0;
	
	clock_t time_start;
	clock_t time_end;
	double elapsed_ms;

	memset(from, 'x', BUFFER_LEN);
	check(valid_copy(from, BUFFER_LEN, 'x'), "'from' initialization failure");

	memset(to, 'y', BUFFER_LEN);
	check(valid_copy(to, BUFFER_LEN, 'y'), "'to' initialization failure");

	//	memcpy
	time_start = clock();
	memcpy(to, from, BUFFER_LEN);
	time_end = clock();
	check(valid_copy(to, BUFFER_LEN, 'x'), "'normal_copy' failed");
	elapsed_ms =  (double) (time_end - time_start) / (double) CLOCKS_PER_SEC * 1000000.0; 
	printf("memcpy, elapsed_ms=(%g)\n", elapsed_ms);


	//	<other functions?>


	//	normal_copy
	memset(to, 'y', BUFFER_LEN);
	time_start = clock();
	rc = normal_copy(from, to, BUFFER_LEN);
	time_end = clock();
	check(rc = BUFFER_LEN, "'normal_copy' failed: %d", rc);
	check(valid_copy(to, BUFFER_LEN, 'x'), "'normal_copy' failed");
	elapsed_ms =  (double) (time_end - time_start) / (double) CLOCKS_PER_SEC * 1000000.0; 
	printf("normal_copy, elapsed_ms=(%g)\n", elapsed_ms);


	//	duffs_device
	memset(to, 'y', BUFFER_LEN);
	time_start = clock();
	rc = duffs_device(from, to, BUFFER_LEN);
	time_end = clock();
	check(rc = BUFFER_LEN, "'duffs_device' failed: %d", rc);
	check(valid_copy(to, BUFFER_LEN, 'x'), "'duffs_device' failed");
	elapsed_ms =  (double) (time_end - time_start) / (double) CLOCKS_PER_SEC * 1000000.0; 
	printf("duffs_device, elapsed_ms=(%g)\n", elapsed_ms);


	//	zeds_device
	memset(to, 'y', BUFFER_LEN);
	time_start = clock();
	rc = zeds_device(from, to, BUFFER_LEN);
	time_end = clock();
	check(rc = BUFFER_LEN, "'zeds_device' failed: %d", rc);
	check(valid_copy(to, BUFFER_LEN, 'x'), "'zeds_device' failed");
	elapsed_ms =  (double) (time_end - time_start) / (double) CLOCKS_PER_SEC * 1000000.0; 
	printf("zeds_device, elapsed_ms=(%g)\n", elapsed_ms);


	//	disentangled_device
	memset(to, 'y', BUFFER_LEN);
	time_start = clock();
	rc = disentangled_device(from, to, BUFFER_LEN);
	time_end = clock();
	check(rc = BUFFER_LEN, "'disentangled_device' failed: %d", rc);
	check(valid_copy(to, BUFFER_LEN, 'x'), "'disentangled_device' failed");
	elapsed_ms =  (double) (time_end - time_start) / (double) CLOCKS_PER_SEC * 1000000.0; 
	printf("disentangled_device, elapsed_ms=(%g)\n", elapsed_ms);

	free(to); 
	free(from); 

	return 0;

error:
	if (to) free(to);
	if (from) free(from);

	return 1;
}

