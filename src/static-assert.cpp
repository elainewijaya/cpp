#include <iostream>

using namespace std;

/**
 * Static Assert
 *
 * To check if a function is evaluated at compile time, we need
 * to be able to also validate at compile-time. C++ provides a way
 * to do this: the following line will not compile
 *
 * static_assert(5 == 2, "failed");
 * src/compile-time-functions.cpp:8:1: error: static_assert failed due to
 * requirement '5 == 2' "failed"
 *
 * ref: https://stackoverflow.com/questions/40274395/c-implement-my-own-static-assert
 * ref: http://warp.povusers.org/programming/template_declarations.html
 */

template<bool> // generic template declaration
// This means, for each value of bool, I can have a different declaration of the struct my_static_assert.
struct my_static_assert;
// at this point, no concrete code is generated.

// trying to construct an object of my_static_assert at this point in main() will fail for both cases below!
// the compiler doesn't know how to create my_static_assert. how big is it? what is its memory layout?
//my_static_assert<true>{};
//my_static_assert<false>{};

// now we specialize our template for the value true only. Remember the {} as it means definition
// (of an empty struct with no data members) VS just declaration (which will give a linker error with no definition)
template<>
struct my_static_assert<true>
{
};
// now this will compile for T == true, but not for T == false

int main() {
  my_static_assert<1==1>{};
  //my_static_assert<1==2>{}; // ERROR: // implicit instantiation of undefined template 'my_static_assert<false>'
  return 0;
}
