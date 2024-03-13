#include <iostream>

using namespace std;

/**
  * Constructors and Destructors
  *
  * Every class or struct must have a constructor and destructor. If
  * one isn't available, the compiler will generate a default one.
  * Only a ctor/dtor can have the name of the class, and only a ctor
  * may have member initializer lists.
  *
  * They have no return types, and the ctor/dtor does not have a function ptr.
  */

class A {
public:
  A() { cout << "A ctor" << endl; }
  ~A() { cout << "A dtor" << endl; }
};

class B {
public:
  B() { cout << "B ctor" << endl; }
  ~B() { cout << "B dtor" << endl; }
};

class C {
public:
  C() { cout << "C ctor" << endl; }
  ~C() { cout << "C dtor" << endl; }
};

class Base {
public:
  Base() { cout << "Base ctor" << endl; }
  ~Base() { cout << "Base dtor" << endl; }

  A m_A;
};

class Derived : public Base {
public:
  Derived() { cout << "Derived ctor" << endl; }
  ~Derived() { cout << "Derived dtor" << endl; }

  void foo() { cout << "Derived foo" << endl; }

  B m_B;
  C m_C;
};


int main() {
  cout << "START" << endl;
  Derived obj;

  // this works
  const auto pFoo = &Derived::foo;
  // but this doesn't!
  //const auto pCtor = &Derived::Derived;
  // a ctor is not a function with an address that executes per se.
  // It is how the compiler initializes the object.

  // What happens when we initialize primitive types? E.g int i = 3;
  // The compiler sees the type 'int', and allocates memory on the stack
  // with sizeof(int). Then, it gives that memory to the "ctor" of int.
  // i is initialized to the value of '3' without any additional assignment.

  // For object ctors, a similar logic follows - e.g A a;
  // We allocate memory for sizeof(A) on the stack, and that memory
  // is given to the ctor of A, in this case, the default ctor since no args
  // are provided. Then, the ctor of A "runs" and we get an object A on the
  // memory for variable a.
  //
  cout << "END" << endl;
}
