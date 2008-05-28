
#include <wchar.h>
#include <stdlib.h>

/* This code is actually caught by EXP09, which flags malloc w/o
   sizeof for non-char types */

void STR33_C() {
  /* ... */
  wchar_t wide_str1[] = L"0123456789";
  wchar_t *wide_str3 = malloc(wcslen(wide_str1) + 1);
  if (wide_str3 == NULL) {
    /* Handle malloc() Error */
  }
  /* ... */
  free(wide_str3);
}
