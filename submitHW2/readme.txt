This file is written c++ with c++11 support.
cd to the directory and use g++ to compile: 

g++ -std=c++11 -o test main.cpp

I put algred.i at here to generate the new code. The new code is named by ¡°output¡± followed by the original code name and method option.
1. run this code can use the value numbering method. 
./test -v algred.i 
2. run this code can use the unrolling method. 
./test -u algred.i 
3. you can also run them together:
./test -v -u algred.i

or 
./test -u -v algred.i

I use CLION for this project. I attached all the working folder here, just in case.