/**
 * \file STR.C
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
 * Ensure that string storage is sufficient for chars & terminator
 *
 * This just ensures that strcpy is copying into a pointer
 */
bool STR31_C(const SgNode *node ) {
	if (!isCallOfFunctionNamed( node, "strcpy")) return false;

	const SgVarRefExp* ref = isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 0));
	if (ref == NULL) return false; // strcpy() not copying into simple var
	if (!Type( getRefDecl( ref)->get_type()).isArray()) return false;
	if (Type( getFnArg( isSgFunctionRefExp(node), 1)->get_type()).isArray()) return false;

	print_error( node, "STR31-C", "String copy destination must contain sufficient storage");
	return true;
}

/**
 * Null-terminate byte strings as required
 *
 * \note This only works on character arrays that don't get initialized
 */
bool STR32_C(const SgNode *node ) {
	if (!isCallOfFunctionNamed(node, "strncpy")) return false;
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	assert(fnRef);
	const SgExpression *dstExp = removeImplicitPromotions(getFnArg(fnRef,0));
	const SgExpression *srcExp = removeImplicitPromotions(getFnArg(fnRef,1));
	const SgExpression *lenExp = getFnArg(fnRef,2);
	assert(dstExp && srcExp && lenExp);

	const SgVarRefExp *srcRef = isSgVarRefExp(srcExp);
	if(!srcRef) {
		const SgCastExp *srcCast = isSgCastExp(srcExp);
		if(!srcCast)
			return false;
		srcRef = isSgVarRefExp(srcCast->get_operand());
		if(!srcRef)
			return false;
	}

	const SgArrayType *arrT = isSgArrayType(srcExp->get_type());
	const SgVarRefExp *dstRef = isSgVarRefExp(dstExp);
	if (!arrT || !dstRef)
		return false;
	size_t len;
	/* Some machines evaluate to unsigned int, others to unsigned long */
	if (!getSizetVal(lenExp,&len))
		return false;
	const SgValueExp *srcVal = isSgValueExp(arrT->get_index());
	if (!srcVal) // VLA or some such...
		return false;
	const SgUnsignedLongVal *srcValInt = isSgUnsignedLongVal(srcVal);
	if (!srcValInt)
		return  false;
	size_t src_size = srcValInt->get_value();

	if (src_size >= len) {
		do {
			// check for null termination, violation if not present

			// first, find the parent block and the next expression after the
			// strncpy()
			const SgStatement * nextStat = findInBlockByOffset(node, 1);
			// if all went well, it should be an expression
			if(!nextStat)
				break;
			const SgExprStatement *nextExpr = isSgExprStatement(nextStat);
			if(!nextExpr)
				break;
			// To comply with the rule, it must be an assignment...
			const SgAssignOp *assignOp = isSgAssignOp(nextExpr->get_expression());
			if (!assignOp)
				break;
			// ... to an array ...
			const SgPntrArrRefExp *arrRef = isSgPntrArrRefExp(assignOp->get_lhs_operand());
			if (!arrRef)
				break;
			// ... that is the same as dstRef ...
			const SgVarRefExp *varRef = isSgVarRefExp(arrRef->get_lhs_operand());
			assert(varRef);
			if (getRefDecl(varRef) != getRefDecl(dstRef))
				break;
			// ... and must have an index ...
			size_t dst_idx;
			if (!getSizetVal(arrRef->get_rhs_operand(), &dst_idx))
				break;
			// ... that is equal to len - 1
			if (len > 0 && dst_idx == len - 1) {
				return false;
			}
		} while(0);
		print_error(node, "STR32-C", "Null-terminate byte strings as required");
		return true;
	} else {
		return false;
	}
}

/**
 * Check if there is a gets or if sscanf/scanf has a "%s"
 */
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

/**
 * Do not specify the dimension of a character array initialized with a string
 * literal
 */
bool STR36_C(const SgNode *node) {
	const SgVariableDeclaration *varDecl = isSgVariableDeclaration(node);
	if (!varDecl)
		return false;

	FOREACH_INITNAME(varDecl->get_variables(), i) {
		assert(*i);
		const SgArrayType *varType = isSgArrayType((*i)->get_type());
		if (!varType)
			continue;
		if (!isAnyCharType(varType->get_base_type()))
			continue;
		const SgAssignInitializer *varInitializer = isSgAssignInitializer((*i)->get_initializer());
		if (!varInitializer)
			continue;
		if (varType->get_index()) {
			print_error(*i, "STR36-C", "Do not specify the dimension of a character array initialized with a string literal", true);
		return true;
		}
	}

	return false;
}

bool STR(const SgNode *node) {
  bool violation = false;
  violation |= STR31_C(node);
  violation |= STR32_C(node);
  violation |= STR35_C(node);
  violation |= STR36_C(node);
  return violation;
}
