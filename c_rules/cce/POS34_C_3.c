#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/* putenv with static array */

char array[20] = "TEST=foo";

int POS34_c(char *var) {
  return putenv( array);
}
