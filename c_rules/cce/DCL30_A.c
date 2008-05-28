
void this_is_OK() {
    const char str[] = "Everything OK";
    const char *p = str;
    /* ... */
    const char* x = p;
    const char* y = x;
    x = y; // make rose & gcc shut up about unused vars
}
/* pointer p is now inaccessible outside the scope of string str */
