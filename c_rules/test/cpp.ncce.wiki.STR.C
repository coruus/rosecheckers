/*
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "util.h"
#include "cpp.util.h"

void STR01();
void STR02();
void STR03();
void STR05();
void STR06();
void STR07();
void STR30();
void STR31();
void STR32();
void STR33();
void STR34();
void STR35();
void STR37();
void STR38();
void STR39();

void STR() {
  STR01();
  STR02();
  STR03();
  STR05();
  STR06();
  STR07();
  STR30();
  STR31();
  STR32();
  STR33();
  STR34();
  STR35();
  STR37();
  STR38();
  STR39();
}

void STR01(void) {
  char* s = new char[32];
  delete[] s;
}

void STR02(void) {
  char buffer[32];
  char addr[10];
  sprintf(buffer, "/bin/mail %s < /tmp/email", addr);
  system(buffer);
}

void STR03(void) {
  char *string_data=NULL;
  char a[16];
  strncpy(a, string_data, sizeof(a));
}

/* GCC gets this
void STR04(void) {
  size_t len;
  char cstr[] = "char string";
  signed char scstr[] = "signed char string";
  unsigned char ucstr[] = "unsigned char string";

  len = strlen(cstr);
  len = strlen(scstr);  
  len = strlen(ucstr);  
}
*/

void STR05(void) {
  char *c = "Hello";
  printf("%s", c);
}

void STR06(void) {
  char *token;
  char *path = getenv("PATH");

  token = strtok(path, ":");
  puts(token);

  while (token = strtok(0, ":")) {
    puts(token);
  }

  printf("PATH: %s\n", path);
  /* PATH is now just "/usr/bin" */
}

void STR07(void) {
  char ch = 'b';
  if ( ( ch >= 'a' ) && (ch <= 'c') ){
  }
}

void STR30(void) {
  char *p  = "string literal";
  p[0] = 'S';
}

void STR31(void) {
  char dest[32];
  char src[32];
  src[0]='a';
  size_t i;
  /* ... */
  for (i=0; src[i] && (i < sizeof(dest)); i++) {
    dest[i] = src[i];
  }
  dest[i] = '\0';
  /* ... */

  printf("%s", dest);
}

void STR32(void) {
  char source[32];
  char ntca[32];

  ntca[sizeof(ntca)-1] = '\0';
  strncpy(ntca, source, sizeof(ntca));

  char ntca2[32];
  memset(ntca2, 0, sizeof(ntca2)-1);
  strncpy(ntca2, source, sizeof(ntca2)-1);
}

void STR33(void) {
  char str[100];

  string *ps = new string("Goodnight Irene");
  strcpy(str, ps->data());
  strcat(str, " I'll see you in my dreams");
}

void STR34(void) {
  register char *string;
  register int c;

  char *s=NULL;

  string = s;
  c = EOF;

  /* If the string doesn't exist, or is empty, EOF found. */
  if (string && *string) {
    c = *string++;
    s = string;
  }

  printf("%s", s);
  printf("%d", c);
}

void STR35(void) {
  char buf[BUFSIZ], *p;
  int ch;
  p = buf;
  while ( ((ch = getchar()) != '\n')
	  && !feof(stdin)
	  && !ferror(stdin))
    {
      *p++ = ch;
    }
  *p++ = 0;
}

/* Gcc catches
void STR36(void) {
  const char s[3] = "abc";
}
*/

size_t STR37_count_preceding_whitespace(const char *s) {
  const char *t = s;
  size_t length = strlen(s) + 1;

  /* possibly *t < 0 */
  while (isspace(*t) && (t - s < length)) {
    ++t;
  }
  return t - s;
}

void STR37(void) {
  (void)STR37_count_preceding_whitespace("   asjkdfld");
}

void STR38(void) {
  char input[] = "bogus@addr.com; cat /etc/passwd";
  string email;
  string::iterator loc = email.begin();

  // copy into string converting ";" to " "
  for (size_t i=0; i <= strlen(input); i++) {
    if (input[i] != ';') {
      email.insert(loc++, input[i]);
    }
    else {
      email.insert(loc++, ' ');
    }
  } // end string for each element in NTBS
}

void STR39(void) {
  string bs("01234567");
  size_t i = 4;

  bs[i] = '\0';
}



