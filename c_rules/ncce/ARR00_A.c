#include <stdlib.h>

void clear(int array[]) {
  size_t i;
  for (i = 0; i < sizeof (array) / sizeof (array[0]); ++i) {
     array[i] = 0;
   }
}

int main() {
  /* ... */
  int dis[12];
  clear(dis);
  /* ... */
  return 0;
}
