#include <stdio.h>

const char *p;
void dont_do_this() {
    const char str[] = "This will change";
    p = str; /* dangerous */
    /* ... */
}

void innocuous() {
    const char str[] = "Surprise, surprise";
    const char* x = str;
    const char* y = x;
    x = y; // make rose & compiler shut up wrt unused vars
}

int main() {
  /* ... */
  dont_do_this();
  innocuous();
  /* now, it is likely that p is pointing to "Surprise, surprise" */
  printf( p);
  // actually p points to junk on my build (ubuntu 7.10, g++ 4.1, amd64)
  return 0;
}

