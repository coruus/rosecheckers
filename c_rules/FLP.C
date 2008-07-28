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
 * Consider avoiding floating point numbers when precise computation is needed
 */
bool FLP02_C( const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if (!op)
		return false;
	if (!(isSgEqualityOp(op)
		||isSgNotEqualOp(op)))
		return false;

	const SgExpression *lhs = op->get_lhs_operand();
	assert(lhs);
	const SgExpression *rhs = op->get_rhs_operand();
	assert(rhs);

	if (Type(lhs->get_type()).isFloatingPoint()
	  ||Type(rhs->get_type()).isFloatingPoint()) {
		if (isZeroVal(removeCasts(lhs))
		||  isZeroVal(removeCasts(rhs)))
			return false;
		
		print_error(node, "FLP02-C", "Consider avoiding floating point numbers when precise computation is needed", true);
		return true;
	}
	return false;
}

/**
 * Detect and handle floating point errors
 *
 * \bog ROSE can't handle the FENV_ACCESS pragma :( so there's no way to a
 * have a compliant solution here
 */
bool FLP03_C( const SgNode *node ) {
	if (isCompilerGeneratedNode(node))
		return false;

	/**
	 * We are looking for floating point mult, divide (with non const/value on
	 * rhs), and floating point casts from double->float
	 */
	if (isSgBinaryOp(node)) {
		const SgBinaryOp *op = isSgBinaryOp(node);
		assert(op);
		const Type lhsT(removeCasts(op->get_lhs_operand())->get_type());
		const Type rhsT(removeCasts(op->get_rhs_operand())->get_type());
		if (!(lhsT.isFloatingPoint() || rhsT.isFloatingPoint()))
			return false;
		bool lhsKnown = isSgValueExp(removeCasts(op->get_lhs_operand())) || lhsT.isConst();
		bool rhsKnown = isSgValueExp(removeCasts(op->get_rhs_operand())) || rhsT.isConst();
		if (isSgDivideOp(op)) {
			if (rhsKnown)
				return false;

			/** Also check for a divde-by-zero check on the previous line */
			const SgStatement *prevSt = findInBlockByOffset(node, -1);
			const SgVarRefExp *varRef = isSgVarRefExp(removeCasts(op->get_rhs_operand()));
			if (prevSt && varRef) {
				FOREACH_SUBNODE(prevSt, nodes, i, V_SgStatement) {
					if (isCheckForZero(isSgStatement(*i), varRef))
					return false;
				}
			}
		} else if (isSgMultiplyOp(op)) {
			if (lhsKnown && rhsKnown)
				return false;
		} else {
			return false;
		}
	} else if (isSgCastExp(node)) {
		const SgCastExp *cast = isSgCastExp(node);
		assert(cast);
		const Type lhsT(cast->get_type());
		const Type rhsT(cast->get_operand()->get_type());
		if (!(lhsT.isDouble() && rhsT.isFloat()))
			return false;
	} else {
		return false;
	}

	const SgStatement *prevSt = findInBlockByOffset(node, -1);
	bool no_feclearexcept = true;
	if (prevSt) {
		FOREACH_SUBNODE(prevSt, nodes, i, V_SgFunctionCallExp) {
			if (isCallOfFunctionNamed(*i, "feclearexcept"))
				no_feclearexcept = false;
		}
	}

	const SgStatement *nextSt = findInBlockByOffset(node,  1);
	bool no_fetestexcept = true;
	if (nextSt) {
		FOREACH_SUBNODE(nextSt, nodes, i, V_SgFunctionCallExp) {
			if (isCallOfFunctionNamed(*i, "fetestexcept"))
				no_fetestexcept = false;
		}
	}

	if (no_feclearexcept || no_fetestexcept) {
		print_error(node, "FLP03-C", "Detect and handle floating point errors", true);
		return true;
	}

	return false;
}

/**
 * Do not use floating point variables as loop counters
 */
bool FLP30_C( const SgNode *node ) {
	const SgForStatement *forSt = isSgForStatement(node);
	if (!forSt)
		return false;

	FOREACH_SUBNODE(forSt->get_increment(), nodes, i, V_SgVarRefExp) {
		if (Type(isSgVarRefExp(*i)->get_type()).isFloatingPoint()) {
			print_error(*i, "FLP30-C", "Do not use floating point variables as loop counters");
			return true;
		}
	}

	return false;
}

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
 * \see INT07_C
 */
bool FLP33_C( const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	const SgInitializedName *var = isSgInitializedName(node);
	const SgType *lhsSgType;
	const SgType *rhsSgType;

	if(binOp) {
		/**
		 * \todo Does this rule even make sense for binory ops?
		 */
		/**
		 * This should allow macros like isnan
		 */
		if (isCompilerGeneratedNode(binOp))
			return false;

		if (!Type(binOp->get_type()).isFloatingPoint())
			return false;
		if (isSgPntrArrRefExp(binOp))
			return false;

		lhsSgType = binOp->get_lhs_operand()->get_type();
		/**
		 * Let's leave those casts in place to allow for macros like isnan
		 */
		rhsSgType = binOp->get_rhs_operand()->get_type();
//		rhsSgType = removeImplicitPromotions(binOp->get_rhs_operand())->get_type();
		assert(lhsSgType);
		assert(rhsSgType);
		const Type &lhsType = Type(lhsSgType).stripTypedefsAndModifiers();
		const Type &rhsType = Type(rhsSgType).stripTypedefsAndModifiers();
		if(lhsType.isFloatingPoint() || rhsType.isFloatingPoint()) {
			return false;
		}
	} else if(var) {
		lhsSgType = var->get_type();
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if(!init)
			return false;
		rhsSgType = removeImplicitPromotions(init->get_operand())->get_type();

		assert(lhsSgType);
		assert(rhsSgType);
		const Type &lhsType = Type(lhsSgType).stripTypedefsAndModifiers();
		const Type &rhsType = Type(rhsSgType).stripTypedefsAndModifiers();
		if(!(lhsType.isFloatingPoint() && rhsType.isIntegral()))
			return false;
	} else
		return false;


	print_error(node, "FLP33-C", "Convert integers to floating point for floating point operations");
	return true;
}

/**
 * Ensure that floating point conversions are within range of the new type
 */
bool FLP34_C( const SgNode *node ) {
	/**
	 * \bug We really shouldn't have this cop-out here, but a lot of macros
	 * (ie. isnan) will trigger this rule otherwise
	 */
	if (isCompilerGeneratedNode(node))
		return false;

	const SgType *lhsType = NULL;
	const SgExpression *rhs = NULL;
	if (const SgBinaryOp *op = isSgBinaryOp(node)) {
		/** \todo Add the other assign op's */
		if (!isSgAssignOp(op))
			return false;
		lhsType = op->get_lhs_operand()->get_type()->stripTypedefsAndModifiers();
		rhs = op->get_rhs_operand();
	} else if (const SgCastExp *cast = isSgCastExp(node)) {
		lhsType = cast->get_type()->stripTypedefsAndModifiers();
		rhs = cast->get_operand();
	} else {
		return false;
	}
	assert(lhsType && rhs);
	const SgType *rhsType = rhs->get_type()->stripTypedefsAndModifiers();
	assert(rhsType);
	const Type lhsT(lhsType);
	const Type rhsT(rhsType);
	if (!rhsT.isFloatingPoint())
		return false;
	if (lhsT.isFloatingPoint() && (sizeOfType(lhsType) >= sizeOfType(rhsType)))
		return false;

	if (valueVerified(removeCasts(rhs)))
		return false;

	print_error(node, "FLP34-C", "Ensure that floating point conversions are within range of the new type");
	return true;
}

bool FLP(const SgNode *node) {
  bool violation = false;
  violation |= FLP02_C(node);
  violation |= FLP03_C(node);
  violation |= FLP30_C(node);
  violation |= FLP31_C(node);
  violation |= FLP33_C(node);
  violation |= FLP34_C(node);
  return violation;
}
