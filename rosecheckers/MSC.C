/*
** Copyright (C) 2007-2009 by Carnegie Mellon University.
**
** @OPENSOURCE_HEADER_START@
**
** Use of the ROSE Checkers system and related source code is subject to
** the terms of the following license:
**
** GNU Public License (GPL) Rights pursuant to Version 2, June 1991
**
** NO WARRANTY
**
** ANY INFORMATION, MATERIALS, SERVICES, INTELLECTUAL PROPERTY OR OTHER
** PROPERTY OR RIGHTS GRANTED OR PROVIDED BY CARNEGIE MELLON UNIVERSITY
** PURSUANT TO THIS LICENSE (HEREINAFTER THE "DELIVERABLES") ARE ON AN
** "AS-IS" BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY
** KIND, EITHER EXPRESS OR IMPLIED AS TO ANY MATTER INCLUDING, BUT NOT
** LIMITED TO, WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE,
** MERCHANTABILITY, INFORMATIONAL CONTENT, NONINFRINGEMENT, OR ERROR-FREE
** OPERATION. CARNEGIE MELLON UNIVERSITY SHALL NOT BE LIABLE FOR INDIRECT,
** SPECIAL OR CONSEQUENTIAL DAMAGES, SUCH AS LOSS OF PROFITS OR INABILITY
** TO USE SAID INTELLECTUAL PROPERTY, UNDER THIS LICENSE, REGARDLESS OF
** WHETHER SUCH PARTY WAS AWARE OF THE POSSIBILITY OF SUCH DAMAGES.
** LICENSEE AGREES THAT IT WILL NOT MAKE ANY WARRANTY ON BEHALF OF
** CARNEGIE MELLON UNIVERSITY, EXPRESS OR IMPLIED, TO ANY PERSON
** CONCERNING THE APPLICATION OF OR THE RESULTS TO BE OBTAINED WITH THE
** DELIVERABLES UNDER THIS LICENSE.
**
** Licensee hereby agrees to defend, indemnify, and hold harmless Carnegie
** Mellon University, its trustees, officers, employees, and agents from
** all claims or demands made against them (and any related losses,
** expenses, or attorney's fees) arising out of, or relating to Licensee's
** and/or its sub licensees' negligent use or willful misuse of or
** negligent conduct or willful misconduct regarding the Software,
** facilities, or other rights or assistance granted by Carnegie Mellon
** University under this License, including, but not limited to, any
** claims of product liability, personal injury, death, damage to
** property, or violation of any laws or regulations.
**
** @OPENSOURCE_HEADER_END@
*/

/**
 * \file MSC.C
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"

/**
 * Strive for logical completeness
 */
bool MSC01_C( const SgNode *node ) {
	const SgSwitchStatement *swch = isSgSwitchStatement(node);
	if (!swch)
		return false;

	const SgStatementPtrList &stats = swch->getStatementList();
	if ((stats.size() > 0)
	&& isSgDefaultOptionStmt(stats.back()))
		return false;

	print_error(node, "MSC01-C", "Strive for logical completeness", true);
	return true;
}

/**
 * Avoid errors of addition
 *
 * \note Because of a problem with the expansion of isnan/isless/etc, this
 * rule is disabled, ROSE catches most of this on it's own, so this should not
 * be a problem
 */
bool MSC03_C( const SgNode *node ) {
	const SgExprStatement *exprStmt = isSgExprStatement(node);
	if (!exprStmt) {
		/**
		 * Check for empty bodies
		 */
		const SgBasicBlock *block = NULL;
		if (isSgIfStmt(node))
			block = isSgBasicBlock(isSgIfStmt(node)->get_true_body());
		else if (isSgForStatement(node))
			block = isSgBasicBlock(isSgForStatement(node)->get_loop_body());
		else if (isSgWhileStmt(node))
			block = isSgBasicBlock(isSgWhileStmt(node)->get_body());
		else
			return false;

		if (!block
		|| !isCompilerGeneratedNode(block))
			return false;
	} else {
		/**
		 * Check for statements with no effect
		 */
		const SgBasicBlock *parent = isSgBasicBlock(exprStmt->get_parent());
		if (!parent)
			return false;

		/**
		 * Ignore the last statement in a block because it could be an implicit
		 * return value, this is GNU extension
		 */
		if ((exprStmt == isSgExprStatement(parent->get_statements().back()))
		&&  (!isSgFunctionDefinition(parent->get_parent())))
			return false;

		const SgExpression *expr = exprStmt->get_expression();
		assert(expr);

		if (isCompilerGeneratedNode(expr))
			return false;

		if(isSgFunctionCallExp(expr)
		|| isSgAssignOp(expr)
		|| isSgConditionalExp(expr)
		|| isAnyAssignOp(expr)
		|| isSgPointerDerefExp(expr)
		|| isSgPlusPlusOp(expr)
		|| isSgMinusMinusOp(expr)
		|| isSgDeleteExp(expr))
			return false;
	}

	print_error(node, "MSC03-C", "Avoid errors of addition", true);
	return true;
}

/**
 * Do not manipulate time_t typed values directly
 */
bool MSC05_C( const SgNode *node ) {
	if (isAnyBinArithOp(node)
	  ||isAnyBinArithAssignOp(node)
	  ||isAnyBinBitOp(node)
	  ||isAnyBinBitAssignOp(node)
	  ||isAnyRelationalOp(node)) {
		const SgBinaryOp *binOp = isSgBinaryOp(node);
		assert(binOp);
		const SgType *lhsT = binOp->get_lhs_operand()->get_type();
		const SgType *rhsT = binOp->get_rhs_operand()->get_type();
		if (!(isTypeTimeT(lhsT) || isTypeTimeT(rhsT)))
			return false;
	} else if(isSgBitComplementOp(node)
	  ||isSgMinusMinusOp(node)
	  ||isSgNotOp(node)
	  ||isSgPlusPlusOp(node)
	  ||isSgUnaryAddOp(node)
	  ||isSgMinusOp(node)) {
		const SgUnaryOp *op = isSgUnaryOp(node);
		assert(op);
		const SgType *opT = op->get_operand()->get_type();
		if (!isTypeTimeT(opT))
			return false;
	} else {
		return false;
	}

	print_error(node, "MSC05-C", "Do not manipulate time_t typed values directly", true);
	return true;
}

/**
 * Detect and remove unused values
 *
 * \bug Disabled until a better algorithm can be found
 */
bool MSC13_C( const SgNode *node ) {
	const SgInitializedName *var = isSgInitializedName(node);
	if (!var)
		return false;
	bool unused = false;
	bool violation = false;
	if (var->get_initializer())
		unused = true;
	const SgNode * prev = var;
	FOREACH_SUBNODE(var->get_scope(), nodes, i, V_SgVarRefExp) {
		const SgVarRefExp *iVar = isSgVarRefExp(*i);
		if (!iVar || (getRefDecl(iVar) != var))
			continue;
		if (varWrittenTo(iVar) && (!findParentOfType(iVar, SgPointerDerefExp))) {
			if (unused) {
				print_error(prev, "MSC13-C", "Detect and remove unused values", true);
				violation = true;
			} else {
				unused = true;
				prev = iVar;
			}
		} else {
			unused = false;
		}
	}
	return violation;
}

/**
 * Do not use rand()
 */
bool MSC30_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "rand")))
		return false;
	print_error( node, "MSC30-C", "Do not use rand()");
	return true;
}

/**
 * Ensure that return values are compared against the proper type
 */
bool MSC31_C( const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if (!op || isCompilerGeneratedNode(node))
		return false;
	if (!(isSgEqualityOp(op) || isSgNotEqualOp(op)))
		return false;
	const SgExpression *lhs = op->get_lhs_operand();
	const SgValueExp *val = NULL;
	intmax_t n;
	while((val = isSgValueExp(lhs)) != NULL) {
		if (!getIntegerVal(val, &n) || (n >= 0))
			return false;
		if (val->get_originalExpressionTree())
			lhs = removeImplicitPromotions(val->get_originalExpressionTree());
		else
			break;
	}
	const SgExpression *rhs = op->get_rhs_operand();
	while((val = isSgValueExp(rhs)) != NULL) {
		if (!getIntegerVal(val, &n) || (n >= 0))
			return false;
		if (val->get_originalExpressionTree())
			rhs = removeImplicitPromotions(val->get_originalExpressionTree());
		else
			break;
	}
	assert(lhs && rhs);
	const SgType *lhsType = stripModifiers(lhs->get_type());
	const SgType *rhsType = stripModifiers(rhs->get_type());
	assert(lhsType && rhsType);
	/**
	 * \todo We should not be using unparseToString, there should be a better
	 * way to get the name of a typedef type
	 */
	std::string lhsName = lhsType->unparseToString();
	std::string rhsName = rhsType->unparseToString();
	if ((lhsName == "time_t" || lhsName == "size_t")
	 && (lhsName != rhsName)) {
		print_error(node, "MSC31-C", "Ensure that return values are compared against the proper type");
		return true;
	}
	return false;
}

bool MSC_C(const SgNode *node) {
  bool violation = false;
  violation |= MSC01_C(node);
  violation |= MSC03_C(node);
  violation |= MSC05_C(node);
//  violation |= MSC13_C(node);
  violation |= MSC30_C(node);
  violation |= MSC31_C(node);
  return violation;
}


/// C++ checkers

bool MSC_CPP(const SgNode *node) {
  bool violation = false;
  return violation;
}

