cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DARGOS_BUILD_NATIVE=true
make clean
make
cd ..
