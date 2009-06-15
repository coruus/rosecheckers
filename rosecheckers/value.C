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

