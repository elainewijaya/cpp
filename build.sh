for i in src/$1/*.cpp; do g++ $i -o apps/`basename $i .cpp` -std=c++20;done;
