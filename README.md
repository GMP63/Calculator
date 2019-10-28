# Calculator
Elemental infix aritmethic expresion parser and executor.

How to build calc
Download this repo. In the root directory you will see a Makefile. Open the console and then type the following:
$ make

This performs two builds, the application (./bin/calc) calc and the unit test ./test/bin/test). After the two successful builds, the console should show the total of the test passed ok.

How to run the command line calculator
To run it from the console, you can supply many character expressions as you please. This can be infered reading the self explanatory elemental help, invoking the application without command line arguments:
$ bin/calc
Usage: calc [-v[1-2]]  <expression 1> <expression 2> ... <expression n>
Example: calc -v2  1+1  5-6/2+3*4  3+4*(2+1*1*(4-(1+1)))-(9+7)  1*2*3*(2-1/3)  "(4 + 5 * (7 - 3)) - 2"

How to check the application
You can run the following and check the printed values:
$ bin/calc 1+1  5-6/2+3*4  '3+4*(2+1*1*(4-(1+1)))-(9+7)'  1*2*3*(2-1/3)  '(4 + 5 * (7 - 3)) - 2'

Expression #1 : Result = 2

Expression #2 : Result = 14

Expression #3 : Result = 3

Expression #4 : Result = 10

Expression #5 : Result = 22

How to check re-run the unit tests:
$ ./test/bin/test

Total tests: 36
Test passed: 36
Test failed: 0

ALL TEST PASSED !

