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

void SIG00();
void SIG01();
void SIG30();

void SIG() {
  SIG00();
  SIG01();
  SIG30();
}

volatile sig_atomic_t SIG00_sig1 = 0;
volatile sig_atomic_t SIG00_sig2 = 0;

void SIG00_handler(int signum) {
  if (signum == SIGUSR1) {
    SIG00_sig1 = 1;
  }
  else if (SIG00_sig1) {
    SIG00_sig2 = 1;
  }
}

void SIG00(void) {
  struct sigaction act;
  act.sa_handler = &SIG00_handler;
  act.sa_flags = 0;
  if (sigemptyset(&act.sa_mask) != 0) {
    return;
  }
  if (sigaddset(&act.sa_mask, SIGUSR1)) {
    return;
  }
  if (sigaddset(&act.sa_mask, SIGUSR2)) {
    return;
  }

  if (sigaction(SIGUSR1, &act, NULL) != 0) {
    return;
  }
  if (sigaction(SIGUSR2, &act, NULL) != 0) {
    return;
  }

  while (SIG00_sig2 == 0) {
    /* Do nothing or give up CPU for a while */
  }
}

void SIG01_handler(int signum) {
#ifndef WINDOWS
  if (signal(signum, SIG_DFL) == SIG_ERR) {
    /* Handler error */
  }
#endif
  /* Handle signal */
}

void SIG01(void) {
  struct sigaction act;
  act.sa_handler = &SIG01_handler;
  act.sa_flags = SA_RESETHAND;
  if (sigemptyset(&act.sa_mask) != 0) {
    return;
  }
  if (sigaction(SIGUSR1, &act, NULL) != 0) {
    return;
  }
}

volatile sig_atomic_t SIG30_eflag = 0;
char *SIG30_info = NULL;

void SIG30_log_message(void) {
  fprintf(stderr, SIG30_info);
}

void SIG30_handler(int signum) {
  SIG30_eflag = 1;
}

/* This is the same cce code for SIG31, SIG33, and SIG34 */
void SIG30(void) {
  if (signal(SIGINT, SIG30_handler) == SIG_ERR) {
    return;
  }
  SIG30_info = (char*)malloc(12);
  if (SIG30_info == NULL) {
    return;
  }

  while (!SIG30_eflag) {
    /* Main loop program code */

    SIG30_log_message();

    /* More program code */
  }

  SIG30_log_message();
  free(SIG30_info);
  SIG30_info = NULL;
}

