/*
 * Copyright (c) 2007-2012 Carnegie Mellon University.
 * 
 * All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following acknowledgments
 * and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 * 
 * 3. The names “Carnegie Mellon University,” "CERT” and/or “Software
 * Engineering Institute" shall not be used to endorse or promote
 * products derived from this software without prior written
 * permission. For written permission, please contact
 * permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "CERT" nor
 * may "CERT" appear in their names without prior written permission
 * of permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 * acknowledgment:
 * 
 * "This product includes software developed by CERT with funding and
 * support from the Department of Defense under Contract No. FA
 * 8721-05-C-0003.  The U.S. Government's rights to use, modify,
 * reproduce, release, perform, display, or disclose this material are
 * restricted by the Rights in Technical Data-Noncommercial Items
 * clauses (DFAR 252-227.7013 and DFAR 252-227.7013 Alternate I
 * contained in the foregoing identified contract.
 * 
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY “AS IS” AND
 * CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AS TO ANY MATTER, AND ALL SUCH WARRANTIES,
 * INCLUDING WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE, ARE EXPRESSLY DISCLAIMED.  WITHOUT LIMITING THE
 * GENERALITY OF THE FOREGOING, CARNEGIE MELLON UNIVERSITY DOES NOT
 * MAKE ANY WARRANTY OF ANY KIND RELATING TO EXCLUSIVITY,
 * INFORMATIONAL CONTENT, ERROR-FREE OPERATION, RESULTS TO BE OBTAINED
 * FROM USE, FREEDOM FROM PATENT, TRADEMARK AND COPYRIGHT INFRINGEMENT
 *  AND/OR FREEDOM FROM THEFT OF TRADE SECRETS.”
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
bool MSC01_C(const SgNode *node) {
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
bool MSC03_C(const SgNode *node) {
	/* This rule has been deprecated in favor of the following rules
	 * EXP15-C. Do not place a semicolon on the same line as an if, for, or while statement
   * MSC12-C. Detect and remove code that has no effect
	 * 
	 * Currently, only MSC12-C is actually enabled by rosecheckers.
	 */
	return false;
}

/**
 * Do not manipulate time_t typed values directly
 */
bool MSC05_C(const SgNode *node) {
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
	} else if (isSgBitComplementOp(node)
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
 * Detect and remove code that has no effect
 */
bool MSC12_C(const SgNode *node) {
	const SgExprStatement *exprStmt = isSgExprStatement(node);
	if (exprStmt) {
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

		if (isSgFunctionCallExp(expr)
        || isSgAssignOp(expr)
        || isSgConditionalExp(expr)
        || isAnyAssignOp(expr)
        || isSgPointerDerefExp(expr)
        || isSgPlusPlusOp(expr)
        || isSgMinusMinusOp(expr)
        || isSgDeleteExp(expr))
			return false;
	} else {
		return false;
	}

	print_error(node, "MSC12-C", "Detect and remove code that has no effect", true);
	return true;
}

/**
 * Detect and remove unused values
 *
 * \bug Disabled until a better algorithm can be found
 */
bool MSC13_C(const SgNode *node) {
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
bool MSC30_C(const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "rand")))
		return false;
	print_error(node, "MSC30-C", "Do not use the rand() function for generating pseudorandom numbers");
	return true;
}

bool MSC_C(const SgNode *node) {
  bool violation = false;
  violation |= MSC01_C(node);
  violation |= MSC05_C(node);
  violation |= MSC12_C(node);
  //  violation |= MSC13_C(node);
  violation |= MSC30_C(node);
  return violation;
}


/// C++ checkers

bool MSC_CPP(const SgNode *node) {
  bool violation = false;
  violation |= MSC_C(node);
  return violation;
}

