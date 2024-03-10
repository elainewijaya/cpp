#include <iostream>

using namespace std;

/**
 *  Variadic Templates and Parameter Packs
 *
 *  Let's try to write a generic function that can print
 *  a variable number of values of different types, again WITHOUT
 *  for-loops.
 */

// Base cases need to be defined before inductive step
// without the base case we get:  error: no matching function for call to 'printN'
// candidate function template not viable: requires at least argument 'first', but no arguments were provided

void printN() {
}

template<typename Singleton>
void printN(Singleton last) {
  cout << last << endl;
}

template<typename T0, typename... T1toN> // ... BEFORE identifier: identifier is for a variable number of elements
void printN(T0 first, T1toN...rest) { // ... BETWEEN identifiers: second name is a variable for the list given by the first
  cout << first << ", ";
  printN(rest...); // ... AFTER identifier: expand the identifier to all elements it represents
}


int main() {
  printN("hello", 1, 2, 4.332, "world", true);
  printN("one elem");
  printN();
}
