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

#include "PRE_NCCE.C"
#include "DCL_NCCE.C"
#include "EXP_NCCE.C"
#include "INT_NCCE.C"
#include "FLP_NCCE.C"
#include "ARR_NCCE.C"
#include "STR_NCCE.C"
#include "MEM_NCCE.C"
#include "ENV_NCCE.C"
#include "SIG_NCCE.C"
#include "ERR_NCCE.C"
#include "OBJ_NCCE.C"
#include "MSC_NCCE.C"

int main(int argc, char const *argv[], char const *envp[]) {
  PRE();
  DCL();
  EXP();
  INT();
  FLP();
  ARR();
  STR();
  MEM();
  ENV(envp);
  SIG();
  ERR();
  OBJ();
  MSC();

  return 0;
}
