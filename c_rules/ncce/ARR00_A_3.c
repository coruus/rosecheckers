#include <stdlib.h>
#include <stdio.h>

int dis[12];

void clear(int array[]) {
  char* tmp = (char*) malloc(sizeof(*&array));
  if (tmp == NULL) {
    /* handle error */
  }
  /* ... */
  free( tmp);
}

int main(int argc, char ** argv) {
  clear(dis);
  return 0;
}
