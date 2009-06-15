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
 * \file categories.C
 */

#include "rose.h"
#include "categories.h"

const SgBinaryOp *isAnyBinArithOp(const SgNode *node) {
	if (isSgAddOp(node)
	  ||isSgDivideOp(node)
	  ||isSgModOp(node)
	  ||isSgSubtractOp(node)
	  ||isSgMultiplyOp(node)
	  ||isSgExponentiationOp(node)
	  ||isSgIntegerDivideOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinArithAssignOp(const SgNode *node) {
	if (isSgPlusAssignOp(node)
	  ||isSgDivAssignOp(node)
	  ||isSgModAssignOp(node)
	  ||isSgMinusAssignOp(node)
	  ||isSgMultAssignOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinBitOp(const SgNode *node) {
	if (isSgBitAndOp(node)
	  ||isSgBitOrOp(node)
	  ||isSgBitXorOp(node)
	  ||isSgLshiftOp(node)
	  ||isSgRshiftOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinBitAssignOp(const SgNode *node) {
	if (isSgAndAssignOp(node)
	  ||isSgIorAssignOp(node)
	  ||isSgXorAssignOp(node)
	  ||isSgLshiftAssignOp(node)
	  ||isSgRshiftAssignOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyBinLogicOp(const SgNode *node) {
	if (isSgAndOp(node)
	  ||isSgOrOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyAssignOp(const SgNode *node) {
	if (isSgAssignOp(node)
	  ||isAnyBinArithAssignOp(node)
	  ||isAnyBinBitAssignOp(node)
	  ||isAnyBinLogicOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyEqualityOp(const SgNode *node) {
	if (isSgEqualityOp(node)
	  ||isSgNotEqualOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyRelationalOp(const SgNode *node) {
	if (isSgGreaterOrEqualOp(node)
	  ||isSgGreaterThanOp(node)
	  ||isSgLessOrEqualOp(node)
	  ||isSgLessThanOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

const SgBinaryOp *isAnyComparisonOp(const SgNode *node) {
	if (isAnyEqualityOp(node)
	  ||isAnyRelationalOp(node)) {
		return isSgBinaryOp(node);
	}
	return NULL;
}

/** NOT ACCOUNTED FOR
	  ||isSgConcatenationOp(node)
	  ||isSgScopeOp(node)
	  ||isSgArrowExp(node)
	  ||isSgArrowStarOp(node)
	  ||isSgCommaOpOp(node)
	  ||isSgDotExp(node)
	  ||isSgDotStarOp(node)
	  ||isSgPntrArrRefExp(node)
*/

/**
 * Checks to see if the type is some kind of char or wchar_t
 *
 * \note we return false on [un]signed chars since they are numbers not
 * characters
 */
bool isAnyCharType(const SgType *type) {
	const SgType *t = type->stripTypedefsAndModifiers();
	return isSgTypeChar(t)
		|| isSgTypeWchar(t)
		|| isTypeWcharT(type);
}

bool isAnyPointerType(const SgType *type) {
	const SgType *t = type->stripTypedefsAndModifiers();
	return isSgPointerType(t)
		|| isSgArrayType(t)
		|| isTypeUintptrT(type);
}
