#include <stdlib.h>
#include <stdio.h>

int dis[12];

void clear(int array[], size_t size) {
  size_t i;
  for (i = 0; i < size; i++) {
     array[i] = 0;
  }
}

int main() {
  clear(dis, sizeof(*&dis) / sizeof(dis[0]));
  return 0;
}

