#include <iostream>

using namespace std;
//#define INSIGHTS

/**
 *  Before getting into more involved use-cases, let's explore a bit
 *  how parameter packs work.
 *
 * @ref: https://www.youtube.com/watch?v=irFkMavpL9A
 * @ref: https://en.cppreference.com/w/cpp/language/parameter_pack
 */

// Parameter packs work with TEMPLATE arguments, just using ... is creating
// a c-style variadic function with VA_ARGS, which does runtime evaluation for the
// number and type of args!
void notParameterPack(int ...) {} // this is NOT a parameter pack
// Won't compile!!!! error: type 'int' of function parameter pack does not contain any unexpanded parameter packs
// Compiler sees this as "i need to unpack int into the variable ints" but it can't unpack int because it is not
// a template parameter pack
// void foo(int ...ints);

// All of these compile as template parameter pack declarations
template<int...>
void foo1();
template<int...Ints> // name our parameter pack of type int as 'Ints'
void foo2();
template<typename...>
void foo3();
template<typename...Types> // name our parameter pack of typenames as 'Types'
void foo4();

// Let's define some class templates that take in either a variable number of types, or expressions.
// The parameter pack has to match the type/expression!
template<typename...Types>
struct MyTuple {};
MyTuple<> t0;
MyTuple<int, float> t1;
// error: template argument for template type parameter must be a type
// MyTuple<25> t2;
template<int...Ints>
struct MyIntSequence {};
MyIntSequence<0,2,3,4> s0;
// error: template argument for non-type template parameter must be an expression
// MyIntSequence<int, float> s1;

// Now for some functions: Given a "list" of types, how do we expand them?
// The syntax to expand a parameter pack is <pattern> ...
// Where the pattern must include at least one parameter pack
template<typename... Types> // name parameter pack as Types
void foo5(Types... args) {} // Types is a parameter pack, expand it to args

// When we instantiate the template, the parameter pack will expand like so
// Instantiated with foo5<5, 3.2>:
#ifdef INSIGHTS
template<> // specialization
// Types... has been expanded to (int arg0, double arg1) at compile-time, so the
// function signature now has inputs of many types
foo5<int, double>(int __args0, double args1)
#endif

// OK, but now that we want to use args, we get an error:
// cout << args << endl; <----- error: expression contains unexpanded parameter pack 'args'
// we cannot use PPs in expressions without expanding them!

// Let's make a simple Sum function
// base case
template<typename T>
T Sum(T arg) {
  return arg;
}
// induction step
template<typename T, typename... Types>
int Sum(T start, Types... args) {
    return start + Sum(args...);
}

// Calling with Sum(5, 2.5, 2.0f), this expands to:
#ifdef INSIGHTS
template<int, double, float> --> first instantiation
start is int, double and float are expanded from PP
template<double, float> --> second instantiation
start is double, float is expanded from PP
template<float> --> last instantiation
base case overload is used, return the float
#endif

// Given many args, our instantiation will expand the PP to
/*
int Sum<int, double, float, double>(int start, double __args1, float __args2, double __args3) <-- expand the PP of typenames
{
  return start + Sum(__args1, __args2, __args3); <-- expand the PP into expression
}
and so on
*/

/**
 *  Variadic Templates and Parameter Packs
 *
 *  Let's try to write a generic function that can print
 *  a variable number of values of different types, again WITHOUT
 *  for-loops.
 *
 *  @ref: https://www.youtube.com/watch?v=GusZ4P_iTks&list=PLWxziGKTUvQFIsbbFcTZz7jOT4TMGnZBh&index=3
 *  @ref: https://en.cppreference.com/w/cpp/utility/integer_sequence
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

/**
  * Let's try to reuse the above function to print the contents of a tuple.
  * We can access the elements of a tuple using std::get<INDEX>(tuple), and get the
  * number of elements in a tuple using std::tuple_size<tuple>::value.
  * The high level idea is to use the above printN function, but providing the tuple
  * CONTENTS in the parameter pack T1toN. For that, we will need to use std::integer_sequence.
  */

// To start with, let's try to print an integer sequence
// The values are stored in the parameter pack as typenames, so we need to expand the parameter pack
template<typename T, T...Ints>
void printIntSequence(integer_sequence<T, Ints...> p_IntSeq) {
  cout << "Sequence has size " << p_IntSeq.size() << endl;
  // This will expand to (((cout << int1) << int2 ) << int3)...
  // (cout << ... << ints);
  // To use separators, we do the fold with a different expression rather than a single operator cout<<
  // The inner bracket expands to the expression, "do cout << ", " << on each element
  // the comma means, fold it for each expanded parameter in Ints
  // so we get (((cout << ", " << Int1) << ", " << Int2) << ", " Int3 << ....)
  ((cout << ", " << Ints), ...);
  cout << endl;
}

template<typename TupleType, size_t...Indices> // we use the parameter pack size_t Indices
void printTuple_impl(TupleType p_Tuple, index_sequence<Indices...>) { // second arg expands to index_sequence<0,1,2....N-1>
  // This is WRONG, the ... will expand in the type which becomes
  // get<0,1,2,3,4,....,N-1>(p_Tuple) - but get only accepts 1 index
  // printN(get<Indices...>(p_Tuple));

  // we need to expand the parameter pack AFTER the function call
  printN(get<Indices>(p_Tuple)...);
}

template<typename TupleType>
void printTuple(TupleType p_Tuple) {
  // need to expand the parameter pack, make another overload
  // impl will take in the tuple, and the index list to print
  printTuple_impl(p_Tuple, make_index_sequence<tuple_size<TupleType>::value>{});
}

int main() {
  printN("hello", 1, 2, 4.332, "world", true);
  printN("one elem");
  printN();

  // template <typename T, T...ints> - takes in the first param as the type of the subsequent elements,
  // and the elements themselves as a parameter pack
  // only has 1 member function, size. Values are stored in types
  printIntSequence(integer_sequence<int, 2, 4, 8, 9, 10, 22>{});
  // make_index_sequence is a convenience function for creating integer_sequence<0, 1, ..., N-1> given some size_t N
  printIntSequence(make_index_sequence<10>{});

  auto tuple = make_tuple("hello", 1, 2, 4.332, "world", true);
  printTuple(tuple);
}
