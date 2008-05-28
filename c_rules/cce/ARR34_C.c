#include <stdlib.h>

int ARR34_C(int array[]) {
  enum { a = 10, b = 10, c = 20 };

  int arr1[c][b];
  int (*arr2)[a];

  arr2 = arr1; /* OK, because a == b */
  return (arr2 != 0);
}

