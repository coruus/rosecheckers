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
void STR03();
void STR04();
void STR05();
void STR06();
void STR07();
void STR30();
void STR31();
void STR32();
void STR33();
void STR34();
void STR35();
void STR36();
void STR37();
void STR38();
void STR39();

void STR() {
  STR01();
  STR03();
  STR04();
  STR05();
  STR06();
  STR07();
  STR30();
  STR31();
  STR32();
  STR33();
  STR34();
  STR35();
  STR36();
  STR37();
  STR38();
  STR39();
}

void STR01(void) {
  string s;
}

void STR03(void) {
  char *string_data=NULL;
  char a[16];

  if (string_data == NULL) {
    /* Handle null pointer error */
  }
  else if (strlen(string_data) >= sizeof(a)) {
    /* Handle overlong string error */
  }
  else {
    strcpy(a, string_data);
  }
}

void STR04(void) {
  size_t len=0;
  char cstr[] = "char string";

  len = strlen(cstr);

  printf("%d", len);
}

void STR05(void) {
  const char *c = "Hello";
  printf("%s", c);
}

void STR06(void) {
  char *token;
  const char *path = getenv("PATH");
  /* PATH is something like "/usr/bin:/bin:/usr/sbin:/sbin" */

  char *copy = (char *)malloc(strlen(path) + 1);
  if (copy == NULL) {
    /* handle error */
  }
  strcpy(copy, path);
  token = strtok(copy, ":");
  puts(token);

  while (token = strtok(0, ":")) {
    puts(token);
  }

  free(copy);
  copy = NULL;
}

void STR07(void) {
  char ch = 't';
  if ( ( ch == 'a' ) || ( ch == 'b') || ( ch == 'c') ){
    
  }
}

void STR30(void) {
  char a[] = "string literal";
  a[0] = 'S';
  printf("%s", a);
}

void STR31(void) {
  char dest[32];
  char src[32];
  src[0]='a';
  size_t i=0;
  /* ... */
  for (i=0; src[i] && (i < sizeof(dest)-1); i++) {
    dest[i] = src[i];
  }
  dest[i] = '\0';

  printf("%s", dest);
}

void STR32(void) {
  char ntca[32];
  char source[32];
  strncpy(ntca, source, sizeof(ntca)-1);
  ntca[sizeof(ntca)-1] = '\0';
}

void STR33(void) {
  char str[100];

  string *ps = new string("Goodnight Irene");
  strcpy(str, ps->c_str());
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
    /* cast to unsigned type */
    c = (unsigned char)*string++;

    s = string;
  }

  printf("%c", c);
  printf("%s", s);
}

void STR35(void) {
  unsigned char buf[32];
  buf[0] = 'a';
  int ch;
  int index = 0;
  int chars_read = 0;
  while ( ( (ch = getchar()) != '\n')
	  && !feof(stdin)
	  && !ferror(stderr) )
    {
      if (index < 32-1) {
	buf[index++] = (unsigned char)ch;
      }
      chars_read++;
    } /* end while */
  buf[index] = '\0';  /* terminate NTBS */
  if (feof(stdin)) {
    /* handle EOF */
  }
  if (ferror(stdin)) {
    /* handle error */
  }
  if (chars_read > index) {
    /* handle truncation */
  }

  printf("%s", buf);
}

void STR36(void) {
  const char s[] = "abc";
  char s2[3] = { 'a', 'b', 'c' }; /* NOT null-terminated */
  printf("%s", s);
  printf("%s", s2);
}

size_t STR37_count_preceding_whitespace(const char *s) {
  const char *t = s;
  size_t length = strlen(s) + 1;

  while (isspace((unsigned char)*t) && (t - s < length)) {
    ++t;
  }
  return t - s;
}

void STR37(void) {
  (void)STR37_count_preceding_whitespace("   sdjfkasjfe");
}

void STR38(void) {
  char input[] = "bogus@addr.com; cat /etc/passwd";
  string email;
  string::iterator loc = email.begin();

  // copy into string converting ";" to " "
  for (size_t i=0; i <= strlen(input); i++) {
    if (input[i] != ';') {
      loc = email.insert(loc, input[i]);
    }
    else {
      loc = email.insert(loc, ' ');
    }
    ++loc;
  } // end string for each element in NTBS
}

void STR39(void) {
  string bs("01234567");
  try {
    size_t i = 12;
    bs.at(i) = '\0';
  }
  catch (...) {
    cerr << "Index out of range" << endl;
  }
}
