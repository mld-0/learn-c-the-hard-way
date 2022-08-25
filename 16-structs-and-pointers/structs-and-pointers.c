//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-25T18:10:21AEST structs in C vs C++
//	Ongoing: 2022-08-25T18:18:46AEST 'Person_Create' vs 'Create_Person' -> the latter is a better name (but the former is probably the one to chose in C (and is what the book choses) (without some namespace to associate function with 'Person) (that is, the better solution would be 'Person::Create'?)) [...] (putting functions (i.e: the functions associated with a struct) <together> (namespace-ing))
//	Ongoing: 2022-08-25T21:45:15AEST Person_Create, (what is the better varname) (book chooses 'who', I chose 'person')
//	Ongoing: 2022-08-25T21:46:47AEST returning a struct by-value vs by-pointer (which/when, why book 'create()' function is choosing returning a pointer which must be free-ed manually
//	Ongoing: 2022-08-25T21:49:05AEST (defining a typename (so we don't have to write 'struct Person' each time) ((or) is that a C++ thing?))
//	Ongoing: 2022-08-25T21:50:06AEST (remembering C++'s lesson 'use nullptr not NULL') (<correct/best> thing to use in C is 'NULL'(?)) (C does not have nullptr)
//	Ongoing: 2022-08-25T22:07:35AEST purpose of 'typedef struct'?
//	Ongoing: 2022-08-25T22:32:29AEST (when to) set a pointer = 0 after it has been free-ed
//	Ongoing: 2022-08-25T22:45:53AEST book instructs us to step (trace) through program [...] (extra credit is to use debugger to identify leaked memory)
//	Ongoing: 2022-08-25T22:48:37AEST is NULL ever not 0? (is it ever worth taking that chance?)
//	Ongoing: 2022-08-25T23:01:30AEST (presumedly) (surely) C-structs work exactly the same when used in C++
//	Ongoing: 2022-08-25T23:08:04AEST implement 'Person_Create' in terms of 'Person_Create_ByValue' (or vice-versa)?
//	Ongoing: 2022-08-25T23:15:49AEST 'const struct Person' and 'struct const Person' mean different things (the former <>, the latter the contents of the struct are const) [...] (what does 'const struct Person' mean?) [...] (for 'const struct Person' -> the (value of the) pointer is constant, (what it points to is not?))
//	}}}

//	C vs C++ structs:
//		C requires 'struct' to be used when declaring an instance of that struct-type
//				'struct Foo foo'
//		C structs do not allow member functions
//		C structs do not have private/protected access modifiers
//		C structs do not allow static members
//		sizeof empty struct: C = 0 / C++ = 1

//	Memory functions:
//	{{{
//	malloc(<>)
//		<>
//	
//	free(<>)
//		<>
//		'free(NULL)' does nothing.
//		<(Good practice is to set a pointer to NULL after freeing it)> 
//		(dangling pointers are probably worse than (whatever problems NULL-ing a free-ed pointer might cause?))
//
//	strdup(const char* s)
//		Returns a pointer to a new string that is a duplicate of 's'.
//		This new string is allocated with 'malloc' and <can/must> be free-ed with 'free()'
//		Use to deep-copy C-strings.
//	}}}

//	A struct is a compound data type.
//	Access elements of a struct pointer with '->', and elements of a stack struct with '.'
//	C is oftern used in systems with very little memory. For such systems, using large amounts of stack space (using structs by-value) can be undesirable.

//	Struct assignment:
//	Assignment with '=' is more readable and more optimizable than with 'memcpy'.
//	Performs a shallow copy (equivalent to a 'memcpy'). Not a deep copy.
//	Primitives/pointers are copied, pointed-to memory (and data-structures at that memory) is not
//	C++ is much better suited than C for implementing deep-copy.

//	Copying structs containing heap-allocated memory makes it ambiguous who owns the pointer, leading to (among others) problems like double-frees.


struct Person {
	char* name;
	int age;
	int height;
	int weight;
};

//	Implementation by-pointer:
struct Person* Person_Create(const char* name, int age, int height, int weight) {
	struct Person *person = malloc(sizeof(struct Person));
	person->name = strdup(name);
	person->age = age;
	person->height = height;
	person->weight = weight;
	return person;
}
struct Person* Person_DeepCopy(struct Person* person) {
	return Person_Create(person->name, person->age, person->height, person->weight);
}
void Person_Destroy(struct Person* person) {
	assert (person != NULL);
	free(person->name);
	person->name = NULL;
	free(person);
}
void Person_Print(struct Person* person) {
	printf("name=(%s), age=(%i), height=(%i), weight=(%i)\n", 
			person->name, person->age, person->height, person->weight);
}


//	Implementation by-value:
struct Person Person_Create_ByValue(const char* name, int age, int height, int weight) {
	struct Person person;
	person.name = strdup(name);
	person.age = age;
	person.height = height;
	person.weight = weight;
	return person;
}
struct Person Person_DeepCopy_ByValue(struct Person person) {
	return Person_Create_ByValue(person.name, person.age, person.height, person.weight);
}
void Person_Destroy_ByValue(struct Person person) {
	free(person.name);
	person.name = NULL;
}
void Person_Print_ByValue(struct Person person) {
	printf("name=(%s), age=(%i), height=(%i), weight=(%i)\n", 
			person.name, person.age, person.height, person.weight);
}


int main()
{
	struct Person x;	//	correct
	//Person x;			//	invalid

	//	Using pointers:
	struct Person* joe = Person_Create("Joe Alex", 32, 64, 140);
	struct Person* frank = Person_Create("Frank Blank", 20, 73, 180);
	printf("joe=(%p)\n", joe);
	printf("frank=(%p)\n", frank);
	Person_Print(joe);
	Person_Print(frank);
	struct Person* joe_copy = Person_DeepCopy(joe);
	Person_Destroy(joe);
	Person_Destroy(frank);
	Person_Destroy(joe_copy);
	joe = NULL;
	frank = NULL;
	printf("\n");

	struct Person larry = Person_Create_ByValue("Larry", 15, 9, 9);
	printf("larry=(%p)\n", &larry);
	Person_Print_ByValue(larry);
	struct Person larry_copy = Person_DeepCopy_ByValue(larry);
	Person_Destroy_ByValue(larry);
	Person_Destroy_ByValue(larry_copy);
	//	Ongoing: 2022-08-25T23:13:14AEST (no way to destroy a by-value (stack) struct?) (just let it go out of scope?)


	return 0;
}

