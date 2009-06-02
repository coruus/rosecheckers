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

#include <vector>
using std::vector;

void ARR00();
void ARR01();
void ARR02();
//void ARR03();
void ARR04();
void ARR30();
//void ARR31();
void ARR32();


void ARR() {
  ARR00();
  ARR01();
  ARR02();
  //ARR03();
  ARR04();
  ARR30();
  //ARR31();
  ARR32();
}

/*use for all such standard array lengths*/
enum {ARR_LEN = 100};

/* ARR_00_CPP */
enum {ARR00_ARRAY_SIZE = 100};
void ARR00() {
  std::vector<int> array(ARR_LEN);
}

/* ARR_01_CPP */
void ARR01_0_clear(int array[], size_t len) {
  for (size_t i = 0; i < len; i++) {
    array[i] = 0;
  }
}
void ARR01_0() {
  int dis[12];
  ARR01_0_clear(dis, sizeof(dis) / sizeof(dis[0]));
  /* ... */
}


void ARR01_1_clear(int a[], size_t len) {
  memset(a, 0, len * sizeof(int));
}

void ARR01_1() {
  int b[ARR_LEN];
  ARR01_1_clear(b, ARR_LEN);
  assert(b[ARR_LEN / 2]==0); /* cannot fail */
}


template <typename T, int N>
inline int  ARR01_arraySize(T (&)[N])   { return N; }

void ARR01_2() {
  char src[] = "hello, world";
  char *dest = new char[ARR01_arraySize(src)];
  strcpy(dest, src);
}

void ARR01() {
  ARR01_0();
  ARR01_1();
  ARR01_2();
}

/* ARR_02_CPP */
void ARR02() {
  int a[4] = {1, 2, 3, 4};
  //to satisfy the warnings
  a[0] = a[1];
}

/* ARR_03_CPP */
/*no example compliance code from wiki*/

class Widget {/**/};
const int WIDGET_SIZE = 10;

/* ARR_04 CPP */
void ARR04_0() {
  vector<Widget*> container;
  for (size_t i = 0; i < WIDGET_SIZE; i++) {
    container.push_back(new Widget(/* ... */));
  }
  /* Work with container */
  /* Free container's widgets*/
  for (vector<Widget*>::iterator it = container.begin(); it != container.end(); ++it) {
    delete *it;
  }
  /* frees container */
}

#include <utility>
/*XXX CANT GET UNIQUE PTR TO WORK:*/
//using std::unique_ptr;
//void ARR04_1() {
//  vector< unique_ptr<Widget> > container;
//  for (size_t i = 0; i < WIDGET_SIZE; i++) {
//    container.push_back(new Widget(/* ... */));
//  }
//  /* Work with container */
//  /* frees container, which frees widgets */
//} 
//
//void ARR04_2() {
//  Widget[WIDGET_SIZE] widgets;
//  {
//    for (size_t i = 0; i < WIDGET_SIZE; i++) {
//      widgets[i] = Widget(/* ... */);
//    }
//
//    vector< unique_ptr<Widget> > container;
//    for (size_t i = 0; i < WIDGET_SIZE; i++) {
//      container.push_back( widgets[i]);
//    }
//    /* Work with container */
//    /* frees container, but not widgets */
//  }
//  /* ... */
//} /* widgets freed now */

void ARR04() {
  ARR04_0();
}

/*RULES*/

/* ARR_30_CPP */
enum { TABLESIZE = 100 };

int *ARR30_0_table = NULL;

int ARR30_0_insert_in_table(size_t pos, int value){
  if (!ARR30_0_table) {
    ARR30_0_table = new int[TABLESIZE];
  }
  if (pos >= TABLESIZE) {
    return -1;
  }
  ARR30_0_table[pos] = value;
  return 0;
}

template <typename T>
void ARR30_1_clear(T array[], size_t n) {
  for (size_t i = 0; i < n; ++i) {
    array[i] = 0;
  }
}
template <typename T, size_t n>
inline void ARR30_1_clear(T (&array)[n]) {
  ARR30_1_clear(array, n);
}
void ARR30_1() {
  int int_array[12];
  ARR30_1_clear(int_array); // deduce T is int, and that n is 12
}

vector<int> ARR30_2_table;

int ARR30_2_insert_in_table(size_t pos, int value){
  if (pos >= ARR30_2_table.size()) {
    return -1;
  }
  ARR30_2_table[pos] = value;
  return 0;
}

vector<int> ARR30_3_table;

int ARR30_3_insert_in_table(size_t pos, int value){
  ARR30_3_table.at(pos) = value;
  return 0;
}

void ARR30() {
  ARR30_0_insert_in_table(0, 0);
  ARR30_1();
  ARR30_2_insert_in_table(0, 0);
  ARR30_3_insert_in_table(0, 0);
}

/* ARR_31_CPP */
/*XXX How would we detect something across
 * multiple files?*/
/* insert_a.hpp include file */
//enum { ARRAYSIZE = 100 };
//
//extern char *a;
//void insert_a(void);
///* end include file */
//
//char *a;
//void insert_a(void) {
//   a[0] = 'a';
//}
//
//void ARR31(void) {
//  a = new char [ARRAYSIZE];
//  insert_a();
//}


/* ARR_32_CPP */
#include <deque>
using std::deque;
#include <algorithm>

void ARR32_0() {
  double data[5] = { 2.3, 3.7, 1.4, 0.8, 9.6 };

  deque<double> d;
  deque<double>::iterator pos = d.begin();

  for (size_t i = 0; i < 5; ++i) {
    pos = d.insert(pos, data[i] + 41);
    ++pos;
  }
}

/*XXX Not sure how to get this example to work*/
//void ARR32_1() {
//  double data[5] = { 2.3, 3.7, 1.4, 0.8, 9.6 };
//  deque<double> d;
//
//  transform(data, data+5,
//      inserter(d, d.begin()),
//      bind2nd(plus<int>(), 41));
//}

void ARR32() {
  ARR32_0();
  //ARR32_1();
}
