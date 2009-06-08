/*
 * Copyright (c) 2009 Carnegie Mellon University.
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

#include "cpp.util.h"

//void ERR00();
void ERR01();
void ERR02();
void ERR03();
void ERR04();
void ERR05();
void ERR06();
void ERR07();
void ERR08();
void ERR09();
void ERR10();
void ERR30();
void ERR31();
void ERR32();
void ERR33();
void ERR34();
void ERR35();
void ERR36();
void ERR37();
void ERR() {
  //ERR00();
  ERR01();
  ERR02();
  ERR03();
  ERR04();
  ERR05();
  ERR06();
  ERR07();
  ERR08();
  ERR09();
  ERR10();
  ERR30();
  ERR31();
  ERR32();
  ERR33();
  ERR34();
  ERR35();
  ERR36();
  ERR37();
}



/* ERR00_cpp */
void ERR00() { }


/* ERR01_cpp */
void ERR01() { 
  errno = 0;
  printf("This\n");
  printf("is\n");
  printf("a\n");
  printf("test.\n");
  if (errno != 0) {
    fprintf(stderr, "printf failed: %s\n", strerror(errno));
  }
}


/* ERR02_cpp */

//XXX context specific
//int i;
//ssize_t count = 0;
//
//for (i = 0; i < 9; ++i) {
//  count += sprintf(
//      buf + count, "%02x ", ((u8 *)&slreg_num)[i]
//      );
//}
//count += sprintf(buf + count, "\n");
//


constraint_handler_t handle_errors() {
  constraint_handler_t data;
  /* define what to do when error occurs */
  return data;
}


//XXX can't use this part, because it is implementation
//specific, but its not the important part
//set_constraint_handler(handle_errors);
//
///* Returns zero on success */
//errno_t function(char *dst1){
//  char src1[100] = "hello";
//
//  strcpy_s(dst1, sizeof(dst1), src1);
//  /* At this point strcpy_s may have yielded an
//     error and handle_errors() might have returned */
//
//  /* ... */
//  return 0;
//}
//

//nothing to run
void ERR02() { }


/* ERR03_cpp */

//again, we dont have strcpy_s
//errno_t function(char *dst1, size_t size){
//  char src1[100] = "hello";
//
//  if (strcpy_s(dst1, size, src1) != 0) {
//    return -1;
//  }
//  /* ... */
//  return 0;
//}
//
//nothing to run
void ERR03() { }


/* ERR04_cpp */

void ERR04_0() {
  //XXX changed from priting "Hello, World!"
  //to hello.txt to just printing to stdout
  printf("Hello, World\n!"); 
  abort(); /* oops! data might not get written! */
  /* ... */
}

void ERR04() { }

void ERR05_g(void) {
  /* ... */
  int a = 0;
  int b = 1;
  if (/*something_really_bad_happens*/ a == b) {
    fprintf(stderr, "Something really bad happened!\n");
    abort();
  }
  /* ... */
}

void ERR05() {
  ERR05_g();
  /* ... do the rest of f ... */
}


/* ERR06_cpp */

void ERR06_cleanup() {
  /* delete temporary files, restore consistent state, etc. */
}

void ERR06() {
  int a = 1;
  if (atexit(cleanup) != 0) {
    /* Handle error */
  }


  assert(a == a);

  /* ... */
}


/* ERR07_cpp */

/* Begin {code} */

void ERR07() {
  if (result == -1) {
    /* deal with error */
  }
  /* normal processing from here */
  // /* ... */
}

/* ERR08_cpp */

void ERR08() {
  try {
    doSomething();
  }
  catch( const char *msg ) {
    std::string m( msg );
    if ( m == "stack underflow" ) { ... }
    else if ( m == "connection timeout" ) { ... }
    else if ( m == "security violation" ) { ... }
    else throw;
  }
}


///* ERR09_cpp */
//
///* Begin {code} */
//
//throw new StackUnderflow;
//// /* ... */
//StackUnderflow su1;
//throw &su1;
//// /* ... */
//static StackUnderflow su2;
//throw &su2;
//
///* End {code} */
//
///* Begin {code} */
//
//StackUnderflow su;
//throw su;
//
///* End {code} */
//
///* Begin {code} */
//
//static StackUnderflow su;
//extern std::runtime_error *lastRuntimeError;
//lastRuntimeError = &su;
//throw su;
//
///* End {code} */
//
///* Begin {code} */
//
//catch( StackUnderflow su ) {
//  su.modify(); // modify argument
//  throw; // modifications lost
//}
//
///* End {code} */
//
///* Begin {code} */
//
//catch( std::runtime_error re ) {
//  // /* ... */
//  throw re; // original type of exception lost
//}
//
///* End {code} */
//void ERR09() { }
//
//
///* ERR10_cpp */
//
///* Begin {code} */
//
//char *str = (char *)malloc(strlen(input_string)+1);
//strcpy(str, input_string); /* What if malloc() fails? */
//
///* End {code} */
//void ERR10() { }
//
//
///* ERR30_cpp */
//
///* Begin {code} */
//
//int main(int argc, char** argv) {
//  Object object; // might not get destroyed if exception thrown
//  // do useful work
//  return 0;
//}
//
///* End {code} */
//
///* Begin {code} */
//
//using namespace std;
//class exception1 : public exception {};
//class exception2 : public exception {};
//
//void f(void) throw( exception1) {
//  // /* ... */
//  throw (exception2());
//}
//
//int main(void) {
//  try {
//    f();
//    return 0;
//  } catch (...) {
//    cerr << "F called" << endl;
//  }
//  return -1;
//}
//
///* End {code} */
//void ERR30() { }
//
//
///* ERR31_cpp */
//
///* Begin {code} */
//
//extern int errno;
//
///* End {code} */
//void ERR31() { }
//
//
///* ERR32_cpp */
//
///* Begin {code} */
//
//#include <signal.h>
//#include <stdlib.h>
//#include <string.h>
//
//typedef void (*pfv)(int);
//
//void handler(int signum) {
//  pfv old_handler = signal(signum, SIG_DFL);
//  if (old_handler == SIG_ERR) {
//    perror("SIGINT handler"); /* undefined behavior */
//    /* handle error condition */
//  }
//}
//
//int main(void) {
//  pfv old_handler = signal(SIGINT, handler);
//  if (old_handler == SIG_ERR) {
//    perror("SIGINT handler");
//    /* handle error condition */
//  }
//
//  /* main code loop */
//
//  return 0;
//}
//
///* End {code} */
//
///* Begin {code} */
//
//#include <stddef.h>
//#include <signal.h>
//#include <errno.h>
//#include <sys/wait.h>
//
//void reaper(int signum) {
//  errno = 0;
//  for (;;) {
//    int rc = waitpid(-1, NULL, WNOHANG);
//    if ( (0 == rc) || (-1 == rc && EINTR != errno) )
//      break;
//  }
//  if (ECHILD != errno) {
//    /* handle error */
//  }
//}
//
//int main(void) {
//  struct sigaction act;
//  act.sa_handler = reaper;
//  act.sa_flags = 0;
//  if (sigemptyset(&act.sa_mask) != 0) {
//    /* handle error */
//  }
//  if (sigaction(SIGCHLD, &act, NULL) != 0) {
//    /* handle error */
//  }
//
//  /* ... */
//
//  return 0;
//}
//
///* End {code} */
//void ERR32() { }
//
//
///* ERR33_cpp */
//
///* Begin {code} */
//
//SomeClass::~SomeClass() {
//  // normal processing
//  if (someCondition) {
//    throw someException("something nasty");
//  }
//}
//
///* End {code} */
//void ERR33() { }
//
//
///* ERR34_cpp */
//void ERR34() { }
//
//
///* ERR35_cpp */
//
///* Begin {code} */
//
//class C {
//  private:
//    int x;
//  public:
//    C() : x(0) {
//      try {
//        // /* ... */
//      } catch (...) {
//        if (0 == x) {
//          // /* ... */
//        }
//      }
//    }
//};
//
///* End {code} */
//void ERR35() { }
//
//
///* ERR36_cpp */
//
///* Begin {code} */
//
//// classes used for exception handling
//class B {};
//class D: public B {};
//
//// /* ... */ Using the classes from above
//try {
//  // /* ... */
//} catch (B &b) {
//  // /* ... */
//} catch (D &d) {
//  // /* ... */
//}
//
///* End {code} */
//
///* Begin {code} */
//
//// classes used for exception handling
//class B {};
//class D: public B {};
//
//// /* ... */ Using the classes from above
//try {
//  // /* ... */
//} catch (D &d) {
//  // /* ... */
//} catch (B &b) {
//  // /* ... */
//}
//
///* End {code} */
//void ERR36() { }
//
//
///* ERR37_cpp */
//
///* Begin {code} */
//
//class exception1 : public exception {};
//class exception2 : public exception {};
//
//void foo() {
//  throw exception2; // ok...since foo() promises nothing wrt exceptions
//}
//
//void bar() throw (exception1) {
//  foo();    // bad, since foo() can throw exception2
//}
//
///* End {code} */
//void ERR37() { }
