/**
 * \file ENV.C
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

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


#include "rose.h"
#include "utilities.h"

/**
 * Do not use system()
 *
 * \note As written, these tests catch template declarations only if instantiated.
 */
bool ENV04_A( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "system") &&
      !isCallOfFunctionNamed( node, "popen")) return false;
  print_error( node, "ENV04-A", "Do not use system() or popen() unless you need a command interpreter", true);
  return true;
}

#define FOREACH_SUBNODE(node, nodes, i, type) \
	Rose_STL_Container<SgNode *> (nodes) = NodeQuery::querySubTree( const_cast<SgExpression*>(node), (type) ); \
	for (Rose_STL_Container<SgNode *>::iterator (i) = (nodes).begin(); (i) != (nodes).end(); ++(i) )

/**
 * No atexit handler should terminate in any way other than by returning
 * \note This catches calls to exit, _exit, _abort, _Exit, longjmp and
 * siglongjmp
 */
bool ENV32_C( const SgNode *node ) {
	if (!isCallOfFunctionNamed( node, "atexit")) {
		return false;
	}
	const SgFunctionRefExp* ref = isSgFunctionRefExp( getFnArg ( isSgFunctionRefExp(node), 0));
	assert(ref);

	const SgFunctionDeclaration *fnDecl = ref->get_symbol()->get_declaration();
	assert(fnDecl);

	const SgExpression *fnCall;
	FOREACH_SUBNODE((SgExpression *) fnDecl,nodes,i,V_SgFunctionCallExp) {
		assert(*i);
		fnCall = isSgFunctionCallExp(*i)->get_function();
		if(isCallOfFunctionNamed( fnCall, "exit")
		|| isCallOfFunctionNamed( fnCall, "_exit")
		|| isCallOfFunctionNamed( fnCall, "abort")
		|| isCallOfFunctionNamed( fnCall, "_Exit")
		|| isCallOfFunctionNamed( fnCall, "longjmp")
		|| isCallOfFunctionNamed( fnCall, "siglongjmp")) {
			print_error( fnDecl, "ENV32-C", "No atexit handler should terminate in any way other than by returning", true);
			return true;
		}
	}

	return false;
}

bool ENV(const SgNode *node) {
	bool violation = false;
	violation |= ENV04_A(node);
	violation |= ENV32_C(node);
	return violation;
}
