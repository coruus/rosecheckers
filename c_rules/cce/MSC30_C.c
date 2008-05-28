#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void MSC30_C() {
  enum {len = 12};
  char id[len];  /* id will hold the ID, starting with the characters "ID" */
  /* followed by a random integer */
  int r;
  int num;
  /* ... */
  srandom(time(0));  /* seed the PRNG with the current time */
  /* ... */
  r = random();  /* generate a random integer */
  num = snprintf(id, len, "ID%-d", r);  /* generate the ID */
  printf("Random number is %d\n", num);
  /* ... */
}

