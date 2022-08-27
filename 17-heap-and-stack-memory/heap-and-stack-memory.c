//	{{{3
//	vim: set tabstop=4 modeline modelines=10:
//	vim: set foldlevel=2 foldcolumn=2 foldmethod=marker:
//	{{{2
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//	Ongoings:
//	{{{
//	Ongoing: 2022-08-26T01:30:05AEST (for some reason) 'int* x' for C++, 'int *x' for C(?)
//	Ongoing: 2022-08-26T02:09:25AEST 'malloc' function which dies if it fails?
//	Ongoing: 2022-08-26T02:15:44AEST Database_create, why book declares 'i' outside for-loop?
//	Ongoing: 2022-08-26T02:26:57AEST all copied (except 'main' -> don't tell me there isn't a mistake somewhere?)
//	Ongoing: 2022-08-26T02:28:21AEST 'char* x[]' vs 'char (*x)[]'  
//	Ongoing: 2022-08-26T02:54:38AEST a db-browser that can handle our '.db' <format> <created/used> here
//	Ongoing: 2022-08-27T19:58:32AEST contention: variable name 'is_set' is (so much) better than 'set'
//	Ongoing: 2022-08-27T20:05:31AEST -1 is a better default for 'id' than 0? (and checking 'id < 0' is a better error condition than 'argc < 4'?)
//	Ongoing: 2022-08-27T21:17:00AEST alter to use variable length MAX_DATA / MAX_ROWS
//	Ongoing: 2022-08-27T22:12:31AEST typedef of struct(?) (if nothing else avoids need to type 'struct' each time?)
//	}}}
//	Continue: 2022-08-27T22:16:26AEST struct padding/packing

//	The stack stores local variables.
//	The heap stores memory allocated by 'malloc()'.
//	Large or variable amounts of data belong on the heap.

//	<(The size of a struct is the size of all its variables, or 0 for an empty struct (vs 1 for C++))>

//	It is advisable to have one 'create' function responsible for all memory allocations, and one 'delete' function responsible for all memory deallocations for a given struct.

//	Initalizing a struct:
//	'struct Address addr = { .id = i, .set = 0 }'
//	Any variables not specified are zeroed.
//	Alternatively, a list can be used, where the member variables are supplied in-order.

//	struct padding/packing:
//	<(how structs are stored in memory)>


//	FILE
//		A typedef to a struct containing data about a filehandle.

//	char* strncpy(char* dest, const char* src, size_t n)
//		Copy up to n characters from src -> dest. Remaining 'n' characters are zeroed.
//		Will not null-terminate 'dest' if 'src' is being truncated. 
//		Returns pointer to 'dest' (copied string)
//		<(A null result indicates failure (or that 'dest' was never valid to begin with?))>
//		<(Good practice to set last element of 'dest' to null)>.
//		<(alternatives that avoid this problem?)>

//	FILE* fopen(const char* filename, const char* mode)
//		Opens filename and associates it with stream to which a pointer is returned.

//	int fclose(FILE* stream)
//		Closes a stream associated with a FILE pointer and flushes all buffers.
//		Returns 0 if successful, or EOF upon failure.

//	size_t fread(void* ptr, size_t size, size_t nmemb, FILE* stream)
//		Reads data from 'stream' into 'ptr'. 
//		Reads 'nmemb' elements, each one being 'size' bytes.

//	int fflush(FILE* stream)
//		Flushes output buffer of a stream.

//	void rewind(FILE* stream)
//		Sets position of a given stream to the beginning.

//	void perror(const char* str)
//		Print error associated with 'errno', along with 'str'.

//	errno
//		Variable set by (certain) system/library-function calls to indicate an error.
//		Significant only when return value of said function call returns an error.
//		Functions that succeed are allowed to change errno.


//	We load/save our database by reading-from/writing-to a sizeof(struct Database) sized piece of memory
//	We deal with errors by calling 'exit(1)' (after printing a suitable error message) (responsibility of 'die()'), and leaving the OS to clean up whatever resources we have acquired.

#define 	MAX_DATA	512
#define 	MAX_ROWS	100

struct TestStruct {
	char a;
	char b;
};

struct Address {
	int id;
	int is_set;
	char name[MAX_DATA];
	char email[MAX_DATA];
};

struct Database {
	struct Address rows[MAX_ROWS];
};

struct Connection {
	FILE* file;
	struct Database* db;
};

void die(const char* msg) {
	if (errno) {
		perror(msg);
	} else {
		fprintf(stderr, "Error: %s\n", msg);
	}
	exit(1);
}

void Address_print(struct Address* addr) {
	int id = addr->id;
	char* name = addr->name;
	char* email = addr->email;
	printf("%d %s %s\n", id, name, email);
}

void Database_load(struct Connection* conn) {
	struct Database* db = conn->db;
	FILE* file = conn->file;
	int rc = fread(db, sizeof(struct Database), 1, file);
	if (rc != 1) {
		die("Failed to load database");
	}
}

struct Connection* Database_open(const char* filename, char mode) {
	struct Connection* conn = malloc(sizeof(struct Connection));
	if (!conn) {
		die("Memory error");
	}
	conn->db = malloc(sizeof(struct Database));
	if (!conn->db) {
		die("Memory error");
	}
	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if (conn->file) {
			Database_load(conn);
		}
	}
	if (!conn->file) {
		die("Failed to open file");
	}
	return conn;
}

void Database_close(struct Connection* conn) {
	if (conn) {
		if (conn->file) {
			fclose(conn->file);
		}
		if (conn->db) {
			free(conn->db);
		}
	}
}

void Database_write(struct Connection* conn) {
	struct Database* db = conn->db;
	FILE* file = conn->file;
	rewind(file);
	int rc = fwrite(db, sizeof(struct Database), 1, file);
	if (rc != 1) {
		die("Failed to write to database");
	}
	rc = fflush(file);
	if (rc == -1) {
		die("Cannot flush database");
	}
}

void Database_create(struct Connection* conn) {
	struct Database* db = conn->db;
	for (int i = 0; i < MAX_ROWS; ++i) {
		struct Address addr = { .id = i, .is_set = 0, };
		db->rows[i] = addr;
	}
}

void Database_set(struct Connection* conn, int id, const char* name, const char* email) {
	struct Database* db = conn->db;
	struct Address* addr = &db->rows[id];
	if (addr->is_set) {
		die("Already is_set <(delete it first)>");
	}
	char* res = strncpy(addr->name, name, MAX_DATA);
	addr->name[MAX_DATA-1] = '\0';
	if (!res) {
		die("Name copy failed");
	}
	res = strncpy(addr->email, email, MAX_DATA);
	addr->email[MAX_DATA-1] = '\0';
	if (!res) {
		die("Email copy failed");
	}
	addr->is_set = 1;
}

void Database_get(struct Connection* conn, int id) {
	struct Address* addr = &conn->db->rows[id];
	if (!addr) {
		die("Failed to get row address");
	}
	if (!addr->is_set) {
		die("ID is not set up");
	}
	Address_print(addr);
}

void Database_delete(struct Connection* conn, int id) {
	struct Database* db = conn->db;
	struct Address addr = { .id = id, .is_set = 0, };
	db->rows[id] = addr;
}

void Database_list(struct Connection* conn) {
	struct Database* db = conn->db;
	for (int i = 0; i < MAX_ROWS; ++i) {
		struct Address* addr = &db->rows[i];
		if (addr->is_set) {
			Address_print(addr);
		}
	}
}

void handle_action(struct Connection* conn, char action, int argc, char* argv[]) {
	int id = 0;
	if (argc >= 4) {
		id = atoi(argv[3]);
	}
	if (id >= MAX_ROWS) {
		die("There is not that many records");
	}
	const char* name = NULL;
	const char* email = NULL;
	if (argc >= 6) {
		name = argv[4];
		email = argv[5];
	}
	switch (action) {
		case 'c':
			Database_create(conn);
			Database_write(conn);
			break;
		case 'g':
			if (id < 0) {
				die("Need an (id) to get");
			}
			Database_get(conn, id);
			break;
		case 's':
			if (!name || !email) {
				die("Need (id, name, email) to set");
			}
			Database_set(conn, id, name, email);
			Database_write(conn);
			break;
		case 'd':
			if (id < 0) {
				die("Need (id) to delete");
			}
			Database_delete(conn, id);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid action: c=create, g=get, s=set, d=del, l=list");
	}
}

void report_struct_sizeof() {
	fprintf(stderr, "sizeof(struct Address)=(%lu)\n", sizeof(struct Address));			//	1032 = 512*2+4*2
	fprintf(stderr, "sizeof(struct Database)=(%lu)\n", sizeof(struct Database)); 		//	103200 = 1032*100
	fprintf(stderr, "sizeof(struct Connection)=(%lu)\n", sizeof(struct Connection)); 	//	16 = 8*2
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		die("Usage: cmd <dbfile> <action> [params]");
	}

	char* filename = argv[1];
	char action = argv[2][0];

	struct Connection* conn = Database_open(filename, action);
	handle_action(conn, action, argc, argv);
	Database_close(conn);

	return 0;
}

