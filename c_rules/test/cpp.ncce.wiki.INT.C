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

#define SIZE_MAX 256

void INT00();
void INT01();
void INT02();
void INT04();
void INT05();
void INT06();
void INT07();
void INT08();
void INT09();
void INT10();
void INT11();
void INT12();
void INT13();
void INT14();
void INT30();
void INT31();
void INT32();
void INT33();
void INT34();
void INT35();

void INT() {
  INT00();
  INT01();
  INT02();
  INT04();
  INT05();
  INT06();
  INT07();
  INT08();
  INT09();
  INT10();
  INT11();
  INT12();
  INT13();
  INT14();
  INT30();
  INT31();
  INT32();
  INT33();
  INT34();
  INT35();
}

void INT00() {
  unsigned int a;
  unsigned int b;
  unsigned long c;
  /* Initialize a and b */
  c = static_cast<unsigned long>(a) * b; /* not guaranteed to fit */
}

void *INT01_alloc(unsigned int blocksize) {
  return malloc(blocksize);
}

void INT01() {
  free(INT01_alloc(4));
}

void INT02() {
  int si = -1;
  unsigned ui = 1;
  cout << (si < ui) << endl;
}

void INT04_create_table(size_t length) {
  char **table;

  if (sizeof(char *) > SIZE_MAX/length) {
    /* handle overflow */
  }

  const size_t table_length = length * sizeof(char *);
  table = (char **)malloc(table_length);

  if (table == NULL) {
    /* Handle error condition */
  }
  else
    free(table);
  /* ... */
}

void INT04() {
  INT04_create_table(4);
}

void INT05() {
  long sl;
  if(scanf("%ld", &sl) != 1) {}
}

void INT06() {
  int si;
  si = atoi("4");
}


void INT07() {
  char c = 200;
  int i = 1000;
  cout << "i/c = " << i/c << endl;
}

void INT08() {
  int i = 32766 + 1;
  i++;
}

void INT09() {
  enum { red=4, orange, yellow, green, blue, indigo=6, violet};
}

int INT10_insert(int INT10_index, int *list, int size, int value) {
  if (size != 0) {
    INT10_index = (INT10_index + 1) % size;
    list[INT10_index] = value;
    return INT10_index;
  }
  else {
    return -1;
  }
}

void INT10() {
  int list[10];

  if(INT10_insert(0, list, -1, 4) != 0) {}
}


void INT11() {
  char *ptr = NULL;
  const unsigned int flag = 0;

  unsigned int number = (unsigned int)ptr;
  number = (number & 0x7fffff) | (flag << 23);
  ptr = (char *)number;
}

struct {
  int a: 8;
} INT12_bits = {225};

void INT12() {
  cout << "bits.a = " << INT12_bits.a << endl;
}

void INT13() {
  int rc = 0;
  const int stringify = 0x80000000;
  char buf[sizeof("256")];
  rc = snprintf(buf, sizeof(buf), "%u", stringify >> 24);
  if (rc == -1 || rc >= sizeof(buf)) {
    /* handle error */
  }
}

void INT14() {
  unsigned int x = 50;
  x += (x << 2) + 1;
}

void INT30() {
  const unsigned int ui1 = 0, ui2 = 0;
  unsigned int sum;

  sum = ui1 + ui2;
}

void INT31() {
  unsigned long int ul = ULONG_MAX;
  signed char sc;
  sc = static_cast<signed char>(ul);
}

void INT32() {
  const int si1 = 0, si2 = 0;
  const int sum = si1 + si2;
  
  signed int result;
  
  result = -si1;
}

void INT33() {
  const signed long sl1 = 0;
  signed long sl2;
  sl2 = random();
  
  const signed long result = sl1/sl2;
}

void INT34() {
  const int si1 = 0, si2 = 0;

  const int sresult = si1 << si2;
}


enum { INT35_BLOCK_HEADER_SIZE = 16 };
unsigned long long INT35_max = UINT_MAX;

void *INT35_AllocateBlock(size_t length) {
  struct memBlock *mBlock;

  if (length + INT35_BLOCK_HEADER_SIZE > INT35_max) return NULL;
  mBlock 
    = (struct memBlock *)malloc(length + INT35_BLOCK_HEADER_SIZE);
  if (!mBlock) return NULL;

  /* fill in block header and return data portion */

  return mBlock;
}


void INT35() {
  free(INT35_AllocateBlock(10));
}
