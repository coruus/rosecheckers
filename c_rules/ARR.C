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


bool ARR01_A( const SgNode *node ) { // Do not use sizeof() on an incomplete array
  const SgSizeOfOp* sizeOfOp = isSgSizeOfOp( node);
  if (sizeOfOp == NULL) return false;

  const SgExpression* sizeOfExpr = sizeOfOp->get_operand_expr();
  if (sizeOfExpr == NULL) return false; // ignore sizeof( type);

  const Type ref_type(sizeOfExpr->get_type());
  const Type type = ref_type.dereference();
  if (!type.isArray()) return false;
  const SgArrayType* aType = isSgArrayType( type.sgType());
  if (aType == NULL) return false;
  const SgExpression* index = aType->get_index();
  if (index != NULL) return false; // we know the size of the array

  print_error( node, "ARR01-A", "Do not use sizeof() on an incomplete array", true);
  return true;
}

bool ARR02_A( const SgNode *node ) { // Explicitly specify array dimensions, even if implicitly defined by an initializer
	const SgVariableDeclaration *varDecl = isSgVariableDeclaration(node);
	if (!varDecl)
		return false;

	FOREACH_INITNAME(varDecl->get_variables(), i) {
		assert(*i);
		const SgArrayType *varType = isSgArrayType((*i)->get_type());
		if (!varType)
			continue;
		// Ignore chars as per STR36
		if (isAnyCharType(varType->get_base_type()))
			continue;
		if (!(*i)->get_initializer())
			continue;
		if (!varType->get_index()) {
			print_error(*i, "ARR02-A", "Explicitly specify array dimensions, even if implicitly defined by an initializer", true);
		return true;
		}
	}

	return false;
}


bool ARR34_C( const SgNode *node ) { // Ensure that array types in expressions are compatible
	// Since GCC produces a warning during compilation for this, Rose already
	// catches violations of this rule

	return false;
}

bool ARR(const SgNode *node) {
  bool violation = false;
  violation |= ARR01_A(node);
  violation |= ARR02_A(node);
  violation |= ARR34_C(node);
  return violation;
}
