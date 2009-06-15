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


int POS34_1(char const *var) {
  static char *oldenv;
  char const *env_format = "TEST=%s";
  size_t const len = strlen(var) + strlen(env_format);
  char *environment = (char *) calloc( len, 256);
  if (environment == NULL) {
    return -1;
  }
  const int rc = snprintf(environment, len, env_format, var);
  if (rc < 0 || (size_t)rc >= len) {
    /* Handle Error */
  }
  if (putenv(environment) != 0) {
    free(environment);
    return -1;
  }
  if (oldenv != NULL)
    free(oldenv); // avoid memory leak
  oldenv = environment;
  return 0;
}


char goodenv[1024];

int POS34_2(const char *var) {
  char *environment;
  environment = goodenv;

  if (snprintf(environment, sizeof(environment),"TEST=%s", var) < 0) {
    /* Handle error */
  }

  return putenv(environment);
}


char env0[1024];
int POS34_3(const char *var) {

  if (snprintf(env0, sizeof(env0),"TEST=%s", var) < 0) {
    /* Handle error */
  }

  return putenv(env0);
}

const char *spongeBob = "bob the sponge, here again!!!";
int POS34_4(){
  return putenv((char*) spongeBob); // const cast necessary for putenv
}


int POS34_5(const char *var) {
  static char environment[1024];

  if (snprintf(environment, sizeof(environment),"TEST=%s", var) < 0) {
    /* Handle error */
  }

  return putenv(environment);
}

int POS34_6(){
  char *env_1 = (char*)(malloc(10));
  if (env_1 ==  NULL) {/* handle error */}
  char *env_2 = env_1;
  return putenv(env_2);
}


