for i in src/*.cpp; do g++ $i -o apps/`basename $i .cpp` -std=c++20;done;
