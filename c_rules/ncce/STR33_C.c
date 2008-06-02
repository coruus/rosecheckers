#include <stdlib.h>
#include <string.h>
#include <wchar.h>

/* This should be caught by the compiler since strlen expects a const char *
 * and not a wchar_t *
 * Moreover, EXP09 will also catch this because we are not using a sizeof() in
 * a call to malloc */

void STR33_C() {
  /* ... */
  wchar_t wide_str1[] = L"0123456789";
  wchar_t *wide_str2 = malloc(strlen(wide_str1) + 1);
  if (wide_str2 == NULL) {
    /* Handle malloc() Error */
  }
  /* ... */
  free(wide_str2);
}

