/*
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "util.h"
#include "cpp.util.h"

void OBJ05(void);
void OBJ06(void);
void OBJ07(void);
void OBJ31(void);
void OBJ33(void);
void OBJ34(void);

void OBJ() {
  OBJ05();
  OBJ06();
  OBJ07();
  OBJ31();
  OBJ33();
  OBJ34();
}

/* OBJ00 */
class OBJ00_Widget {
public:
  int total;
  OBJ00_Widget (): total(0) {}
  void add () {
    total++;
  }
  void remove () {
    total--;
  }
};

/* OBJ03 */
/*XXX
class OBJ03_Thing {
 public:
  virtual ~OBJ03_Thing();
  virtual void update( int );
  virtual void update( double );
};

class MyOBJ03_Thing : public OBJ03_Thing {
public:
  void update( int ); // overrides only update(int), hides both!
};

void OBJ03(void) {
  MyOBJ03_Thing *mt = new MyOBJ03_Thing;
  OBJ03_Thing *t = mt;
  t->update( 12.3 ); // OK, base
  mt->update( 12.3 );  // oops! derived!
}
*/

/* OBJ04 */
class OBJ04_Thing {
public:
  virtual ~OBJ04_Thing();
  virtual void doItNTimes( int numTimes = 10 );
  virtual void doItThisHigh( double howHigh = 1.0 );
  // ...
};
class OBJ04_MyThing : public OBJ04_Thing {
public:
  void doItNTimes( int numTimes );
  void doItThisHigh( double howHigh = 2.2 );
  // ...
};

/* OBJ05 */
class OBJ05_SomeClass {
public:
  void doSomething();
  void destroy();
};

void OBJ05_SomeClass::destroy() {
  delete this;  // Dangerous!!
}

void OBJ05(void) {
  OBJ05_SomeClass sc;
  sc.destroy();
}

/* OBJ06 */
class OBJ06_thing {
public:
  int * ptr;
  int a;
};

void OBJ06(void) {
  OBJ06_thing *object = new OBJ06_thing();
  OBJ06_thing *objectcpy = new OBJ06_thing((const OBJ06_thing &)object);
}

/* OBJ07 */
class OBJ07_Base1 {
public:
  const char* f(int) {return "OBJ07_Base1::f()";}
};

class OBJ07_Base2 {
private:
  const char* f() {return "OBJ07_Base2::f()";}
};

class OBJ07_Derived : public OBJ07_Base1, public OBJ07_Base2 {
public:
  const char* g() {return "Derived::g()";}
};

void OBJ07(void) {
  OBJ07_Derived d;
  std::cout << d.g() << std::endl;
}

/* OBJ30 */
class OBJ30_B {
public:
  OBJ30_B() { seize(); }
  virtual ~OBJ30_B() { release(); }
protected:
  virtual void seize() {}
  virtual void release() {}
};

class OBJ30_D : public OBJ30_B {
public:
  OBJ30_D() {}
  ~OBJ30_D() {}
protected:
  void seize() {
    OBJ30_B::seize();
    // get derived resources...
  }
  void release() {
    // release derived resources...
    OBJ30_B::release();
  }
};

/* OBJ31 */
class OBJ31_Widget {
public:
  explicit OBJ31_Widget() {cerr << "constructed" << endl;}
};

void OBJ31(void) {
  OBJ31_Widget w();
}

/* OBJ32 */
class OBJ32_Widget {
public:
  OBJ32_Widget(int size);
};

/* OBJ33 */
class OBJ33_Employee {
public:
  OBJ33_Employee(string theName) : name(theName) {};
  string getName() const {return name;}
  virtual void print() const {
    cout << "OBJ33_Employee: " << getName() << endl;
  }
private:
  string name;
};

class OBJ33_Manager : public OBJ33_Employee {
public:
  OBJ33_Manager(string theName, OBJ33_Employee theEmployee) :
    OBJ33_Employee(theName), assistant(theEmployee) {};
  OBJ33_Employee getAssistant() const {return assistant;}
  virtual void print() const {
    cout << "Manager: " << getName() << endl;
    cout << "Assistant: " << assistant.getName() << endl;
  }
private:
  OBJ33_Employee assistant;
};

void OBJ33(void) {
  OBJ33_Employee coder("Joe Smith");
  OBJ33_Employee typist("Bill Jones");
  OBJ33_Manager designer("Jane Doe", typist);

  coder = designer;  // slices Jane Doe!
  coder.print();
}

/* OBJ34 */
class OBJ34_Base {
public:
  OBJ34_Base() {
    // Build Base object
  }
  ~OBJ34_Base() {
    // Destroy Base object
  }
};

class OBJ34_Derived : public OBJ34_Base {
public:
  OBJ34_Derived() {
    // Build Derived object
  }
  ~OBJ34_Derived() {
    // Destroy Derived object
  }
};

void OBJ34(void) {
  OBJ34_Base *b = new OBJ34_Derived();
  delete b;
}

/* OBJ35 */
class OBJ35_Widget {
public:
  OBJ35_Widget (): total(0) {}

  void add () {
    total++;
  }
  void remove () {
    total--;
  }

  int& getTotal () {return total;}

private:
  int total;
};

/* OBJ36 */
class OBJ36_A {
public:
  OBJ36_A() : value(0) {}
  OBJ36_A(const OBJ36_A& rhs) : value(rhs.value) {
    ++invocations;
  }
private:
  int value;
  static int invocations;
};

int OBJ36_A::invocations = 0;

OBJ36_A OBJ36_f() {
  return OBJ36_A();
}

void OBJ36(void) {
  OBJ36_A a = OBJ36_f();
}
