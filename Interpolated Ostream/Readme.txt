-----------------------------------------------------------------------

## DESCRIPTION OF PROJECT:
Implemented a function template named Interpolate that accepts arguments like printf.
e.g:
SomeArbitaryClass obj;
int i=1234;
double x=3.14;
std::string str("foo");
std::cout<< Interpolate(R"(i=%, x2=%\%, str1=%, obj=%)",i,x,1.001,str,"hello",obj)<<std::endl;

If there is a mismatch between number of percent signs and the number of arguments to output, 
an exception of type cs540::WrongNumberOfArgs is thrown.

-----------------------------------------------------------------------

## FILES INCLUDED:
1. Interpolate.hpp : containes implementation of array api.
2. test.cpp : contains comprehensive test provided by course intructor.
3. Include Interpolate.hpp is your test code. The function name is cs540::Interpolate.
-----------------------------------------------------------------------

## COMPILING THE CODE:
g++ -std=c++17 -test.cpp -o demo
-----------------------------------------------------------------------

## RUNNING THE CODE:
run the demo obj file generated.
-----------------------------------------------------------------------