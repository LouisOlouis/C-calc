@echo off

mkdir build
cd build

cmake ..
cmake --build .

"Debug/C-calc.exe"

