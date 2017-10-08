-----------------------------------------------------------------------

## DESCRIPTION OF PROJECT:

Implemented a template using macro for a double-ended queue without using C++ features. As a
template it can contain any type without using void* (which violates type safety). Deque was
implemented as a circular dynamic array. The functionalities to be implemented for deque had to
be inferred from a test code. Some of the functionalities were: iterator, sorting, equality of
deques. Other exhaustive stress tests were performed to check performance of deque in terms of
time and memory allocations.
-----------------------------------------------------------------------

## INPUT:
Include Deque.hpp header file in your own test code to use the functionality of Container template. Or run 
any of the test files to do a comprehensive analysis of the Container implemented.
-----------------------------------------------------------------------

## OUTPUT:
Correct output files for each test code is in folder. Other test have assert statements to check correctness of program.
-----------------------------------------------------------------------
## FILES INCLUDED:
	1.Deque.hpp:
	Long macro has implementation of generic deque that accepts any data type (struct,predefined tyeps etc.). To be able to use 
	the this deque have to include Deque.hpp in header file and declare Declare Deque(MyClassType) to replicate the deque code for Deque which accepts
	type MyClassType.

	2.basic_fns_test_1.cpp
	2.1 Basic Testing

	3.all_fns.cpp
	3.1 ouput should match with all_fns_output

	4.compatibility_check_1.cpp:
	4.1 If all required methods are function pointers or not.
	4.2 STL is not used.
	4.3 Shoule match compatibility_check_output.
	
	5.compatibility_check_2.cpp:
	5.1 To check if deque is circular or not.

	6.compatibility_check_3.cpp:
	6.1 To check if macro guards are present or not
	6.2 Deque.hpp should be self sufficient in terms of header files.
	
	7.sort.c
	7.1 Sorting check output should match with sort_ouput.txt

	8.performance.c
	8.1 Performance test

	9.exhaustive_test.c
	9.1 Checking if complete API works seamlessly.

	10.sort_output.txt
	11.compatibility_check_output.txt
-----------------------------------------------------------------------

## COMPILING THE CODE:
include Deque.hpp in any of the test then use command:
g++ -std=c++11 -test.cpp -ldl -o demo
-----------------------------------------------------------------------

## RUNNING THE CODE:
launch the executable demo.
-----------------------------------------------------------------------