#include <iostream>

using namespace std;

// without constexpr, we get this error: static_assert expression is not an integral constant expression
// the function won't be run at compile time and doesn't evaluate to an integral constant expression
template<int i>
constexpr int factorial() {
  return factorial<i - 1>() * i;
}

template<>
constexpr int factorial<1>() {
  return 1;
}

// easier to read and recommended way as of C++ 11.
// 11 - can only have 1 return statement. 14 - can have more flexibility of expression
// however at the end of the day, constexpr is a compiler hint - it may or may not work
// at compile-time and just demote to a runtime function. If we want to guarantee it
// we can use class templates.
constexpr int factorial(int i) {
  return (i == 1) ? 1 : factorial(i - 1) * i;
}

int main() {
  static_assert(factorial<5>() == 120, "Failed");
  static_assert(factorial(5) == 120, "Failed");
  return 0;
}
