#include <iostream>

using namespace std;

/**
  * Template Specialization - FUNCTIONS
  *
  * Consider how templated functions generally work.
  * We have a function that takes in some inputs, and when it is called,
  * if it the instructions are defined for that type, that template
  * is instantiated. But what if we want different behaviour for specific types?
  * This is why we have template specialization.
  *
  * ref: https://www.learncpp.com/cpp-tutorial/function-template-specialization/
  * ref: https://www.learncpp.com/cpp-tutorial/class-template-specialization/
  */

template<typename T> // generic
void print(const T& val) { cout << "Val: " << val << endl; }

template<> // specialization!
void print<double>(const double& val) { cout << "DoubleVal:" << val << endl; }

struct A{}; // implicit specialization won't work here!

void functionSpecializations() {
  print(5); // here, print template for 'int' is implicitly instantiated
  print(5.21);  // here, print template for 'double' was already explicitly instantiated, so we use that
  print(13789329244242); // here, print template for 'long' is implicitly instantiated
  // this fails to compile because operator<< is not defined for class A
  // so we cannot implicitly instantiate the template func
  //print(A());
}




int main() {
  functionSpecializations();
  return 0;
}
