#include <stdlib.h>

int main() {
  int number = 3;
  int error_condition = 1;
  int *x = (int *)malloc (number * sizeof(int));
  if (x == NULL) {
    /* Handle Allocation Error */
  }
  /* ... */
  if (error_condition == 1) {
    /* Handle Error Condition*/
    free(x);
  }
  /* ... */
  free(x);
}
