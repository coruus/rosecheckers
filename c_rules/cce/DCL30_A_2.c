#include <stdlib.h> // import NULL

const char *p;
void is_this_OK() {
    const char str[] = "Everything OK?";
    p = str;
    /* ... */
    p = NULL;
}
