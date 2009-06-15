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

void EXP09_1() {
  const char** random2 = (const char**) malloc(4);
  int **matrix = (int**) malloc(10*sizeof(int*));
  matrix[0] = (int*)realloc( matrix[0], 10*4);
}

void EXP09_2() {
  size_t i;
  int **triarray = (int**) malloc(100 * 4);

  if (!triarray) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    triarray[i] = (int*) malloc(20 * 4);
    if (!triarray[i]) {
      /* handle error */
    }
  }
}

void EXP30() {
  int i;
  for(i = 0; i < 10; i = i++ + 1){
    /* Loop */
  }
}
