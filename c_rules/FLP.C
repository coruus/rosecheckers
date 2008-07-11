/**
 * \file FLP.C
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
 * Do not call functions expecting real values with complex values
 */
bool FLP31_C( const SgNode *node ) {
	bool violation = false;;
	/* one arg */
	if (isCallOfFunctionNamed(node, "cbrt")
	||  isCallOfFunctionNamed(node, "ceil")
	||  isCallOfFunctionNamed(node, "erf")
	||  isCallOfFunctionNamed(node, "erfc")
	||  isCallOfFunctionNamed(node, "exp2")
	||  isCallOfFunctionNamed(node, "expm1")
	||  isCallOfFunctionNamed(node, "floor")
	||  isCallOfFunctionNamed(node, "frexp")
	||  isCallOfFunctionNamed(node, "ilogb")
	||  isCallOfFunctionNamed(node, "ldexp")
	||  isCallOfFunctionNamed(node, "lgamma")
	||  isCallOfFunctionNamed(node, "llrint")
	||  isCallOfFunctionNamed(node, "llround")
	||  isCallOfFunctionNamed(node, "log10")
	||  isCallOfFunctionNamed(node, "log1p")
	||  isCallOfFunctionNamed(node, "log2")
	||  isCallOfFunctionNamed(node, "logb")
	||  isCallOfFunctionNamed(node, "lrint")
	||  isCallOfFunctionNamed(node, "lround")
	||  isCallOfFunctionNamed(node, "nearbyint")
	||  isCallOfFunctionNamed(node, "rint")
	||  isCallOfFunctionNamed(node, "round")
	||  isCallOfFunctionNamed(node, "tgamma")
	||  isCallOfFunctionNamed(node, "trunc")) {
		violation = isSgTypeComplex(getFnArg(isSgFunctionRefExp(node), 0)->get_type());
	} else
	/* two args */
	if (isCallOfFunctionNamed(node, "atan2")
	||  isCallOfFunctionNamed(node, "copysign")
	||  isCallOfFunctionNamed(node, "fdim")
	||  isCallOfFunctionNamed(node, "fmax")
	||  isCallOfFunctionNamed(node, "fmin")
	||  isCallOfFunctionNamed(node, "fmod")
	||  isCallOfFunctionNamed(node, "hypot")
	||  isCallOfFunctionNamed(node, "nextafter")
	||  isCallOfFunctionNamed(node, "nexttoward")
	||  isCallOfFunctionNamed(node, "remainder")
	||  isCallOfFunctionNamed(node, "remquo")
	||  isCallOfFunctionNamed(node, "scalbn")
	||  isCallOfFunctionNamed(node, "scalbln")) {
	} else
	/* three args */
	if (isCallOfFunctionNamed(node, "fma")) {
	}
	if (violation) {
		print_error(node, "FLP31-C", "Do not call functions expecting real values with complex values");
		return true;
	}
	return false;
}

/**
 * Convert integers to floating point for floating point operations
 *
 * \see INT07_A
 */
bool FLP33_C( const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	const SgInitializedName *var = isSgInitializedName(node);
	const SgType *lhsSgType;
	const SgType *rhsSgType;

	if(binOp) {
		/**
		 * This should allow macros like isnan
		 */
		if (isCompilerGeneratedNode(binOp))
			return false;

		lhsSgType = binOp->get_lhs_operand()->get_type();
		/**
		 * Let's leave those casts in place to allow for macros like isnan
		 */
		rhsSgType = binOp->get_rhs_operand()->get_type();
//		rhsSgType = removeImplicitPromotions(binOp->get_rhs_operand())->get_type();
	} else if(var) {
		lhsSgType = var->get_type();
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if(!init)
			return false;
		rhsSgType = removeImplicitPromotions(init->get_operand())->get_type();
	} else
		return false;

	assert(lhsSgType);
	assert(rhsSgType);

	const Type &lhsType = Type(lhsSgType).stripTypedefsAndModifiers();
	const Type &rhsType = Type(rhsSgType).stripTypedefsAndModifiers();

	if(lhsType.isFloatingPoint() && rhsType.isIntegral()) {
		print_error(node, "FLP33-C", "Convert integers to floating point for floating point operations");
		return true;
	}
	return false;
}

bool FLP(const SgNode *node) {
  bool violation = false;
  violation |= FLP31_C(node);
  violation |= FLP33_C(node);
  return violation;
}
