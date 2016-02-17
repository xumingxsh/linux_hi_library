PWD=`pwd`

BITS=`getconf LONG_BIT`
echo -----------compile lib
cd $PWD
make

echo -----------compile test
cd $PWD/test
make
export LD_LIBRARY_PATH=~/lib
./test.run