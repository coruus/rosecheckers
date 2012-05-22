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
