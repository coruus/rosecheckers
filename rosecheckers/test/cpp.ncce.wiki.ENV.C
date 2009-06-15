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
#include <pwd.h>

void ENV00();
void ENV01();
void ENV02();
void ENV03();
void ENV04();
void ENV30();
void ENV31(char const *envp[]);
void ENV32();

void ENV(char const *envp[]) {
  ENV00();
  ENV01();
  ENV02();
  ENV03();
  ENV04();
  ENV30();
  ENV31(envp);
  ENV32();
}

void ENV00(void) {
  char *tmpvar;
  char *tempvar;

  tmpvar = getenv("TMP");
  if (!tmpvar) return;
  tempvar = getenv("TEMP");
  if (!tempvar) return;
}

void ENV01(void) {
  char copy[16];
  const char *temp = getenv("TEST_ENV");
  if (temp != NULL) {
    strcpy(copy, temp);
  }
}

void ENV02(void) {
  if (putenv("TEST_ENV=foo") != 0) {
    return;
  }
  if (putenv("Test_ENV=bar") != 0) {
    return;
  }

  const char *temp = getenv("TEST_ENV");
}

void ENV03(void) {
  if (system("/bin/ls dir.`date +%Y%m%d`") == -1) {
    return;
  }
}

void ENV04(void) {
  char *input = NULL;
  /* input gets initialized by user */

  char cmdbuf[512];
  int len_wanted = snprintf(
			    cmdbuf, sizeof(cmdbuf), "any_cmd '%s'", input
			    );
  if (len_wanted >= sizeof(cmdbuf)) {
    perror("Input too long");
  }
  else if (len_wanted < 0) {
    perror("Encoding error");
  }
  else if (system(cmdbuf) == -1) {
    perror("Error executing input");
  }
}

void ENV30(void) {
  char *env = getenv("TEST_ENV");
  if (env == NULL) {
    return;
  }
  env[0] = 'a';
}

void ENV31(char const *envp[]) {
  size_t i;
  if (setenv("MY_NEW_VAR", "new_value", 1) != 0) {
    return;
  }
  if (envp != NULL) {
    for (i = 0; envp[i] != NULL; i++) {
      if (puts(envp[i]) == EOF) {
	return;
      }
    }
  }
}

void ENV32_exit1() {
  exit(0);
}

void ENV32(void) {
  if (atexit(ENV32_exit1) != 0) {
    return;
  }
}
