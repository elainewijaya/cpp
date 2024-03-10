#include <vector>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

/**
  * Writing metafunctions
  *
  * Before attempting to write a metafunction, let's try to write a
  * runtime version. The normal way we would do this is with a for loop
  * or std::find, which are imperative methods, however these don't translate
  * well to TMP. This is because variables in template metaprograms are immutable,
  * since they are instantiated only once. TMP is a form of functional programming,
  * where the majority of control-flow is implemented with recursion.
  */

bool runtimeContains(const string& p_TypeToFind, const vector<string>& p_Types,
    size_t p_StartFrom = 0) {
  if (p_StartFrom >= p_Types.size() - 1) return false; // reached the end
  else if (p_Types[p_StartFrom] == p_TypeToFind) return true; // found it
  else return runtimeContains(p_TypeToFind, p_Types, p_StartFrom + 1);
}

/**
  * Now let's write the TMP version.
  * 1. Input args: The input args will be the same except they are in the types <> instead of params ().
  * We can keep the input types generic with typename T, or give it concrete values.
  *
  * 2. If - Then control flow: We don't have if-else in TMP so we have to make our own simple function.
  * 3. ==: We need a compile-time is equal check. We can use std::is_same or implement our own simple struct.
  * 4. Return True/False: We need a way to convert between the TYPE true/false and the VALUE true/false.
  * 5. Using template logic to determine true/false type: We can't specialize the template for each and every
  *    case that will return true/false! So we instead DERIVE from a templated base class. The templated base
  *    class will hold a type from the result of IfThenElse. This is how we get our final true/false value
  */

// declaration
template<bool Condition, typename Then, typename Else> // Note: Condition is a VALUE but the rest are typenames
struct IfThenElse {};

// specialization for Then and Else cases
template<typename Then, typename Else>
struct IfThenElse<true, Then, Else> {
  using type = Then;
};
template<typename Then, typename Else>
struct IfThenElse<false, Then, Else> {
  using type = Else;
};

template<typename T1, typename T2>
struct my_is_same {
  static constexpr bool value = false;
};
template<typename T1>
struct my_is_same<T1, T1> {
  static constexpr bool value = true;
};

// Type holding values for true and false
struct my_false_type {
  static constexpr bool value = false;
};
struct my_true_type {
  static constexpr bool value = true;
};

template<typename TypeToFind, typename TupleType, size_t StartFrom=0>
struct tupleContains : // Derive from the base class IfThenElse<...>
  IfThenElse<
    /*IF*/ my_is_same<tuple_element_t<StartFrom, TupleType>, TypeToFind>::value,
    /*THEN*/ my_true_type,
    /*ELSE*/ typename IfThenElse<
      /*IF*/ (tuple_size<TupleType>::value-1 == StartFrom),
      /*THEN*/ my_false_type,
      /*ELSE*/ tupleContains<TypeToFind, TupleType, StartFrom + 1>
    >::type // the result of IfThenElse is stored in ::type
  >::type
{};
// At the induction step, we are instantiating the templates for StartFrom 0...n

int main() {
  // for the runtime version, our input will be values, and the output is a value
  vector<string> types = {"double", "float", "string"};
  cout << "Types contains double :" << runtimeContains("double", types, 0) <<
    ", contains int: " << runtimeContains("int", types, 0) << endl;

  // IsSame
  cout << "My IsSame: " << my_is_same<int, int>::value << ", " << my_is_same<int, float>::value << endl;

  // IfThenElse
  cout << "IfThenElse: " << my_is_same<IfThenElse<(5 > 3), int, bool>::type, int>::value << endl;
  cout << "IfThenElse: " << my_is_same<IfThenElse<(5 < 3), int, bool>::type, int>::value << endl;
  using result = IfThenElse<(5 < 3), int, IfThenElse<
                     (5 > 2), double, float>::type>::type;
  cout << "Nested IfThenElse: " << my_is_same<result, double>::value << endl;

  tuple<double, float, string> tuple;
  // The templated function will take in a type and the type of the tuple as
  // an input and a value as an input. Since it's a metafunction, inputs will be in <>
  // Since we are passing in the type of tuple, we can use decltype(tuple) instead of
  // typing tuple<double, float, string> each time - decltype transforms an expression
  // to a type. Since our output is a value, we store it in the template class declaration.
  cout << "Tuple contains double: " << tupleContains<double, decltype(tuple)>::value <<
    ", contains int: " << tupleContains<int, decltype(tuple)>::value << endl;

  return 0;
}
