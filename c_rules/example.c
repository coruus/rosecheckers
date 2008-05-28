/**
 * This program serves to demonstrate CERT/CC's secure coding rules,
 * in conjunction with flymake.
 * It compiles with no warnings from gcc, but Rose has some
 * extra information...
 */

#include <stdlib.h>

/** Returns an array of doubles */
double *allocate_array() {
  double *d_array;
  size_t num_elems = 10;
  d_array = (double*) malloc(sizeof(d_array) * num_elems);
  return d_array; // (d_array == NULL) ? NULL : d_array;
}
