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
 * \file value.C
 */

#include "rose.h"
#include "value.h"

/**
 * Takes a Value node and tries to extract the numeric value from it.  The
 * need for this exists because size_t are expressed as both UnsignedIntVal's
 * and UnsignedLongVal's.
 */
bool getSizetVal(const SgExpression *node, size_t *value) {
	if(!node)
		return false;
	if (isSgUnsignedIntVal(node)) {
		*value = isSgUnsignedIntVal(node)->get_value();
	} else if (isSgUnsignedLongVal(node)) {
		*value = isSgUnsignedLongVal(node)->get_value();
	} else {
		return false;
	}
	return true;
}

bool getIntegerVal(const SgExpression *node, intmax_t *n) {
	if(!node)
		return false;
	if (isSgUnsignedIntVal(node)) {
		*n = isSgUnsignedIntVal(node)->get_value();
	} else if (isSgIntVal(node)) {
		*n = isSgIntVal(node)->get_value();
	} else if (isSgUnsignedLongVal(node)) {
		*n = (intmax_t) (isSgUnsignedLongVal(node)->get_value());
	} else if (isSgLongIntVal(node)) {
		*n = isSgLongIntVal(node)->get_value();
	} else if (isSgUnsignedLongLongIntVal(node)) {
		*n = (intmax_t) isSgUnsignedLongLongIntVal(node)->get_value();
	} else if (isSgLongLongIntVal(node)) {
		*n = isSgLongLongIntVal(node)->get_value();
	} else if (isSgUnsignedShortVal(node)) {
		*n = isSgUnsignedShortVal(node)->get_value();
	} else if (isSgShortVal(node)) {
		*n = isSgShortVal(node)->get_value();
	} else {
		return false;
	}
	return true;
}

bool getFloatingVal(const SgExpression *node, long double *n) {
	if(!node)
		return false;
	if (isSgFloatVal(node)) {
		*n = isSgFloatVal(node)->get_value();
	} else if (isSgDoubleVal(node)) {
		*n = isSgDoubleVal(node)->get_value();
	} else if (isSgLongDoubleVal(node)) {
		*n = isSgLongDoubleVal(node)->get_value();
	} else {
		return false;
	}
	return true;
}

bool isVal(const SgExpression *node, const intmax_t n) {
	if (!node)
		return false;
	intmax_t x;
	if (!getIntegerVal(node, &x))
		return false;
	return x == n;
}

/**
 * Takes a Value node and tries to make sure it is 0
 */
bool isZeroVal(const SgExpression *node) {
	if (!node)
		return false;
	if (node->get_type()->isIntegerType()) {
		return isVal(node,0);
	} else if (node->get_type()->isFloatType()) {
		long double x;
		if (!getFloatingVal(node, &x))
			return false;
		return x == 0.0l;
	} else {
		return false;
	}
}

/**
 * Takes a Value node and tries to make sure it is the minimum
 */
bool isMinVal(const SgExpression *node) {
	if(!node)
		return false;
	if (isSgUnsignedIntVal(node)) {
		return 0 == isSgUnsignedIntVal(node)->get_value();
	} else if (isSgIntVal(node)) {
		return INT_MIN == isSgIntVal(node)->get_value();
	} else if (isSgUnsignedLongVal(node)) {
		return 0 == isSgUnsignedLongVal(node)->get_value();
	} else if (isSgLongIntVal(node)) {
		return LONG_MIN == isSgLongIntVal(node)->get_value();
	} else if (isSgUnsignedLongLongIntVal(node)) {
		return 0 == isSgUnsignedLongLongIntVal(node)->get_value();
	} else if (isSgLongLongIntVal(node)) {
		return std::numeric_limits<long long>::min() == isSgLongLongIntVal(node)->get_value();
	} else if (isSgUnsignedShortVal(node)) {
		return 0 == isSgUnsignedShortVal(node)->get_value();
	} else if (isSgShortVal(node)) {
		return SHRT_MIN == isSgShortVal(node)->get_value();
	} else if (isSgUnsignedCharVal(node)) {
		return 0 == isSgUnsignedCharVal(node)->get_value();
	} else if (isSgCharVal(node)) {
		return CHAR_MIN == isSgCharVal(node)->get_value();
	} else {
		return false;
	}
}

