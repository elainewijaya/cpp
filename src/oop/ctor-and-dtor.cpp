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

/**
  * Constructors and virtual
  *
  * Why can't a ctor be virtual? error: constructor cannot be declared 'virtual'
  * A virtual call is a way to do something without knowing the complete type
  * of an object. But to create an object, you need to know exactly what type
  * you want to create. So a constructor cannot be virtual.
  * @ref: https://www.stroustrup.com/bs_faq2.html#virtual-ctor
  *
  * What about calling virtual functions from a constructor?
  * In a ctor, virtual calls are disabled because the override mechanisms don't exist yet!
  * If a derived class is constructed, it will first call the ctor of the base class.
  * In the base class ctor, when it sees a virtual function call, it there are no derived
  * implementations yet - only the existing base class one - so it will call the base class one.
  *
  * @ref: https://www.stroustrup.com/bs_faq2.html#vcall
  *
  * More details for above: Recall how virtual functions work.
  *
  * VTABLE: At compile time, we create the vtable per class (not instance). This table contains
  * a function pointer to the most-derived function accessible by that class.
  *
  * VPTR: At compile time, the compiler ALSO adds a hidden pointer as a member of a base class.
  * When an object is created (when the ctor is called), the vptr is set to the
  * vtable of that class. VPTR will be inherited by derived classes.
  *
  * When we call a virtual function through the base class, first we see that it's a virtual function.
  * Then we use the vptr to get the vtable for its (derived) class (the derived class knows which class it is).
  * Then we get the correct function for the derived class to call through the vtable.
  *
  * SO: for our original problem, when we create a derived class, the base ctor is called and calls
  * a virtual function. At this point the vptr is still set to the vtable of the base class. So the base
  * class version get called.
  *
  * @ref: https://www.learncpp.com/cpp-tutorial/the-virtual-table/
  */


/**
  * Delegating constructors
  *
  * Delegating constructors allow us to call ctors of the same, or base class,
  * from within the ctor itself, not in the body.
  */

struct E {
public:
  int num;
  E() { num = 10; cout << "E default ctor" << endl; }
  E(int x) { num = x; cout << "E(int x) ctor"<< endl; }
  E(int x, int y) : E(x*y) { cout << "E delegating ctor" << endl; } // delegates to E(int x)
};

struct F : public E {
  F(int x, int y, int z) : E(x*y, z) { cout << "F delegating ctor" << endl; }
  F() { cout << "F default ctor" << endl; }
  F(int x) { cout << "F non-delegating ctor" << endl; }
};

/**
  * Pure virtual functions and abstract base class
  *
  * An abstract function is marked by = 0 in the header file.
  * Abstract functions MUST be virtual.
  * Any class with at least 1 abstract function cannot be instantiated.
  * Just like a ctor cannot be virtual, it also cannot be pure virtual.
  * Even if a base interface class cannot be instantiated, a derived class (which
  * provides implementations for all its abstract methods) will instantiate it,
  * hence an abstract class needs to have a ctor defined.
  *
  * A pure virtual function may still have a definition (!)
  * This means that derived classes are required to override it, but you can still
  * call the base class's implementation directly from the derived class. We can't
  * instantiate the base class directly, but remember that it is implicitly instantiated
  * in the derived class!
  *
  * Destructors can be pure virtual. However, they must have a definition so that derived
  * classes know how to destroy them.
  * Unlike normal abstract functions, a pure dtor DOES NOT need to be implemented in the derived class.
  * The definition must be provided and is tied to the base class.
  * The only difference is that it marks the class as abstract and cannot be instantiated.
  * The reason dtors are different are that they are never "overridden" - they are always called cumulatively.
  * E.g the basic case of base class dtor being virtual will call the derived class dtor first (via vptr) and then itself.
  * It doesn't OVERRIDE the base class dtor, it's cumulative - so it's different in that sense.
  *
  * @ref: https://www.learncpp.com/cpp-tutorial/pure-virtual-functions-abstract-base-classes-and-interface-classes/
  */

class G { // abstract class
public:
  virtual void print() = 0;
  virtual ~G() = 0;
};
// defn must not be inline
void G::print() { cout << "G says hello" << endl; }
G::~G() { cout << "G is destructed" << endl; }

class H : public G {
public:
  virtual void print() override { cout << "H says hello" << endl; }
  void useGPrint() { G::print(); }
};

class Interface {
};

int main() {
  cout << "=====START=====" << endl;

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

  cout << "BASE/DERIVED CTOR/DTOR ORDER" << endl;
  Derived obj;
  // order of construction: Member of Base -> Base -> Member of Derived -> Derived
  // Class members are constructed in the order they are DECLARED in the header.
  // order of destruction: exactly opposite
  // A class needs to have its members constructed before it can be completely constructed.
  // A derived class must have its base class fully constructed before it can be
  // completely constructed.

  cout << "DELEGATING CONSTRUCTORS" << endl;
  E e(2, 5); // Will call the E(int x) ctor, then run the delegating ctor body
  F f(2, 4, 10); // Will call E(int x, int y) delegating ctor, and E(int x) ctor, then
  // run F(int x, int y, int z) ctor body
  F f2(2); // MUST call a ctor for E, so calls its default ctor, then calls F's ctor
  // Marking E() = delete will cause this error:
  // error: call to deleted constructor of 'E'
  // --> F() { cout << "F default ctor" << endl; }
  // Base/member class ctors are implicitly called, there must be an implementation for
  // derived/container class to use or else the compiler will complain

  H h;
  h.print();
  h.useGPrint();
  cout << "END" << endl;
}
