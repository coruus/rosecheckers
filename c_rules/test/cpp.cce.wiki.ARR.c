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


void ARR() {
  ARR00();
  ARR01();
  ARR02();
  //ARR03();
  ARR04();
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

/*XXX CANT GET UNIQUE PTR TO WORK:*/
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
