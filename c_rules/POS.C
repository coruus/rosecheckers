/**
 * \file POS.C
 *
 * \note As written, these tests catch template declarations only if
 * instantiated.
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.
 * 
 * Permission to use this software and its documentation for any purpose is
 * hereby granted, provided that the above copyright notice appear and that
 * both that copyright notice and this permission notice appear in supporting
 * documentation, and that the name of CMU not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WSTRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR
 * IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"
#include <assert.h>

/**
 * Do not use vfork (was overload operator&&)
 */
bool POS33_C( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "vfork")) return false;
  print_error( node, "POS33-C", "Do not use vfork()");
  return true;
}

/**
 * Do not call putenv() with auto var
 */
bool POS34_C( const SgNode *node ) {
	if (!isCallOfFunctionNamed( node, "putenv")) return false;

	// ok, bail iff putenv's arg is a char* (not char[])
	const SgExpression *arg0 = getFnArg( isSgFunctionRefExp( node), 0);
	assert( arg0 != NULL);
	Type t( arg0->get_type());
	if (t.isPointer())
		return false;
	/**
	 * \todo We only know how to deal with arrays for now
	 */
	if (!t.isArray())
		return false;

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

/**
 * Observe correct revocation order while relinquishing privileges
 *
 * \note Since there's really no clean way to do this, we'll just traverse up
 * to the first SgBasicBlock above a setgid(), then find the previous
 * statement, and see if it contains a call to setuid()
 */
bool POS36_C( const SgNode *node ) {
	if (!isCallOfFunctionNamed(node, "setgid"))
		return false;

	const SgStatement *prevStat = findInBlockByOffset(node, -1);
	if (!prevStat)
		return false;

	FOREACH_SUBNODE(prevStat, nodes, i, V_SgFunctionRefExp) {
		assert(*i);
		if (isCallOfFunctionNamed(*i, "setuid")) {
			print_error(node, "POS36_C", "Observe correct revocation order while relinquishing privileges", true);
			return true;
		}
	}
	return false;
}

bool POS(const SgNode *node) {
  bool violation = false;
  violation |= POS33_C(node);
  violation |= POS34_C(node);
  violation |= POS36_C(node);
  return violation;
}
