make all
./heap-and-stack-memory "db.dat" c
./heap-and-stack-memory "db.dat" s 1 "zed" 	"zed@zedshaw.com"
./heap-and-stack-memory "db.dat" s 2 "frank" "frand@zedshaw.conm"
./heap-and-stack-memory "db.dat" s 3 "joe" 	"joe@zedshaw.conm"

./heap-and-stack-memory "db.dat" l
echo ""

./heap-and-stack-memory "db.dat" d 3
./heap-and-stack-memory "db.dat" l
echo ""

./heap-and-stack-memory "db.dat" g 2
echo ""

