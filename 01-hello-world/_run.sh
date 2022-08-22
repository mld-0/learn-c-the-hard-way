
#	Can ask make to build a file without needing a makefile (and provide CFLAGS).
CFLAGS="-Wall" make hello-world
#	Does 'hello-world' exist already?
#	If not, is there a source file beginning with 'hello-world'
#	Run: 'cc hello-world.c -o hello-world'

#	Run the resulting binary
./hello-world

