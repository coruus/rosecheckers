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

void DCL01_1(void) {
  int tmp=1;
  while(1) {
    int tmp=0;
    /* use tmp */
    printf("tmp=%d\n", tmp);
  }
}

void DCL02_1(void) {
  const int notel = 6;
  const int note1 = 4;
  if (notel == note1) {
    printf("We found notel to be wrong");
  }
}

void DCL02_2(void) {
  const int home = 6;
  const int nome = 4;
  if (home == nome) {
    printf("We found home to be wrong");
  }
}
