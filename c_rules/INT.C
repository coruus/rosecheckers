/**
 * \file INT.C
 *
 * \note As written, these tests catch template declarations only if
 * instantiated.
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

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"

/**
 * Remove all modifiers such as const or volatile, but leave the typedefs
 */
const SgType *stripModifiers(const SgType *type) {
	const SgModifierType *mt;
	while(mt = isSgModifierType(type)) {
		type = mt->get_base_type();
	}
	return type;
}

/**
 * Use rsize_t or size_t for all integer values representing the size of an
 * object
 */
bool INT01_A( const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if(!op)
		return false;
	/* Ignore array derefences */
	if(isSgPntrArrRefExp(op))
		return false;

	const SgType *lhsType = stripModifiers(op->get_lhs_operand()->get_type());
	const SgType *rhsType = stripModifiers(op->get_rhs_operand()->get_type());

	/**
	 * scalars are ok
	 */
	if(isSgValueExp(op->get_lhs_operand())
	|| isSgValueExp(op->get_rhs_operand()))
		return false;

	const bool lhsIsSize = isSgTypedefType(lhsType) && (lhsType->unparseToString() == "size_t");
	const bool lhsIsRSize = isSgTypedefType(lhsType) && (lhsType->unparseToString() == "rsize_t");
	const bool rhsIsSize = isSgTypedefType(rhsType) && (rhsType->unparseToString() == "size_t");
	const bool rhsIsRSize = isSgTypedefType(rhsType) && (rhsType->unparseToString() == "rsize_t");

	if((lhsIsSize ^ rhsIsSize) || (lhsIsRSize ^ rhsIsRSize)) {
		print_error(node,"INT01-A", "Use rsize_t or size_t for all integer values representing the size of an object", true);
		return true;
	}

	return false;
}

bool INT(const SgNode *node) {
  bool violation = false;
  violation |= INT01_A(node);
  return violation;
}
