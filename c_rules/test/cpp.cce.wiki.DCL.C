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

void DCL00();
//void DCL02();
void DCL03();
void DCL04();
void DCL05();
void DCL06();
void DCL07();
void DCL08();
void DCL09();
//void DCL10();
//void DCL11();
//void DCL12();
void DCL13();
//void DCL14();
void DCL15();
void DCL16();

void DCL() {
  DCL00();
  //DCL02();
  DCL03();
  DCL04();
  DCL05();
  DCL06();
  DCL07();
  DCL08();
  DCL09();
  //DCL10();
  //DCL11();
  //DCL12();
  DCL13();
  //DCL14();
  DCL15();
  DCL16();
}

/* DCL_00_CPP */

void DCL00_0() {
  const float pi = 3.14159f;
  float degrees;
  float radians;
  /* ... */
  radians = degrees * pi / 180;
}


const int DCL00_1_max = 1<<16;
// /* ... */
void DCL00_1_f(int a) { return; }
void DCL00_1_f(long a) { return; }
// /* ... */
void DCL00_1() {
  DCL00_1_f(DCL00_1_max);
}

//XXX should we try using this at all, or just keep
//it in the code for the declaration being good?
class DCL00_2_Widget {
  // /* ... */
  enum {maxLength = 64};
  // /* ... */
};

void DCL00() {
  DCL00_0();
  DCL00_1();
}


/* DCL_01_CPP */

char DCL01_system_msg[100];

void DCL01_report_error(const char *error_msg) {
  char default_msg[80];
  /* ... */
  if (error_msg)
    strncpy(DCL01_system_msg, error_msg, sizeof(DCL01_system_msg));
  else
    strncpy(DCL01_system_msg, default_msg, sizeof(DCL01_system_msg));
  DCL01_system_msg[ sizeof(DCL01_system_msg) - 1] = '\0';
  return;
}

void DCL01() {
  char error_msg[80];
  /* ... */
  DCL01_report_error(error_msg);
  /* ... */
}


/* DCL_02_CPP */
//blank

/* DCL_03_CPP */

struct DCL03_timer {
  uint8_t MODE;
  uint32_t DATA;
  uint32_t COUNT;
};

/*XXX NOT SUPPORTED BY LINUX??*/
//#if (offsetof(DCL03_timer, DATA) != 4)
//  #error "DATA must be at offset 4"
//#endif

//

#define DCL03_1_JOIN(x, y) DCL03_1_JOIN_AGAIN(x, y)
#define DCL03_1_JOIN_AGAIN(x, y) x ## y

#define static_assert(e) \
  typedef char DCL03_1_JOIN(assertion_failed_at_line_, __LINE__) [(e) ? 1 : -1]

void DCL03_1() {
  static_assert(offsetof(struct DCL03_timer, DATA) == 4);
}

void DCL03() {
  //DCL02_0();
  DCL03_1();
}

/* DCL_04_CPP */

void DCL04() {
  char *src;    /* source string */
  char c;       /* character being tested */

  int i = 1;
  int j = 1;

  size_t mx = 20;
  //allowed to declare on the same line
  //with loop counters
  for (size_t k = 0; k < mx; ++k) {
    /* ... */
  }
}


/* DCL_05_CPP */
typedef void (*DCL05_SighandlerType)(int signum);
extern DCL05_SighandlerType signal(
    int signum, 
    DCL05_SighandlerType handler
    );

//no DCL_05 function
void DCL05() { }


/* DCL_06_CPP */

void DCL06_0() {
  enum { ADULT_AGE=18 };
  int age = 4;
  /* ... */
  if (age >= ADULT_AGE) {
    /* Take action */
  }
  else {
    /* Take a different action */
  }
  /* ... */
}

void DCL06_1() {
  enum { BUFFER_SIZE=256 };

  char buffer[BUFFER_SIZE];
  /* ... */
  fgets(buffer, BUFFER_SIZE, stdin);
}

void DCL06_2() {
  char buffer[256];
  fgets(buffer, sizeof(buffer), stdin);
}


/*XXX how to get this to run with context?*/
//#ifndef PORTNUMBER     /* might be passed on compile line */
//#  define PORTNUMBER 1234
//#endif
//
//#ifndef HOSTNAME        /* might be passed on compile line */
//#  define HOSTNAME "localhost"
//#endif
//
///* ... */
//
//LDAP *ld = ldap_init(HOSTNAME, PORTNUMBER);
//if (ld == NULL) {
//  perror("ldap_init");
//  return(1);
//}

int DCL06_sqrt(int x) {
  return x;
}

void DCL06_4() {
  //filler vars so it will compile
  int a = 1;
  int b = 2;
  int c = 3;
  int x;
  x = (-b + DCL06_sqrt(b*b - 4*a*c)) / (2*a);
}

void DCL06() {
  DCL06_1();
  DCL06_2();
  //DCL06_3();
  DCL06_4();
}


/* DCL_07_CPP */

void DCL07() {
  //
  // other code
  //
  for (size_t i = 0; i < 10; i++) { //contains declaration
    // perform operations
  }
}


/* DCL_08_CPP */

void DCL08() {
  enum { IN_STR_LEN=18, OUT_STR_LEN=IN_STR_LEN+2 };
  enum { ADULT_AGE=18 };
  enum { ALCOHOL_AGE=21 };
}


/* DCL_09_CPP*/

/* Begin {code} */

#include <errno.h>

enum { NO_FILE_POS_VALUES = 3 };

//most implementations don't define errno_t
typedef int errno_t;

errno_t DCL09_opener(
    FILE *file, 
    int *width, 
    int *height, 
    int *data_offset
    ) {
  int file_w;
  int file_h;
  int file_o;
  fpos_t offset;

  if (file == NULL) { return EINVAL; }
  errno = 0;
  if (fgetpos(file, &offset) != 0 ) { return errno; }
  if (fscanf(file, "%i %i %i", &file_w, &file_h, &file_o) 
      != NO_FILE_POS_VALUES) { 
    return EIO; 
  }

  errno = 0;
  if (fsetpos(file, &offset) != 0 ) { return errno; }

  if (width != NULL) { *width = file_w; }
  if (height != NULL) { *height = file_h; }
  if (data_offset != NULL) { *data_offset = file_o; }

  return 0;
}
//nothing to do
void DCL09() { }


/* DCL_10_CPP */
//blank

/* DCL_11_CPP */
//blank


/* DCL_12_CPP */
//compliant solution correctly points out the error via
//a compiler error, so it shouldn't be in the rose test
//suite for false positives


/* DCL_13_CPP */

void DCL13_0(const int * x) {
  if (x != NULL) {
    printf("Value is %d\n", *x);
  }
  /* ... */
}

char* DCL13_strcat_c(char *s1, const char *s2)  {};
void DCL13_1() {

  char *str1 = "str1";
  const char *str2 = "str2";
  char str3[9] = "str3";
  const char str4[9] = "str4";

  DCL13_strcat_c(str3, str2); 
  DCL13_strcat_c(str3, str1);  /* Args reversed to prevent overwriting string literal */ 
  //DCL13_strcat_c(str4, str3);  /* Compiler warns that str4 is const */
}

void DCL13() {
  int x = 4;
  DCL13_0(&x);
  DCL13_1();
}


/* DCL_14_CPP */
//XXX works across multiple files so we can't test for it


/* DCL_15_CPP */
enum {DCL15_MAX = 100};

static int DCL15_helper(int i) {
  /* perform some computation based on i */
}

void DCL15() {
  size_t i;
  int out[DCL15_MAX];

  for (i = 0; i < DCL15_MAX; i++) {
    out[i] = DCL15_helper(i);
  }
  /* ... */
}


/* DCL_16_CPP */
void DCL16() { 
  std::cout << "Sum is " << 1111 + 111L << std::endl;
}


/* DCL_17_CPP */
typedef int Contact;

void DCL17_0_print_vector(vector<Contact>& v){ /* Pass parameter by const reference ensures its integrity */
  for(int i=0; i<v.size(); i++){
    cout<<v[i]<<endl;
  }
}

void DCL17_0() {
  /* ... */
  vector<Contact> vec;
  /* Initialize the vector vec with instantiations of Contact class */
  /* ... */
  DCL17_0_print_vector(vec); /* Fast */
}


bool DCL17_1_compare(const Contact* c1, const Contact* c2) {
  return *c1 < *c2;
}

void DCL17_1_print_vector(const vector<Contact>& v){
  vector<const Contact*> tmp;
  for(int i=0; i<v.size(); i++){
    tmp.push_back(&(v[i]));
  }
  sort(tmp.begin(), tmp.end(), DCL17_1_compare);
  for(int i=0; i<tmp.size(); i++){
    cout<<*tmp[i]<<endl;
  }
}

void DCL17_1() {
  /* ... */
  vector<Contact> vec;
  /* Initialize the vector vec with instantiations of Contact class */
  /* ... */
  DCL17_1_print_vector(vec);
}

void DCL17() {
  DCL17_0();
  DCL17_1();
}
