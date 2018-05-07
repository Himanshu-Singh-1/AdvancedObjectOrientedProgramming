-----------------------------------------------------------------------

## DESCRIPTION OF PROJECT:
Implemented a non-intrusive, thread-safe, exception-safe, reference-counting smart pointer named SharedPtr. The model for this is the std::shared_ptr.
It allows different smart pointers in different threads to be safely assigned and unassigned to the same shared object using locks.
When reference-count of object becomes 0 it deletes the object via a pointer to the original type. even if the template argument of the final smart pointer is of 
a base type. It avoids object slicing for non-virtual destructors.
-----------------------------------------------------------------------

## INPUT:
Include SharedPtr in your test file to use SharedPtr API. Test code provided in this file is given by Professor Kenneth Chiu. It does a comprehensive test of all functionalities 
in multi threaded environment.
-----------------------------------------------------------------------

## OUTPUT:
Correct output corresponding to the test file is given.
-----------------------------------------------------------------------
## FILES INCLUDED:
	1. SharedPtr.hpp : Implementation of Shared Pointer api.
-----------------------------------------------------------------------

## COMPILING THE CODE:
g++ -std=c++11 -pthread -test.cpp -o demo
-----------------------------------------------------------------------

## RUNNING THE CODE:
run the demo obj file generated.
-----------------------------------------------------------------------
