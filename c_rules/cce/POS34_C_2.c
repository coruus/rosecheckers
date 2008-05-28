#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* putenv with dynamic char* */

int POS34_C(char* var) {
  char env[1024];

  if (snprintf(env, sizeof(env),"TEST=%s", var) < 0) {
    /* Handle Error */
  }
  size_t len = strlen(env)+1;
  char* env2 = NULL;
  env2 = (char*) calloc( len, sizeof( char));
  if (env2 == NULL) {
    /* Handle Error */
  }
  strncpy( env2, env, len);

  return putenv(env2);
}
