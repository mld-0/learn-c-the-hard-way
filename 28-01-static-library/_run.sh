make 
./hello

#	List all modules in library
ar -t "libfoo.a"
echo ""
otool -I "libfoo.a"
echo ""
nm "libfoo.a"
echo ""

otool -hv "libfoo.a"

