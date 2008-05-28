#include <stdlib.h>

/* use setenv(), not putenv() */

int POS34_C(char *var) {
  return setenv("TEST", var, 1);
}
