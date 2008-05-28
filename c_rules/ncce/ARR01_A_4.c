#include <stdlib.h>

void clear(int array[], int array2[]) {
  size_t i;
  for (i = 0; i < sizeof (array2) / sizeof (array2[0]); ++i) {
     array2[i] = 0;
   }
}

int main() {
  /* ... */
  int dis[12];
  clear(dis, dis);
  /* ... */
  return 0;
}
