/**
 * \file MSC.C
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
 * Avoid errors of addition 
 *
 * \note Because of a problem with the expansion of isnan/isless/etc, this
 * rule is disabled, ROSE catches most of this on it's own, so this should not
 * be a problem
 */
//bool MSC03_A( const SgNode *node ) {
//	const SgExprStatement *exprStmt = isSgExprStatement(node);
//	if (!exprStmt)
//		return false;
//
//	const SgBasicBlock *parent = isSgBasicBlock(exprStmt->get_parent());
//	if (!parent)
//		return false;
//
//	const SgExpression *expr = exprStmt->get_expression();
//	assert(expr);
//
//	if (isCompilerGeneratedNode(expr))
//		return false;
//
//	if(isSgFunctionCallExp(expr)
//	|| isSgAssignOp(expr)
//	|| isSgConditionalExp(expr)
//	|| isSgAndAssignOp(expr)
//	|| isSgDivAssignOp(expr)
//	|| isSgIorAssignOp(expr)
//	|| isSgXorAssignOp(expr)
//	|| isSgLshiftAssignOp(expr)
//	|| isSgRshiftAssignOp(expr)
//	|| isSgMinusAssignOp(expr)
//	|| isSgModAssignOp(expr)
//	|| isSgMultAssignOp(expr)
//	|| isSgPointerDerefExp(expr)
//	|| isSgPlusPlusOp(expr)
//	|| isSgMinusMinusOp(expr)
//	|| isSgPlusAssignOp(expr))
//		return false;
//
//	std::cerr << expr->unparseToString() << std::endl;
//
//	print_error(node, "MSC03-A", "Avoid errors of addition", true);
//	return true;
//}

/**
 * Do not use rand()
 */
bool MSC30_C( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "rand")) return false;
  print_error( node, "MSC30-C", "Do not use rand()");
  return true;
}

bool MSC(const SgNode *node) {
  bool violation = false;
//  violation |= MSC03_A(node);
  violation |= MSC30_C(node);
  return violation;
}
