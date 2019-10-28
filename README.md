# Calculator
Elemental infix aritmethic expresion parser and executor.

## Overview
The purpose of this simple excercise is to provide a simple command line calculator (aritmethic infix operator syntax).
The operands are limited to just one digit integers (0 to 9) in this version. The operators are limited to arithmetic one: + - * / ^ ( ) 

## Build and run
### How to build calc
Download this repo. In its root directory you will see a Makefile. Positioned inside this directory (folder), open the console and then type the following:
$ make

This performs two builds, the application (./bin/calc) calc and the unit test ./test/bin/test). After the two successful builds, the console should show the total of the test passed ok.

### How to run the command line calculator
To run it from the console, you can supply many character expressions as you please. This can be infered reading the self explanatory elemental help, invoking the application without command line arguments:
```
$ bin/calc
Usage: calc [-v[1-2]]  <expression 1> <expression 2> ... <expression n>
Example: calc -v2  1+1  5-6/2+3*4  3+4*(2+1*1*(4-(1+1)))-(9+7)  1*2*3*(2-1/3)  "(4 + 5 * (7 - 3)) - 2"
```
### How to check the application
You can run the following and check the printed values:
```
$ bin/calc 1+1  5-6/2+3*4  '3+4*(2+1*1*(4-(1+1)))-(9+7)'  1*2*3*(2-1/3)  '(4 + 5 * (7 - 3)) - 2'

Expression #1 : Result = 2

Expression #2 : Result = 14

Expression #3 : Result = 3

Expression #4 : Result = 10

Expression #5 : Result = 22
```
### How to re-run the unit tests:
```
$ ./test/bin/test

Total tests: 36
Test passed: 36
Test failed: 0


ALL TEST PASSED !
```
## Further builds
### Rebuild all in release
Normal build generates by default executables having debug information. When you perform a regular build typing **make**, a debug buils is performed. You can confirm this reading the compilation of each .cpp file and noticing "g++ -g3 -O0 ...".
When you need to do a final build without debug info (release mode), you have to type:
```
$ make clean
------------------------------------------------------------------------
Cleaning whole project Abstract Syntaxt Tree ...
rm -f   obj/OperationItem.o  obj/ExpressionParser.o  obj/ArithmeticEvaluator.o obj/calc.o bin/calc  test/obj/test-macros.o test/obj/test.o test/bin/test
Done.
------------------------------------------------------------------------
Cleaning whole project Abstract Syntaxt Tree ...
rm -f   obj/OperationItem.o  obj/ExpressionParser.o  obj/ArithmeticEvaluator.o obj/calc.o bin/calc  test/obj/test-macros.o test/obj/test.o test/bin/test
Done.
$ make release all
make: No se hace nada para 'release'.
mkdir -p bin
mkdir -p obj
------------------------------------------------------------------------
Building file: src/calc.cpp
g++ -g0 -O3 -std=gnu++11 -Iinclude -c -o obj/calc.o src/calc.cpp
Finished building: src/calc.cpp
------------------------------------------------------------------------
Building file: src/OperationItem.cpp
g++ -g0 -O3 -std=gnu++11 -Iinclude -c -o obj/OperationItem.o src/OperationItem.cpp
Finished building: src/OperationItem.cpp
------------------------------------------------------------------------
Building file: src/ExpressionParser.cpp
g++ -g0 -O3 -std=gnu++11 -Iinclude -c -o obj/ExpressionParser.o src/ExpressionParser.cpp
Finished building: src/ExpressionParser.cpp
------------------------------------------------------------------------
Building file: src/ArithmeticEvaluator.cpp
g++ -g0 -O3 -std=gnu++11 -Iinclude -c -o obj/ArithmeticEvaluator.o src/ArithmeticEvaluator.cpp
Finished building: src/ArithmeticEvaluator.cpp
------------------------------------------------------------------------
Linking file: calc
gcc -o bin/calc obj/calc.o  obj/OperationItem.o  obj/ExpressionParser.o  obj/ArithmeticEvaluator.o -lstdc++ -lm
Finished linking: calc
BUILD SUCCEEDED

mkdir -p test/bin
mkdir -p test/obj
------------------------------------------------------------------------
Building file: test/src/test.cpp
g++ -g0 -O3 -std=gnu++11 -Iinclude -c -o test/obj/test.o test/src/test.cpp
Finished building: test/src/test.cpp
------------------------------------------------------------------------
Building file: test/src/test-macros.cpp
g++ -g0 -O3 -std=gnu++11 -Iinclude -c -o test/obj/test-macros.o test/src/test-macros.cpp
Finished building: test/src/test-macros.cpp
------------------------------------------------------------------------
Linking file: test
gcc -o test/bin/test test/obj/test.o  test/obj/test-macros.o  obj/OperationItem.o  obj/ExpressionParser.o  obj/ArithmeticEvaluator.o -lstdc++ -lm
Finished linking: test
BUILD SUCCEEDED

Running tests...

test/bin/test

Total tests: 34
Test passed: 34
Test failed: 0

ALL TEST PASSED !

```
You notice now that every file is built in release mode ("g++ -g0 -O3 ..."). Also the executable files ./bin/calc and ./test/bin/test are much smaller in size.
