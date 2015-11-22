cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DARGOS_BUILD_NATIVE=true
make clean
make
cd ..
