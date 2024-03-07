#include <iostream>

using namespace std;

/**
 * Constexpr Functions
 * Our goal is to run a function at compile-time. In C++
 * 14 onwards, we can actually do this with constexpr/consteval functions.
 * This is the more intuitive way as we read functions taking in values
 * as parameters and outputing values. It's recommended and more readable.
 * In C++11, we can only have 1 return statement in the function. C++14
 * onwards has more flexibility of expression.
 *
 * At the end of the day, a function is just a set of instructions with
 * an address specified in the binary executable to run these instructions -
 * if all required values are known at compile time there is no reason we
 * can't run the function at compile-time.
 *
 * What happens under the hood is that the compiler will evaluate the expression at
 * compile-time, and replace all function calls with the return value.
 *
 * Note that the above sentence means constexpr functions ar implicitly inline.
 * The full definition of each function must be known, forward declarations are NOT allowed.
 *
 * @ref: https://www.learncpp.com/cpp-tutorial/constexpr-and-consteval-functions/
 */

constexpr int factorial(int i) {
  return (i == 1) ? 1 : factorial(i - 1) * i;
}

// Another version of the constexpr written in templates - the input is the type, but the
// output is a value. Without constexpr, we get error: static_assert expression is not an integral constant expression
// the function won't be run at compile time and doesn't evaluate to an integral constant expression
template<int i>
constexpr int factorial() {
  return factorial<i - 1>() * i;
}

template<>
constexpr int factorial<1>() {
  return 1;
}

/**
 * Why not just use constexpr functions for everything?
 * It is NOT always required to run at compile-time wherever possible.
 * It's only guaranteed if the return value is used for a constexpr variable.
 * e.g for a constexpr function foo(), constexpr int x = foo() is guaranteed
 * to run foo() at compile-time. But cout << foo() << cout is not.
 *
 * It's better to think of constexpr as
 * "A function whose return value can be used as a constexpr"
 * rather than "a function that runs at compile time".
 *
 * For the above reason, the ONLY way we can guarantee that a function is run at
 * compile-time is to save its return value in a constexpr variable. But it's kind of
 * hacky and ugly.
 *
 * In C++20 onwards, we can use consteval, which guarantees compile-time running.
 * But then we can't reuse the same function for runtime.
 */

int main() {
  // With the use of constexpr, both of these will compile to
  // static_assert<120 == 120, "Failed");
  static_assert(factorial<5>() == 120, "Failed");
  static_assert(factorial(5) == 120, "Failed");

  // guaranteed to run at compile-time since `product` is needed at compile time
  constexpr int product = factorial(4) * factorial(5);
  static_assert(product == 2880);

  return 0;
}
