#include <iostream>
#include <string>

using namespace std;

/**
 * STL provides <type_traits>, which lets you check what type things
 * are at compile time. This can be handy in compile-time polymorphism type
 * situations such as when we have a generic function and we want to
 * do something slightly different for each type. Let's implement some of them
 * on our own.
 *
 * @ref: https://www.youtube.com/watch?v=VBI6TSo8Zog&list=PLWxziGKTUvQFIsbbFcTZz7jOT4TMGnZBh&index=1
 */

// since the input is a template value, we can't use dynamic casting to check
// which is the usual OOP way for type-checking. How can we check the type then?
// What else is generated at compile time? Template specializations!

// the default value for is_pointer is false
template<typename T>
struct my_is_pointer {
  static constexpr bool value = false; // value is not tied to class instance
};
// specialize the template for T*
template<typename T>
struct my_is_pointer<T*> {
  static constexpr bool value = true;
};

template<typename T>
struct my_is_int {
  static constexpr bool value = false; // value is not tied to class instance
};
template<>
struct my_is_int<int> {
  static constexpr bool value = true;
};

template<typename T>
struct my_is_floating_point {
  static constexpr bool value = false; // value is not tied to class instance
};
template<>
struct my_is_floating_point<float> {
  static constexpr bool value = true;
};
template<>
struct my_is_floating_point<double> {
  static constexpr bool value = true;
};

template<typename T>
struct my_is_string {
  static constexpr bool value = false; // value is not tied to class instance
};
template<>
struct my_is_string<string> {
  static constexpr bool value = true;
};

template<typename T>
void printTypeNaive(const T& val) {
  if constexpr(my_is_pointer<T>::value) {
    cout << "Pointer to ";
  }
  cout << "Type ";
  if constexpr(my_is_int<T>::value) {
    cout << "Int ";
  } else if constexpr(my_is_floating_point<T>::value) {
    cout << "Float/Double ";
  } else if constexpr(my_is_string<T>::value) {
    cout << "String ";
  } else {
    cout << "Unknown ";
  }
  cout << "with Val: " << val;
  cout << endl;
}

// define a way to get the type without the ptr
// since our "output" is a type, we use aliases instead of values
// before we specialize, we need to declare the default template
template<typename T>
struct strip_ptr{
  using type = T;
};
template<typename T>
struct strip_ptr<T*>{
  using type = T;
};
template<typename T>
struct strip_ptr<T**>{
  using type = T;
};

template<typename T>
void printTypeWithPtrType(T val) {
  if constexpr(!my_is_pointer<T>::value) return; // not a ptr
  cout << "Pointer to Type ";

  using without_ptr_type = typename strip_ptr<T>::type;
  if constexpr(my_is_int<without_ptr_type>::value) {
    cout << "Int ";
  } else if constexpr(my_is_floating_point<without_ptr_type>::value) {
    cout << "Float/Double ";
  } else if constexpr(my_is_string<without_ptr_type>::value) {
    cout << "String ";
  } else {
    cout << "Unknown ";
  }
  cout << "with Val: " << *val;
  cout << endl;
}

int main() {
  int a = 10;
  double b = 12.345;
  float c = 1.0;
  string d = "hello!";

  int* pA = &a;
  double* pB = &b;
  float* pC = &c;
  string* pD = &d;

  int** ppA = &pA;
  double** ppB = &pB;
  float** ppC = &pC;
  string** ppD = &pD;

  cout << "==== Naive: ====" << endl;
  printTypeNaive(a);
  printTypeNaive(b);
  printTypeNaive(c);
  printTypeNaive(d);

  // notice that this will correctly check is_ptr,
  // but will NOT know the type of the ptr, since
  // int* is NOT int!
  printTypeNaive(pA);
  printTypeNaive(pB);
  printTypeNaive(pC);
  printTypeNaive(pD);

  cout << "=== With Ptr Type: ===" << endl;

  printTypeWithPtrType(pA);
  printTypeWithPtrType(pB);
  printTypeWithPtrType(pC);
  printTypeWithPtrType(pD);

  printTypeWithPtrType(ppA);
  printTypeWithPtrType(ppB);
  printTypeWithPtrType(ppC);
  printTypeWithPtrType(ppD);
  return 0;
}
