#include <stdio.h>
#include <stdlib.h>

void exit1(void) {
  /* ...cleanup code... */
  return;
}

void exit2(void) {
  if (1) {
    /* ...more cleanup code... */
    exit(0);
  }
  return;
}

int main(void) {
  atexit(exit1);
  atexit(exit2);
  /* ...program code... */
  exit(0);
}
