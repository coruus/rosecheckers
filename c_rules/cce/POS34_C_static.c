#include <stdio.h>
#include <stdlib.h>

/* putenv with local array */

int POS34_C(char *var) {
  static char env[1024];

  if (snprintf(env, sizeof(env),"TEST=%s", var) < 0) {
    /* Handle Error */
  }

  return putenv(env);
}
