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

/**
  * Template Specialization - CLASSES
  *
  * For classes, we normally think of generic templates that will implicitly instantiate
  * its members and functions, for example std::vector<T>, which will allocate objects of type
  * T for each. But sometimes we want the class to do something different, e.g vector<bool> which could
  * be optimized as a bitset.
  *
  * This is NOT recommended but we will demonstrate that it is indeed the case.
  * A workaround will be to user std::vector<char> or std::deque<bool>, which doesn't have a specialization.
  * Or, for fixed-size arrays of bits, use std::bitset.
  *
  * ref: https://www.learncpp.com/cpp-tutorial/class-template-specialization/
  * ref: https://cplusplus.com/reference/vector/vector-bool/#google_vignette
  * ref: http://en.cppreference.com/w/cpp/container/vector_bool
  */

template <typename T>
void printFirst(vector<T>& v) {
  T& first = v[0];
  cout << first << endl;
}

template<>
void printFirst(vector<bool>& v) {
  vector<bool>::reference first = v[0];
  cout << first << endl;
}

template <typename T>
void printFirstByVal(vector<T>& v) {
  T first = v[0];
  cout << first << endl;
}

void classSpecializations() {
  vector<char> charVec(10000, 'a');
  vector<bool> boolVec(10000, true);
  // doesn't work without template specialization for vector<bool>
  // error: non-const lvalue reference to type 'bool' cannot bind to a temporary of type 'std::vector<bool>::reference' (aka '__bit_reference<std::vector<bool>>')
  printFirst(charVec);
  printFirst(boolVec);
  // works because assignment of vector<bool>::reference into bool is allowed
  printFirstByVal(charVec);
  printFirstByVal(boolVec);

  // error: no member named 'data' in 'std::vector<bool>'
  // data is not necessarily stored in a contiguous chunk of memory, so we can't access the raw ptr and use ptr arithmetic
  // boolVec.data();
}

int main() {
  functionSpecializations();
  classSpecializations();
  return 0;
}
