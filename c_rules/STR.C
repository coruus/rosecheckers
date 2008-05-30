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
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WSTRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "rose.h"
#include "utilities.h"


bool STR31_C(const SgNode *node ) { // Ensure that string storage is sufficient for chars & terminator
  // This just ensures that strcpy is copying into a pointer
  if (!isCallOfFunctionNamed( node, "strcpy")) return false;

  const SgVarRefExp* ref = isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 0));
  if (ref == NULL) return false; // strcpy() not copying into simple var
  if (!Type( getRefDecl( ref)->get_type()).isArray()) return false;
  if (Type( getFnArg( isSgFunctionRefExp(node), 1)->get_type()).isArray()) return false;

  print_error( node, "STR31-C", "String copy destination must contain sufficient storage");
  return true;
}

//Check if there is a gets
//Check if sscanf/scanf has a "%s"

bool STR35_C(const SgNode *node) {

	if(isCallOfFunctionNamed(node, "gets")) {
		print_error(node, "STR35-C", "Do not copy data from an unbounded source to a fixed-length array");
		return true;
	}

	if(isCallOfFunctionNamed(node, "scanf")) {
		SgFunctionCallExp const *fn = isSgFunctionCallExp(node->get_parent());
		assert(fn);

		SgExprListExp const *expr = fn->get_args();
		assert(expr);

		SgExpressionPtrList const &list = expr->get_expressions();
		SgExpressionPtrList::const_iterator i = list.begin();
		//How do I asserted iterator?

		SgCastExp const *cast = isSgCastExp(*i);
		assert(cast);
		
		std::vector< SgNode * > t = const_cast<SgCastExp *>(cast)->get_traversalSuccessorContainer ();
		SgStringVal const *val = isSgStringVal(t[0]);
		assert(val);

		std::string s = val->get_value();

		if(strstr(s.c_str(), "%s") != NULL) {
			print_error(node, "STR35-C", "Do not copy data from an unbounded source to a fixed-length array");
			return true;
		}
	}

	return false;
}

bool STR(const SgNode *node) {
  bool violation = false;
  violation |= STR31_C(node);
  violation |= STR35_C(node);
  return violation;
}
