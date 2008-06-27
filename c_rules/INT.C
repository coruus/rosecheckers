/**
 * \file INT.C
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
 * Use rsize_t or size_t for all integer values representing the size of an
 * object
 */
bool INT01_A( const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if(!op)
		return false;
	/* Ignore array derefences */
	if(isSgPntrArrRefExp(op))
		return false;

	const SgType *lhsType = stripModifiers(op->get_lhs_operand()->get_type());
	const SgType *rhsType = stripModifiers(op->get_rhs_operand()->get_type());

	/**
	 * We should allow pointer arithmetic, and a special exception for
	 * uintptr_t because it in a typedef to unsigned int and is not actually a
	 * pointer type
	 */
	if(isSgPointerType(lhsType)
			|| isSgPointerType(rhsType)
			|| (lhsType->unparseToString() == "uintptr_t")
			|| (rhsType->unparseToString() == "uintptr_t")
			|| isSgArrayType(lhsType)
			|| isSgArrayType(rhsType)) {
		return false;
	}

	/**
	 * scalars are ok
	 */
	if(isSgValueExp(op->get_lhs_operand())
			|| isSgValueExp(op->get_rhs_operand()))
		return false;

	const bool lhsIsSize = isSgTypedefType(lhsType) && (lhsType->unparseToString() == "size_t");
	const bool lhsIsRSize = isSgTypedefType(lhsType) && (lhsType->unparseToString() == "rsize_t");
	const bool rhsIsSize = isSgTypedefType(rhsType) && (rhsType->unparseToString() == "size_t");
	const bool rhsIsRSize = isSgTypedefType(rhsType) && (rhsType->unparseToString() == "rsize_t");

	if((lhsIsSize ^ rhsIsSize) || (lhsIsRSize ^ rhsIsRSize)) {
		print_error(node,"INT01-A", "Use rsize_t or size_t for all integer values representing the size of an object", true);
		return true;
	}

	return false;
}

/**
 * Use strtol() or a related function to convert a string token to an integer
 *
 * \todo catch sscanf conversions
 */
bool INT06_A( const SgNode *node ) {
	if(isCallOfFunctionNamed(node,"atoi")
			|| isCallOfFunctionNamed(node,"atol")
			|| isCallOfFunctionNamed(node,"atoll")
			|| isCallOfFunctionNamed(node,"atoq")) {
		print_error(node,"INT01-A", "Use strtol() or a related function to convert a string token to an integer", true);
		return true;
	}

	return false;
}

/**
 * Use only explicitly signed or unsigned char type for numeric values
 *
 * \bug (char c = 'a') will trigger a false positive, work around by
 * using (char c; c = 'a')
 */
bool INT07_A( const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	const SgInitializedName *var = isSgInitializedName(node);
	const SgType *lhsSgType;
	const SgType *rhsSgType;

	if(binOp) {
		lhsSgType = binOp->get_lhs_operand()->get_type();
		rhsSgType = removeImplicitPromotions(binOp->get_rhs_operand())->get_type();
	} else if(var) {
		lhsSgType = var->get_type();
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if(!init)
			return false;
		rhsSgType = removeCasts(init->get_operand())->get_type();
	} else return false;

	const Type &lhsType = Type(lhsSgType).stripTypedefsAndModifiers();
	const Type &rhsType = Type(rhsSgType).stripTypedefsAndModifiers();

	if(lhsType.isPlainChar() && (rhsType.isSigned()||rhsType.isUnsigned())) {
		/**
		 * Most of our false positives will come from getchar et al
		 * However, there is no good way to find those cases... there's just
		 * too many ways a getchar can get called
		 */
/*		const SgNode * parent = isSgBasicBlock(findParentNodeOfType(node, V_SgBasicBlock).first)->get_parent();
		const SgStatement *stat =
			isSgIfStmt(parent)? isSgIfStmt(parent)->get_conditional():
			isSgWhileStmt(parent)? isSgWhileStmt(parent)->get_condition():
			isSgDoWhileStmt(parent)? isSgDoWhileStmt(parent)->get_condition():
			isSgForStatement(parent)? isSgForStatement(parent)->get_test():
			NULL;

		if (stat) {
			std::cerr << "stat" <<std::endl;
			std::cerr << stat->unparseToString() <<std::endl;
			FOREACH_SUBNODE(stat, nodes, i, V_SgAssignOp) {
				const SgAssignOp *assign = isSgAssignOp(*i);
				std::cerr << assign->unparseToString() << std::endl;
				const SgFunctionRefExp *fn = isSgFunctionRefExp(assign->get_rhs_operand());
				if(isCallOfFunctionNamed(fn, "fgetc")
				|| isCallOfFunctionNamed(fn, "gettc")
				|| isCallOfFunctionNamed(fn, "getchar")) {
					return false;
				}
			}
		}*/

		print_error(node, "INT07-A", "Use only explicitly signed or unsigned char type for numeric values", true);
		return true;
	}
	return false;
}

/**
 * Use bitwise operators only on unsigned operands
 */
bool INT13_A( const SgNode *node ) {
	bool violation = false;
	if(isSgBitComplementOp(node)) {
		const SgBitComplementOp *bitOp = isSgBitComplementOp(node);
		/** Allow compile time constants to be bit negated */
		if(isSgValueExp(bitOp->get_operand())) {
			return false;
		}
		if(!Type(bitOp->get_operand()->get_type()).isUnsigned()) {
			violation = true;
		}
	} else if(isSgBinaryOp(node)) {
		const SgBinaryOp *binOp = isSgBinaryOp(node);
		/**
		 * Allow &/| on compile time constants
		 */
		if(isSgBitAndOp(binOp)
		|| isSgBitOrOp(binOp)) {
			if(isSgValueExp(binOp->get_lhs_operand())
			|| isSgValueExp(binOp->get_rhs_operand())) {
				return false;
			}
		}
		if(isSgAndAssignOp(binOp)
		|| isSgBitAndOp(binOp)
		|| isSgIorAssignOp(binOp)
		|| isSgBitXorOp(binOp)
		|| isSgIorAssignOp(binOp)
		|| isSgXorAssignOp(binOp)
		|| isSgBitOrOp(binOp)) {
			if((!Type(binOp->get_lhs_operand()->get_type()).isUnsigned())
			|| (!Type(binOp->get_rhs_operand()->get_type()).isUnsigned())) {
				violation = true;
			}
		} else if(isSgLshiftOp(binOp)
		|| isSgLshiftAssignOp(binOp)
		|| isSgRshiftOp(binOp)
		|| isSgRshiftAssignOp(binOp)) {
			/** Allow compile time constants on the right side of a shift */
			if(isSgValueExp(binOp->get_rhs_operand())) {
				return false;
			}
			if(!Type(binOp->get_lhs_operand()->get_type()).isUnsigned()) {
				violation = true;
			}
		}
	}

	if(violation) {
		print_error(node, "INT13-A", "Use bitwise operators only on unsigned operands", true);
		return true;
	}

	return false;
}

static bool isCheckForZero(const SgStatement *stat, const SgVarRefExp *varRef) {
	if (!stat)
		return false;

	const SgVarRefExp *compareVar;
	const SgExpression *lhs;
	const SgExpression *rhs;
	FOREACH_SUBNODE(stat, nodes, i, V_SgBinaryOp) {
		assert(*i);
		if(!(isSgEqualityOp(*i) || isSgNotEqualOp(*i)))
			continue;
		lhs = removeImplicitPromotions(isSgBinaryOp(*i)->get_lhs_operand());
		rhs = removeImplicitPromotions(isSgBinaryOp(*i)->get_rhs_operand());
		if ((compareVar = isSgVarRefExp(lhs))
		&&  (compareVar->get_symbol()->get_name() == varRef->get_symbol()->get_name())
		&& isZeroVal(rhs))
			return true;
		if ((compareVar = isSgVarRefExp(rhs))
		&&  (compareVar->get_symbol()->get_name() == varRef->get_symbol()->get_name())
		&& isZeroVal(lhs))
			return true;
	}

	return false;
}

/**
 * Ensure that division and modulo operations do not result in divide-by-zero
 * errors
 *
 * \note INT07 sets off false positives, buts that's OK
 */
bool INT33_C( const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	if (!(isSgModOp(binOp) || isSgDivideOp(binOp))) {
		return false;
	}

	const SgVarRefExp *varRef = isSgVarRefExp(removeCasts(binOp->get_rhs_operand()));
	if (!varRef)
		return false;

	/**
	 * See if we checked the var against zero in the previous line or
	 * maybe we're in an if statement or something that has the check in the
	 * conditional
	 */
	const SgNode * parent = isSgBasicBlock(findParentNodeOfType(node, V_SgBasicBlock).first)->get_parent();
	const SgStatement *stat =
		isSgIfStmt(parent)? isSgIfStmt(parent)->get_conditional():
		isSgWhileStmt(parent)? isSgWhileStmt(parent)->get_condition():
		isSgDoWhileStmt(parent)? isSgDoWhileStmt(parent)->get_condition():
		isSgForStatement(parent)? isSgForStatement(parent)->get_test():
		findInBlockByOffset(node, -1);
	if (stat && isCheckForZero(stat, varRef))
		return false;


	print_error(node,"INT33-C", "Ensure that division and modulo operations do not result in divide-by-zero errors", true);
	return true;
}

bool INT(const SgNode *node) {
  bool violation = false;
  violation |= INT01_A(node);
  violation |= INT06_A(node);
  violation |= INT07_A(node);
  violation |= INT13_A(node);
  violation |= INT33_C(node);
  return violation;
}
