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

void FLP02();
void FLP03();
void FLP04();
void FLP05();
void FLP30();
void FLP32();
void FLP33();
void FLP34();
void FLP35();

void FLP() {
  FLP02();
  FLP03();
  FLP04();
  FLP05();
  FLP30();
  FLP32();
  FLP33();
  FLP34();
  FLP35();
}

float FLP02_mean(float array[], int size) {
  float total = 0.0;
  int i;
  for (i = 0; i < size; i++) {
    total += array[i];
    printf("array[%d] = %f and total is %f\n", i, array[i], total);
  }
  if (size != 0) {
    return total / size;
  } else {
    return 0.0;
  }
}

enum {array_size = 10};
float array_value = 10.1;

void FLP02(void) {
  float array[array_size];
  float avg;
  int i;
  for (i = 0; i < array_size; i++) {
    array[i] = array_value;
  }

  avg = FLP02_mean( array, array_size);
  printf("mean is %f\n", avg);
  if (avg == array[0]) {
    printf("array[0] is the mean\n");
  } else {
    printf("array[0] is not the mean\n");
  }
}

void FLP03(void) {
  double a = 1e-40, b, c = 0.1;
  float x = 0, y;
  y = a;
  b = y / x;
  c = sin(30) * a;
}

enum { MAX_VALUE = 256 };

void FLP04(void) {
  float currentBalance; /* User's cash balance */

  float val;

  scanf("%f", &val);

  if(val >= MAX_VALUE - currentBalance) {
    /* Handle range error */
  }

  currentBalance += val;
}

void FLP05(void) {
  float x = 1/3.0;
  printf("Original      : %e\n", x);
  x = x * 7e-45;
  printf("Denormalized? : %e\n", x);
  x = x / 7e-45;
  printf("Restored      : %e\n", x);
}

void FLP30(void) {
  float x;
  for (x = 0.1f; x <= 1.0f; x += 0.1f) {
    /* ... */
  }

  for (x = 100000001.0f; x <= 100000010.0f; x += 1.0f) {
    /* ... */
  }
}

void FLP32(void) {
  const double x = 0;
  const double y = 0; 
  double result;

  result = acos(x);
  result = atan2(y, x);
  result = pow(x, y);
  if (((x == 0.f) && islessequal(y, 0)) ||
      (isless(x, 0))) {
    /* handle domain and range errors */
  }
  result = pow(x, y);
  result = sqrt(x);
}

void FLP33(void) {
  const short a = 533;
  const int b = 6789;
  const long c = 466438237;

  const float d = a / 7; /* d is 76.0 */
  const double e = b / 30; /* e is 226.0 */
  const double f = c * 789; /*  f may be negative due to overflow */
}

void FLP34(void) {
  float f1 = (float)INT_MAX * 2.0;
  int i1;

  /* initializations */

  i1 = (int)f1; /* Undefined if the integral part of f1 > INT_MAX */

  /* End {code} */

  /* Begin {code} */

  const long double ld = 0.0;
  const double d1 = 0.0;
  double d2 = 0.0;
  f1 = 0.0f;
  float f2 = 0.0f;

  /* initializations */

  f1 = (float)d1;
  f2 = (float)ld;
  d2 = (double)ld;
}

void FLP35(void) {
  float a = 3.0;
  float b = 7.0;
  float c = a / b;

  if (c == a / b) {
    puts("Comparison succeeds");
  } else {
    puts("Unexpected result");
  }
}
