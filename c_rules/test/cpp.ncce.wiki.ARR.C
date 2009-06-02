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

void ARR() {
  ARR00();
  ARR01();
  ARR02();
  //ARR03();
  ARR04();
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

