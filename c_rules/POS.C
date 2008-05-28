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

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"
#include <assert.h>

// Note:  As written, these tests catch template declarations only if instantiated.

bool POS33_C( const SgNode *node ) { // Do not use vfork (was overload operator&&)
  if (!isCallOfFunctionNamed( node, "vfork")) return false;
  print_error( node, "POS33-C", "Do not use vfork()");
  return true;
}


bool POS34_C( const SgNode *node ) { // Do not call putenv() with auto var
  if (!isCallOfFunctionNamed( node, "putenv")) return false;

  // ok, bail iff putenv's arg is a char* (not char[])
  const SgExpression *arg0 = getFnArg( isSgFunctionRefExp( node), 0);
  assert( arg0 != NULL);
  Type t( arg0->get_type());
  if (t.isPointer()) return false;
  assert( t.isArray());

  // bail iff putenv's arg is a static variable
  const SgVarRefExp* var = isSgVarRefExp( arg0);
  if (var == NULL) return false; // we don't handle non-var putenv args

  const SgInitializedName* decl = var->get_symbol()->get_declaration();
  if (isSgGlobal( decl->get_parent()->get_parent()))
    return false;
  if (decl->get_declaration()->get_declarationModifier().get_storageModifier().isStatic())
    return false;

  print_error( node, "POS34-C", "Do not call putenv() with an automatic variable");
  return true;
}


bool POS(const SgNode *node) {
  bool violation = false;
  violation |= POS33_C(node);
  violation |= POS34_C(node);
  return violation;
}
