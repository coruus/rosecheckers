#include <stdlib.h>
#include <stdio.h>

struct x {
  int array[12];
};

void clear(struct x *var) {
  size_t i;
  for (i = 0; i < sizeof (var->array) / sizeof (var->array[0]); ++i) {
    var->array[i] = 0;
  }
}

int main() {
  /* ... */
  struct x var;
  clear(&var);
  /* ... */
  return 0;
}
