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
 * Beware of multiple environment variables with the same effective name
 */
bool ENV02_C( const SgNode *node ) {
	static std::set<std::string> origStrs;
	static std::set<std::string> normStrs;
	static std::map<std::string, const SgFunctionRefExp *> strToNode;

	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;

	if (!(isCallOfFunctionNamed(fnRef, "getenv")
		||isCallOfFunctionNamed(fnRef, "setenv")
		||isCallOfFunctionNamed(fnRef, "putenv"))) {
		return false;
	}
	const SgStringVal *strVal = isSgStringVal(removeImplicitPromotions(getFnArg(fnRef,0)));
	if (!strVal)
		return false;
	std::string str = strVal->get_value();
	if (const unsigned int eq = str.find_first_of("="))
		str = str.substr(0,eq);

	if (origStrs.find(str) != origStrs.end())
		return false;
	origStrs.insert(str);

	std::transform(str.begin(), str.end(), str.begin(), tolower);

	if (normStrs.find(str) != normStrs.end()) {
		print_error(node, "ENV02-C", "Beware of multiple environment variables with the same effective name", true);
		print_error(strToNode[str], "ENV02-C", "Previous reference was here", true);
		return true;
	}

	normStrs.insert(str);
	strToNode[str] = fnRef;
	return false;
}

/**
 * Do not use system()
 *
 * \note As written, these tests catch template declarations only if instantiated.
 */
bool ENV04_C( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "system") &&
      !isCallOfFunctionNamed( node, "popen")) return false;
  print_error( node, "ENV04-C", "Do not use system() or popen() unless you need a command interpreter", true);
  return true;
}

/**
 * Do not rely on an environment pointer following an operation that may
 * invalidate it 
 */
bool ENV31_C( const SgNode *node ) {
	const SgVarRefExp *varRef = isSgVarRefExp(node);
	if (!varRef)
		return false;
	if (getRefDecl(varRef)->get_name().getString() != "envp")
		return false;
	bool violation = false;
	FOREACH_SUBNODE(getRefDecl(varRef)->get_scope(), nodes, i, V_SgExpression) {
		if (varRef == isSgVarRefExp(*i))
			break;
		const SgFunctionRefExp *iFn = isSgFunctionRefExp(*i);
		if (!iFn)
			continue;
		if (isCallOfFunctionNamed(iFn, "putenv")
		  ||isCallOfFunctionNamed(iFn, "setenv")) {
			violation = true;
			break;
		}
	}
	if (violation) {
		print_error(node, "ENV31-C", "Do not rely on an environment pointer following an operation that may invalidate it");
	}
	return violation;
}

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

	bool violation = false;

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
			print_error( fnDecl, "ENV32-C", "No atexit handler should terminate in any way other than by returning");
			violation = true;
		}
	}

	return violation;
}

bool ENV(const SgNode *node) {
	bool violation = false;
	violation |= ENV02_C(node);
	violation |= ENV04_C(node);
	violation |= ENV31_C(node);
	violation |= ENV32_C(node);
	return violation;
}
