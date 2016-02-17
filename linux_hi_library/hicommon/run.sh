filepath="$(cd `dirname $0`;pwd)"
echo $filepath
echo -----------compile lib
echo current folder:$filepath
cd $filepath
make

echo -----------compile test
cd $filepath/test
make
export LD_LIBRARY_PATH=~/lib:$LD_LIBRARY_PATH 
./test.run