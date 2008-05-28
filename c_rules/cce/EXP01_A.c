#include <stdlib.h>

double *EXP01_A() {
  double *d_array;
  size_t num_elems = 10;
  /* ... */

  return (double*) malloc(sizeof(*d_array) * num_elems);
}
