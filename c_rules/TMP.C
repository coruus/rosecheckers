/*
 *
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

#include "rose.h"
#include "utilities.h"


bool TMP32_C( const SgNode *node ) { // Do not use tmpfile()
  if (!isCallOfFunctionNamed( node, "tmpfile")) return false;
  print_error( node, "TMP32-C", "Do not use tmpfile()");
  return true;
}


bool TMP32_C_2( const SgNode *node ) { // Do not use fopen() on the results of tmpnam()
  if (!isCallOfFunctionNamed( node, "tmpnam")) return false;

  if (!isVarUsedByFunction("fopen", isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 0))))
    return false;

  print_error( node, "TMP32-C", "Do not use fopen() on the results of tmpnam()");
  return true;
}

bool TMP32_C_3( const SgNode *node ) { // Do not use open() on the results of mktemp()
  if (!isCallOfFunctionNamed( node, "mktemp")) return false;

  if (!isVarUsedByFunction("open", isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 0))))
    return false;

  print_error( node, "TMP32-C", "Do not use open() on the results of mktemp()");
  return true;
}

bool TMP32_C_4( const SgNode *node ) { // Do not use fopen_s() on the results of tmpname_s()
  if (!isCallOfFunctionNamed( node, "tmpname_s")) return false;

  if (!isVarUsedByFunction("fopen_s", isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 1))))
    return false;

  print_error( node, "TMP32-C", "Do not use fopen_s() on the results of tmpname_s()");
  return true;
}


bool TMP(const SgNode *node) {
  bool violation = false;
  violation |= TMP32_C(node);
  violation |= TMP32_C_2(node);
  violation |= TMP32_C_3(node);
  violation |= TMP32_C_4(node);
  return violation;
}
