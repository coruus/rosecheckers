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
//void ERR03();
void ERR04();
void ERR05();
void ERR06();
void ERR07();
void ERR08();
void ERR09();
void ERR10();
void ERR30();
//void ERR31();
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
  //ERR03();
  ERR04();
  ERR05();
  ERR06();
  ERR07();
  ERR08();
  ERR09();
  ERR10();
  ERR30();
  //ERR31();
  ERR32();
  ERR33();
  ERR34();
  ERR35();
  ERR36();
  ERR37();
}



/* ERR00_cpp */
//no code

/* ERR01_cpp */

void ERR01() { 
  printf("This\n");
  printf("is\n");
  printf("a\n");
  printf("test.\n");
  if (ferror(stdout)) {
    fprintf(stderr, "printf failed\n");
  }
}


/* ERR02_cpp */
//XXX context specific
//errno_t sprintf_m(
//    string_m buf, 
//    const string_m fmt, 
//    int *count, 
//    /* ... */
//    );
//
//
//int i;
//rsize_t count = 0;
//errno_t err;
//
//for (i = 0; i < 9; ++i) {
//  err = sprintf_m(
//      buf + count, "%02x ", &count, ((u8 *)&slreg_num)[i]
//      );
//  if (err != 0) {
//    /* Handle print error */
//  }
//}
//err = sprintf_m(
//    buf + count, "%02x ", &count, ((u8 *)&slreg_num)[i]
//    );
//if (err != 0) {
//  /* Handle print error */
//}


/*
* The abort_handler_s() function writes a message on the
* standard error stream and then calls the abort() function.
*/

//XXX abort_handler_s blankly typedef'd in cpp.util.h
//XXX XXX won't work:
//set_constraint_handler(abort_handler_s);

//XXX this part won't work on GCC but isn't important
///*...*/
//
///* Returns zero on success */
//errno_t function(char *dst1){
//  char src1[100] = "hello";
//
//  strcpy_s(dst1, sizeof(dst1), src1);
//  /* Because abort_handler_s() never returns,
//     we only get here if strcpy_s() succeeds. */
//
//  /* ... */
//  return 0;
//}
//
///* End {code} */

void ERR02() { }


/* ERR03_cpp */

constraint_handler_t handle_errors(void) {
  /* Handle runtime constraint error */
}

//XXX XXX won't work:
//set_constraint_handler_s(handle_errors);


//we don't have strcpy_s
/* Returns zero on success */
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
//void ERR03() { }


/* ERR04_cpp */
void ERR04_0() {
  int a = 0;
  int b = 1;
  if (a == b)
    exit(EXIT_FAILURE);
}

void ERR04_1() {
  int a = 0;
  int b = 1;
  if (a == b) {
    _Exit(EXIT_FAILURE);
  }
}

void ERR04_2() {
  printf("Hello, World");
  /* ... */
  exit(EXIT_FAILURE); /* writes data & closes f. */
  /* ... */
}

void ERR04() {
  ERR04_0();
  ERR04_1();
  ERR04_2();
}


/* ERR05_cpp */

const errno_t ERR05_ESOMETHINGREALLYBAD = 1;

errno_t ERR05_0_g(){
  int a = 0;
  int b = 1;
  if (a == b) {
    return ERR05_ESOMETHINGREALLYBAD;
  }
  /* ... */
  return 0;
}

errno_t ERR05_0() {
  errno_t status = ERR05_0_g();
  if (status != 0) return status;

  /* ... do the rest of f ... */

  return 0;
}

void ERR05_1_g(errno_t* err) {
  int a = 0;
  int b = 1;
  if (err == NULL) {
    /* Handle null pointer */
  }
  /* ... */
  if (/*something_really_bad_happens*/ a == b) {
    *err = ERR05_ESOMETHINGREALLYBAD;
  } else {
    /* ... */
    *err = 0;
  }
}

//we can't call this... so ERR05_1 is just skipped
void ERR05_1_f(errno_t* err) {
  if (err == NULL) {
    /* Handle null pointer */
  }
  ERR05_1_g(err);
  if (*err == 0) {
    /* ... do the rest of f ... */
  }
}

errno_t ERR05_2_my_errno; /* also declared in a .h file */

void ERR05_2_g() {
  int a = 0;
  int b = 1;
  /* ... */
  if (a == b) {
    ERR05_2_my_errno = ERR05_ESOMETHINGREALLYBAD;
    return;
  }
  /* ... */
}

void ERR05_2() {
  int a = 0;
  int b = 1;
  ERR05_2_my_errno = 0;
  ERR05_2_g();
  if (ERR05_2_my_errno != 0) {
    return;
  }
  /* ... do the rest of f ... */
}

#include <setjmp.h>

jmp_buf ERR05_3_exception_env;

void ERR05_3_g(void) {
  int a = 0;
  int b = 1;
  if (a == b) {
    longjmp(ERR05_3_exception_env, ERR05_ESOMETHINGREALLYBAD);
  }
  /* ... */
}

void ERR05_3_f() {
  ERR05_3_g();
  /* ... do the rest of f ... */
}


void ERR05_3() {
  errno_t ERR05_3_err = setjmp(ERR05_3_exception_env);
  ERR05_3_f();

  /* ... */
  if (ERR05_3_err != 0) {
    /* if we get here, an error occurred
       and err indicates what went wrong */
  }
  /* ... */
  /* ... */
}

void ERR05() { 
  ERR05_0();
  ERR05_2();
  ERR05_3();
}

/* ERR06_cpp */

/* Begin {code} */

void ERR06_cleanup() {
  /* delete temporary files, restore consistent state, etc */
}

void ERR06() {
  int a = 0;
  if (atexit(ERR06_cleanup) != 0) {
    /* Handle error */
  }

  /* ... */

  if (a == a) {
    exit(EXIT_FAILURE);
  }

  /* ... */
}


/* ERR07_cpp */

/* Begin {code} */


void ERR07(){
  int result = 0;
  if (result == -1) {
    throw runtime_error("problem");
  }
}

/* ERR08_cpp */

// /* ... */
void ERR08() {
  try {
    printf("Hello, World\n");
  }
  catch( StackUnderflow &su ) { throw; }
  // /* ... */
}

/* ERR09_cpp */
//XXX dont run!!
void ERR09_0() {
  throw StackUnderflow();

  try { }
  catch( StackUnderflow &su ) {
    su.modify(); // modify exception object
    throw; // modifications not lost
  }
  catch( std::runtime_error &re ) {
    // /* ... */
    throw; // original type of exception not lost
  }
}

void ERR09() { }


/* ERR10_cpp */
void ERR10_0() { 
  char input_string[] = "input";
  char *str = (char *)malloc(strlen(input_string)+1);
  if (str == NULL) {
    /* Handle Allocation Error */
  }
  strcpy(str, input_string);
}

void ERR10_1() {
  char input_string[] = "input";
  char *str = new char [strlen(input_string)+1];
  strcpy(str, input_string);
}

void ERR10() { 
  ERR10_0();
  ERR10_1();
}


/* ERR30_cpp */

void ERR30_0() {
  string object;
  bool error = false;

  try {
    // do useful work
  } catch (...) {
    error = true;
  }

  return;
}

/* End {code} */

/* Begin {code} */

int main(int argc, char** argv) {
  try {
    string object;
    // do useful work
    return 0; // object gets destroyed here
  } catch (...) {
    throw;  
  }
}

class exception1 : public exception {};
class exception2 : public exception {};

void ERR30_1_f(void) throw( exception1) {
  // /* ... */
  throw (exception1());
}

void ERR30_1() {
  try {
    ERR30_1_f();
  } catch (exception1 &e) {
    cerr << "F called" << endl;
  }
}

/* End {code} */
void ERR30() { }


//nothing useful to do
//void ERR31() { }


/* ERR32_cpp */

/* Begin {code} */

typedef void (*pfv)(int);

void ERR32_handler(int signum) {
  pfv old_ERR32_handler = signal(signum, SIG_DFL);
  if (old_ERR32_handler == SIG_ERR) {
    abort();
  }
}

void ERR32_0(void) {
  pfv old_ERR32_handler = signal(SIGINT, ERR32_handler);
  if (old_ERR32_handler == SIG_ERR) {
    perror("SIGINT ERR32_handler");
    /* handle error condition */
  }

  /* main code loop */
}

void ERR32_reaper(int signum) {
  int save_errno = errno;
  errno = 0;
  for (;;) {
    int rc = waitpid(-1, NULL, WNOHANG);
    if ( (0 == rc) || (-1 == rc && EINTR != errno) )
      break;
  }
  if (ECHILD != errno) {
    /* handle error */
  }
  errno = save_errno;
}

void ERR32_1() {
  struct sigaction act;
  act.sa_handler = ERR32_reaper;
  act.sa_flags = 0;
  if (sigemptyset(&act.sa_mask) != 0) {
    /* handle error */
  }
  if (sigaction(SIGCHLD, &act, NULL) != 0) {
    /* handle error */
  }

  /* ... */
}

void ERR32() { 
  ERR32_0();
  ERR32_1();
}


/* ERR33_cpp */
class ERR33_X {
  public:
    ERR33_X() { }
    ~ERR33_X() {
      try {
        printf("bye bye\n");
      }
      catch(...) {
      }
    }
};

void ERR33() {
  ERR33_X();
}


/* ERR34_cpp */

class ERR34_Counter {
  public:
    static int Instances;

    ERR34_Counter() {Instances++;}
    ~ERR34_Counter() {Instances--;}
  private:
    ERR34_Counter(const ERR34_Counter& that);
    ERR34_Counter& operator=(const ERR34_Counter& that);
};

int ERR34_Counter::Instances = 0;
class ERR34_Error {};

void ERR34_func() {
  ERR34_Counter c;
  cout << "ERR34_func(): Instances: " << ERR34_Counter::Instances << endl;
  throw ERR34_Error();
}

void ERR34() {

  cout << "Before try: Instances: " << ERR34_Counter::Instances << endl;
  try {
    ERR34_func();
  } catch (...) {
    cout << "In catch: Instances: " << ERR34_Counter::Instances << endl;
  }

  cout << "After catch: Instances: " << ERR34_Counter::Instances << endl;
}

/* ERR35_cpp */

/* Begin {code} */

class ERR35_C {
  private:
    int x;
  public:
    ERR35_C() : x(0) {
      int y = x;
      try {
        // /* ... */
      } catch (...) {
        if (0 == y) {
          // /* ... */
        }
      }
    }
};

/* End {code} */
void ERR35() { 
  ERR35_C a = ERR35_C();
}


///* ERR36_cpp */
//void ERR36() { }
//
//
///* ERR37_cpp */
//
///* Begin {code} */
//
//void bar() throw (exception1) {
//  try {
//    foo();
//  } catch (...) {
//    // handle error, without re-throwing it
//  }
//}
//
///* End {code} */
//void ERR37() { }
