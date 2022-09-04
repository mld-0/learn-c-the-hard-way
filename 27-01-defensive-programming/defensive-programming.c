//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include "debug-macros.h"
#include <stdio.h>
#include <assert.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-09-04T22:24:23AEST failure vs error - (failure would be returning null, error would be aborting/throwing-exception?) [...] 'cause a failure' vs 'return an error' suggests book is using them the opposite way(?)
//	Ongoing: 2022-09-04T22:30:20AEST is the user allowed to set errno?
//	Ongoing: 2022-09-04T22:42:31AEST safer_copy proves len_from/len_to, strncpy provides only length of <source?>
//	Ongoing: 2022-09-04T22:44:08AEST how does our 'safer_copy' compare to strncpy(?) ... (is there an <official/standard-library> C string copy 'safer' than strncpy?)
//	Ongoing: 2022-09-04T22:50:23AEST 'assert(test && "msg")' is *such* a C-ism
//	Ongoing: 2022-09-04T22:56:41AEST (discussing abort vs error-codes) <logic> why book uses assert to check to/from NULL-ness, but returns -1 for an invalid length
//	Ongoing: 2022-09-04T22:59:15AEST defensive programming - always using 'sizeof(x)/sizeof(*x)' (unlike book which omits the second sizeof for char-arrays)
//	Ongoing: 2022-09-04T23:06:31AEST macro 'check' vs assert
//	}}}

//	Defensive programming strategies:
//		Never trust (always validate/sanitize) input
//		(Try to) prevent errors <(what does that even mean?)>
//		Fail early and openly
//		Document assumptions
//		Prevention over documentation
//		Automate everything (testing, building, deployment, administration, error reporting) (especially testing)
//		Priorities code clarity 
//		Question authority (don't blindly follow or reject rules)

//	(Book) what to do for each function:
//		Identify parameter preconditions, and whether violations should cause a failure or error. (Libraries should favor errors over failures).
//		Use 'assert (test && "message")' to test failure preconditions
//		For other</error> preconditions, return an error code, or use the 'check' macro to give an error message
//		Document these preconditions
//		If modifying inputs, verify they are correctly formatted when the function exits
//		Check the error</return> codes of functions that return such codes <(and then check errno)>
//		Use consistent error codes

//	(Book) preventing errors in software:
//		List all (within reason) possible errors, assign a probability, and estimate effort to prevent it
//		Rank by effort/probability
//		Remove those that are feasible, document those that are not

//	Fail early and openly:
//	C provides two choices for reporting errors:
//		Return an error code
//		Abort the process
//	Libraries should try their hardest to never abort


void naive_copy(char* to, char* from) {
	int i = 0;
	while ((to[i] = from[i]) != '\0') {
		++i;
	}
}

int safer_copy(const char* from, int from_len, char* to, int to_len) {
	//assert (from != NULL && to != NULL && "from/to must not be NULL");
	//assert (from_len >= 0 && to_len > 0 && "from_len must be >= 0 and to_len must be > 0");
	if (!from || !to) return -1;
	if (from_len < 0 || to_len <= 0) return -1;
	int max_len = from_len > to_len - 1 ? to_len - 1 : from_len;
	int i;
	for (i = 0; i < max_len; ++i) {
		to[i] = from[i];
	}
	to[to_len-1] = '\0';
	return i;
}

//	<(behaviour versus)>
//	char* strncpy(char dest, const char* src, size_t n)


int main(int argc, char* argv[]) 
{
	char from[] = "0123456789";
	char to[] = "abcdefg";
	int from_len = sizeof(from)/sizeof(*from);
	int to_len = sizeof(to)/sizeof(*to);
	printf("from[%d]=(%s), to[%d]=(%s)\n", from_len, from, to_len, to);

	int rc;
	rc = safer_copy(from, from_len, to, to_len);
	check(rc > 0, "safer_copy should succeed for valid inputs");
	check(to[to_len-1] == '\0', "C-string 'to' should be null terminated");
	check(from[from_len-1] == '\0', "C-string 'from' should be null terminated");
	printf("from[%d]=(%s), to[%d]=(%s)\n", from_len, from, to_len, to);

	rc = safer_copy(from, -1*from_len, to, to_len);
	check(rc == -1, "safer_copy should fail for negative from_len");

	rc = safer_copy(NULL, from_len, to, to_len);
	check(rc == -1, "safer_copy should fail for NULL 'from'");

	rc = safer_copy(from, from_len, NULL, to_len);
	check(rc ==  -1, "safer_copy should fail for NULL 'to'");

	return 0;
error:
	return 1;
}

