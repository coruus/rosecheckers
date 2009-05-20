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
  const char* random1 = (char *)malloc(10);
  const unsigned char* random2 = (unsigned char*)malloc(20);
  const signed char* random3 = (signed char*)malloc(5);
  int **matrix = (int**) malloc( 10 * sizeof( int*));
  matrix[0] = (int*)realloc( matrix[0], sizeof(int)*10);
  const char* random4 = (char *)malloc(sizeof(char));
}


void* Malloc(size_t amt) {
    void* ptr;
    
    if(amt == 0)
        return NULL;

    ptr = (void*) malloc(amt);
    if (ptr == NULL) {
        printf("Memory error\n");
        exit(1);
    }
    return ptr;
}


void EXP09_2() {
  const char* foo = (char*) Malloc(5);

  const size_t ptr_size = sizeof(char *);
  size_t i;
  
  if (ptr_size > SIZE_MAX/100) return;
  char **triarray = (char**) calloc(100, ptr_size);
  if (!triarray) {
    /* handle error */
  }

  for (i = 0; i < 100; i++) {
    triarray[i] = (char*) calloc(20, 0 * 1 + 1 * 1 + 1 * 0);
    if (!triarray[i]) {
      /* handle error */
    }
  }
}
