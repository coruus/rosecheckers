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
 * \file ARR.C
 */

#include "rose.h"
#include "utilities.h"

/**
 * Do not apply the sizeof operator to a pointer when taking the size of an
 * array
 *
 * We find cases of sizeof(var) where var is an array and is also a function
 * parameter
 */
bool ARR01_C( const SgNode *node ) {
	const SgSizeOfOp* sizeOfOp = isSgSizeOfOp( node);
	if(!sizeOfOp)
		return false;

	const SgExpression *expr = removeImplicitPromotions(sizeOfOp->get_operand_expr());
	if(!expr)
		return false;

	if(isSgUnaryOp(expr)) expr = isSgUnaryOp(expr)->get_operand();

	const SgVarRefExp* var = isSgVarRefExp(removeImplicitPromotions(expr));
	if(!var)
		return false;

	const SgArrayType *arrT = isSgArrayType(var->get_type());
	if(!arrT)
		return false;

	const SgFunctionDeclaration *fnRef = findParentOfType( node,SgFunctionDeclaration);
	if (!fnRef)
		return false;

	const SgName &varName = var->get_symbol()->get_name();

	FOREACH_INITNAME(fnRef->get_args(), i) {
		if((*i)->get_name() == varName) {
			print_error( node, "ARR01-C", "Do not apply the sizeof operator to a pointer when taking the size of an array", true);
			return true;
		}
	}

	return false;
}

/**
 * Explicitly specify array dimensions, even if implicitly defined by an
 * initializer
 *
 * \see STR36_C
 */
bool ARR02_C( const SgNode *node ) {
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
			print_error(*i, "ARR02-C", "Explicitly specify array dimensions, even if implicitly defined by an initializer", true);
			return true;
		}
	}

	return false;
}

/**
 * Guarantee that array indices are within the valid range 
 *
 * \note Without tainting, the only thing we can really do is ensure that if a
 * value is already getting checked and is signed, that it's also being
 * checked against 0.
 *
 * \bug This doesn't work with if statement blocks (i.e. with {}) because
 * of the SgBasicBlock that is thrown in.
 */
bool ARR30_C( const SgNode *node ) {
	if (isCompilerGeneratedNode(node))
		return false;

	const SgPntrArrRefExp *deref = isSgPntrArrRefExp(node);
	if (!deref)
		return false;

	const SgVarRefExp *varRef = isSgVarRefExp(removeCasts(deref->get_rhs_operand()));
	if (!varRef)
		return false;
	if (varRef->get_type()->stripTypedefsAndModifiers()->isUnsignedType())
		return false;

	/**
	 * \note Technically we should throw a violation here since there's no
	 * excuse to have signed array indices, but that would cause too many
	 * false positives
	 */

	const SgInitializedName *var = getRefDecl(varRef);
	assert(var);

	const SgStatement *prevSt = findInBlockByOffset(varRef, -1);
	if (!prevSt || isSgForStatement(prevSt))
		return false;

	if (isSgIfStmt(prevSt)) {
		prevSt = isSgIfStmt(prevSt)->get_conditional();
	} else if (isSgWhileStmt(prevSt)) {
		prevSt = isSgWhileStmt(prevSt)->get_condition();
	}

	bool check = false;
	FOREACH_SUBNODE(prevSt, nodes, i, V_SgBinaryOp) {
		const SgBinaryOp *op = isAnyComparisonOp(*i);
		if (!op)
			continue;
		const SgExpression *lhs = removeCasts(op->get_lhs_operand());
		const SgExpression *rhs = removeCasts(op->get_rhs_operand());
		assert(lhs && rhs);
		const SgVarRefExp *lhsVar = isSgVarRefExp(lhs);
		const SgVarRefExp *rhsVar = isSgVarRefExp(rhs);
		if (lhsVar && (getRefDecl(lhsVar) == var)) {
			if (isZeroVal(rhs))
				return false;
			check = true;
		} else if (rhsVar && (getRefDecl(rhsVar) == var)) {
			if (isZeroVal(lhs))
				return false;
			check = true;
		}
	}

	if (check) {
		print_error(node, "ARR30-C", "Guarantee that array indices are within the valid range");
		return true;
	}

	return false;
}

/**
 * Guarantee that copies are made into storage of sufficient size
 *
 * We make sure that the length argument to memcpy is at most the size
 * of memcpy's first argument (destination). This rule fires if:
 * * the destination is a fixed-length array
 * * the last argument is N * sizeof( arraytype)
 * * N is known at compile time
 * * N > destination array index
 */
bool ARR33_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if(!(fnRef && isCallOfFunctionNamed(fnRef, "memcpy")))
		return false;
	const SgExpression *dstExp = removeImplicitPromotions(getFnArg(fnRef,0));
	const SgExpression *lenExp = getFnArg(fnRef,2);
	assert(dstExp && lenExp);

	if(isSgAddressOfOp(dstExp))
		dstExp = isSgAddressOfOp(dstExp)->get_operand();

	const SgArrayType *arrT = isSgArrayType(dstExp->get_type());
	if (!arrT)
		return false;
	size_t len;
	if (!getSizetVal(lenExp,&len))
		return false;
	const SgValueExp *dstVal = isSgValueExp(arrT->get_index());
	if (!dstVal) // VLA or some such...
		return false;
	size_t dst_size;
	if (!getSizetVal(dstVal, &dst_size))
		return false;
	dst_size *= sizeOfType(arrT->findBaseType());
	if (dst_size == 0)
		return false;
	if (dst_size > len) {
		print_error(node, "ARR33-C", "Guarantee that copies are made into storage of sufficient size");
		return true;
	}
	return false;
}

/**
 * Ensure that array types in expressions are compatible
 *
 * \note Since GCC produces a warning during compilation for this, Rose
 * already catches violations of this rule and we can leave this rule blank
 */
bool ARR34_C( const SgNode *node ) {
	return false;
}

/**
 * Do not add or subtract an integer to a pointer to a non-array object
 */
bool ARR37_C( const SgNode *node ) {
	const SgVarRefExp* varRef = isSgVarRefExp(node);
	if (!varRef || !isSgPointerType(varRef->get_type()))
		return false;

	const SgNode *parent = removeCasts(isSgExpression(varRef->get_parent()));
	if (!parent)
		return false;

	/**
	 *  See if we have a case of pointer arithmetic
	 */
	if (!(isSgPlusPlusOp(parent)
	||  isSgMinusMinusOp(parent)
	||  isSgAddOp(parent)
	||  isSgSubtractOp(parent)
	||  isSgPlusAssignOp(parent)
	||  isSgMinusAssignOp(parent)))
		return false;

	/* Search forward to find the varRef */
	FOREACH_SUBNODE( varRef->get_symbol()->get_scope(), nodes, i, V_SgVarRefExp) {
		const SgVarRefExp * iVar = isSgVarRefExp(*i);
		assert(iVar);
		if (iVar == varRef)
			break;
	}

	/* Now walk backwards looking at the assignments to our variable */
	assert(i != nodes.begin());
	bool flag = true;
	for (--i; flag; --i) {
		/* We have to be  a little tricky here since we can't do >= begin */
		if (i == nodes.begin())
			flag = false;
		const SgVarRefExp * iVar = isSgVarRefExp(*i);
		assert(iVar);
		if (getRefDecl(iVar) != getRefDecl(varRef))
			continue;
		parent = removeCasts(isSgExpression(iVar->get_parent()));
		assert(parent);
		const SgExpression *rhs = NULL;
		if (isSgAssignInitializer(parent)) {
			rhs = isSgAssignInitializer(parent)->get_operand();
		} else if (isSgAssignOp(parent)) {
			rhs = isSgBinaryOp(parent)->get_rhs_operand();
		} else {
			/* Hopefully this is just a reference to var */
			continue;
		}
		assert(rhs);
		rhs = removeImplicitPromotions(rhs);
		if (isSgAddressOfOp(rhs)) {
			FOREACH_SUBNODE(rhs, rhs_nodes, j, V_SgPntrArrRefExp) {
				return false;
			}
			print_error(node, "ARR37-C", "Do not add or subtract an integer to a pointer to a non-array object");
			return true;
		}
	}

	return false;
}

/**
 * Do not add or subtract an integer to a pointer if the resulting value does
 * not refer to a valid array element
 *
 * \note GCC catches violations of this rule where subscripts to static arrays
 * are out of range.
 */
bool ARR38_C( const SgNode *node ) {
	return false;
}

bool ARR_C(const SgNode *node) {
  bool violation = false;
  violation |= ARR01_C(node);
  violation |= ARR02_C(node);
  violation |= ARR30_C(node);
  violation |= ARR33_C(node);
  violation |= ARR34_C(node);
  violation |= ARR37_C(node);
  violation |= ARR38_C(node);
  return violation;
}


/// C++ checkers

bool ARR_CPP(const SgNode *node) {
  bool violation = false;
  return violation;
}

