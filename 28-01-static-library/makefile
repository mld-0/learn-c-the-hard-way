
#	-L	locate the path of the library
#	-l	give the name of the static library (automatically prefixes 'lib')
hello: hello.o libfoo.a
	cc -o hello hello.o -L. -lfoo

#	No need to use -L if library is in /usr/lib or /usr/local/lib ('man ld' to <get/verify> those paths)

#	-c	compile only
hello.o: hello.c
	cc -c hello.c -I`pwd`

#	ar:
#		-c	create
#		-r	replace
#		-s	create index
#	Note that ar arguments work without the '-' 
libfoo.a: foo.o foo2.o
	ar -rcs libfoo.a foo.o foo2.o
#alternatively: <('-static' is not an option on macOS)>
#	libtool -static -o libfoo.a foo.o foo2.o

foo.o: foo.c
	cc -c foo.c

foo2.o: foo.c
	cc -c foo2.c

clean:
	rm -f libfoo.a
	rm -f *.o

