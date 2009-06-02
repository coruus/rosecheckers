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

enum {ARR_LEN = 100};

/* ARR00_CPP */
void ARR00() {
  int *array = new int[ARR_LEN];
  /* ... */
  delete[] array;
}


/* ARR01_CPP */
void ARR01_0_clear(int array[]) {
  for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
    array[i] = 0;
  }
}
void ARR01_0() {
  int dis[12];
  ARR01_0_clear(dis);
  /* ... */
}

void ARR01_1_clear(int a[ARR_LEN]) {
  memset(a, 0, sizeof(a)); /* error */
}

void ARR01_1() {
  int b[ARR_LEN];
  ARR01_1_clear(b);
  assert(b[ARR_LEN / 2]==0); /* may fail */
}

void ARR01_2() {
  const char *src = "hello, world";
  char *dest = new char [sizeof(src)];
  strcpy(dest, src);
}


void ARR01() {
  ARR01_0();
  ARR01_1();
  ARR01_2();
}

void ARR02() {
  int a[] = {1, 2, 3, 4};
  a[0] = a[1];
}

class Widget {/* ... */};
const int WIDGET_SIZE = 10;
void ARR04() {
  vector<Widget*> container;
  for (size_t i = 0; i < WIDGET_SIZE; i++) {
    container.push_back(new Widget(/* ... */));
  }
  /* Work with container */
} /* frees container */


/* ARR_30_CPP */
enum { TABLESIZE = 100 };

int *ARR30_0_table = NULL;

int ARR30_0_insert_in_table(int pos, int value){
  if (!ARR30_0_table) {
    ARR30_0_table = new int[TABLESIZE];
  }
  if (pos >= TABLESIZE) {
    return -1;
  }
  ARR30_0_table[pos] = value;
  return 0;
}

vector<int> ARR30_1_table;

int ARR30_1_insert_in_table(int pos, int value){
  if (pos >= ARR30_1_table.size()) {
    return -1;
  }
  ARR30_1_table[pos] = value;
  return 0;
}

void ARR30() {
  ARR30_0_insert_in_table(0, 0);
  ARR30_1_insert_in_table(0, 0);
}

/* ARR_31_CPP */

//#include "cpp.ncce.wiki.ARR30_helper.h"
//enum { ARRAYSIZE = 100 };
//
//extern char a[];
//
//void insert_a(void) {
//  a[0] = 'a';
//}
//
//void ARR31(void) {
//  a = new char [ARRAYSIZE];
//  insert_a();
//}

/* ARR_32_CPP */
#include <deque>
using std::deque;

void ARR32() {
double data[5] = { 2.3, 3.7, 1.4, 0.8, 9.6 };

deque<double> d;
deque<double>::iterator pos = d.begin();

for (size_t i = 0; i < 5; ++i) {
  d.insert(pos++, data[i] + 41);
}
}
