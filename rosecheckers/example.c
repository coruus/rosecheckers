#include <stdlib.h>

/** Returns an array of doubles */
double *allocate_array() {
  double *d_array;
  size_t num_elems = 10;
  d_array = (double*) malloc(sizeof(d_array) * num_elems);
  return d_array; // (d_array == NULL) ? NULL : d_array;
}
