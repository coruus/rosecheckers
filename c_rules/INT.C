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
#include <boost/regex.hpp>

/**
 * Use rsize_t or size_t for all integer values representing the size of an
 * object
 */
bool INT01_C( const SgNode *node ) {
	const SgBinaryOp *op = isSgBinaryOp(node);
	if(!op)
		return false;

	/* Ignore commas */
	if(isSgCommaOpExp(op))
		return false;

	/* Ignore array derefences */
	if(isSgPntrArrRefExp(op))
		return false;

	const SgExpression *lhs = op->get_lhs_operand();
	const SgExpression *rhs = op->get_rhs_operand();
	const SgType *lhsType = stripModifiers(lhs->get_type());
	const SgType *rhsType = stripModifiers(rhs->get_type());

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

	bool lhsIsSize = isSgTypedefType(lhsType) && (lhsType->unparseToString() == "size_t");
	bool lhsIsRSize = isSgTypedefType(lhsType) && (lhsType->unparseToString() == "rsize_t");
	bool rhsIsSize = isSgTypedefType(rhsType) && (rhsType->unparseToString() == "size_t");
	bool rhsIsRSize = isSgTypedefType(rhsType) && (rhsType->unparseToString() == "rsize_t");

	// Treat SizeOfOp as having type size_t even though compiler will assign
	// some kind of unsigned integer type to it
	if (isSgSizeOfOp(lhs))
		lhsIsSize = true;
	if (isSgSizeOfOp(rhs))
		rhsIsSize = true;

	// Common idiom is to do something like (sizeof(foo) - 1)
	const SgBinaryOp *lhsOp = isSgBinaryOp(lhs);
	if(lhsOp) {
		if ((isSgSizeOfOp(lhsOp->get_lhs_operand())
			&& isSgValueExp(lhsOp->get_rhs_operand()))
		|| ((isSgSizeOfOp(lhsOp->get_rhs_operand())
			&& isSgValueExp(lhsOp->get_lhs_operand()))))
			lhsIsSize = true;
	}
	const SgBinaryOp *rhsOp = isSgBinaryOp(rhs);
	if(rhsOp) {
		if ((isSgSizeOfOp(rhsOp->get_rhs_operand())
			&& isSgValueExp(rhsOp->get_lhs_operand()))
		|| ((isSgSizeOfOp(rhsOp->get_lhs_operand())
			&& isSgValueExp(rhsOp->get_rhs_operand()))))
			rhsIsSize = true;
	}

	if((lhsIsSize ^ rhsIsSize) || (lhsIsRSize ^ rhsIsRSize)) {
		print_error(node,"INT01-C", "Use rsize_t or size_t for all integer values representing the size of an object", true);
		return true;
	}

	return false;
}

/**
 * Do not use input functions to convert character data if they cannot handle
 * all possible inputs
 */
bool INT05_C( const SgNode *node ) {
	const SgFunctionRefExp *ref = isSgFunctionRefExp( node);
	if (ref == NULL) return false;
	int numArg = getScanfFormatString(ref);
	if (numArg == -1) return false;

	const SgStringVal *format = isSgStringVal(removeCasts(getFnArg(ref,numArg)));
	if (!format)
		return false;

	const std::string formatStr = format->get_value();

	boost::regex r("\%.?[fDdiouxXegE]");
	if (regex_search(formatStr,r)) {
		print_error(node, "INT05-C", "Do not use input functions to convert character data if they cannot handle all possible inputs", true);
		return true;
	}

	return false;
}

/**
 * Use strtol() or a related function to convert a string token to an integer
 *
 * \todo catch sscanf conversions
 */
bool INT06_C( const SgNode *node ) {
	if(isCallOfFunctionNamed(node,"atoi")
			|| isCallOfFunctionNamed(node,"atol")
			|| isCallOfFunctionNamed(node,"atoll")
			|| isCallOfFunctionNamed(node,"atoq")) {
		print_error(node,"INT01-C", "Use strtol() or a related function to convert a string token to an integer", true);
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
bool INT07_C( const SgNode *node ) {
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
			FOREACH_SUBNODE(stat, nodes, i, V_SgAssignOp) {
				const SgAssignOp *assign = isSgAssignOp(*i);
				const SgFunctionRefExp *fn = isSgFunctionRefExp(assign->get_rhs_operand());
				if(isCallOfFunctionNamed(fn, "fgetc")
				|| isCallOfFunctionNamed(fn, "gettc")
				|| isCallOfFunctionNamed(fn, "getchar")) {
					return false;
				}
			}
		}*/

		print_error(node, "INT07-C", "Use only explicitly signed or unsigned char type for numeric values", true);
		return true;
	}
	return false;
}

/**
 * Do not make assumptions about the type of a plain int bit-field when used
 * in an expression
 */
bool INT12_C( const SgNode *node ) {
	const SgInitializedName *varName = isSgInitializedName(node);
	if (!varName)
		return false;

	if (!findParentNodeOfType(varName, V_SgClassDefinition).first)
		return false;

	const SgVariableDefinition *varDef = isSgVariableDefinition(varName->get_definition());
	assert(varDef);

	if (!varDef->get_bitfield())
		return false;

	const SgType *varType = varName->get_type();
	if (isSgTypeChar(varType)
	||  isSgTypeShort(varType)
	||  isSgTypeInt(varType)
	||  isSgTypeLong(varType)
	||  isSgTypeLongLong(varType)) {
		print_error(node, "INT12-C", "Do not make assumptions about the type of a plain int bit-field when used in an expression", true);
		return true;
	}

	return false;
}

/**
 * Use bitwise operators only on unsigned operands
 */
bool INT13_C( const SgNode *node ) {
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
			/** Allow compile time constants in a shift */
			if(isSgValueExp(binOp->get_lhs_operand())
			|| isSgValueExp(binOp->get_rhs_operand())) {
				return false;
			}
			if(!Type(binOp->get_lhs_operand()->get_type()).isUnsigned()) {
				violation = true;
			}
		}
	}

	if(violation) {
		print_error(node, "INT13-C", "Use bitwise operators only on unsigned operands", true);
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
 * Ensure that operations on signed integers do not result in overflow
 *
 * \note We only check the Unary negation case here
 *
 * \see INT33_C
 */
bool INT32_C( const SgNode *node ) {
	const SgMinusOp *negOp = isSgMinusOp(node);
	if(!negOp)
		return false;

	const SgVarRefExp * var= isSgVarRefExp(negOp->get_operand());
	if (!var)
		return false;
	const SgInitializedName *varName = getRefDecl(var);
	assert(varName);

	FOREACH_SUBNODE(findInBlockByOffset(node, -1), nodes, i, V_SgBinaryOp) {
		const SgBinaryOp *binOp = isSgBinaryOp(*i);
		assert(binOp);

		const SgExpression *lhs = binOp->get_lhs_operand();
		SgValueExp *rhs = isSgValueExp(binOp->get_rhs_operand());
		const SgVarRefExp *iVar = isSgVarRefExp(lhs);
		if (iVar && (getRefDecl(iVar) == varName)) {
			if (isMinVal(computeValueTree(rhs)))
				return false;
		}
		iVar = isSgVarRefExp(rhs);
		if (iVar && (getRefDecl(iVar) == varName)) {
			if (isMinVal(lhs))
				return false;
		}
	}

	print_error(node,"INT32-C", "Ensure that operations on signed integers do not result in overflow");
	return true;
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
	 * If the variable is constant, it's likely that the programmer already
	 * knows the value and a check is not necessary
	 */
	Type varType(varRef->get_type());
	if (varType.isConst())
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

	print_error(node,"INT33-C", "Ensure that division and modulo operations do not result in divide-by-zero errors");
	return true;
}

bool INT(const SgNode *node) {
  bool violation = false;
  violation |= INT01_C(node);
  violation |= INT05_C(node);
  violation |= INT06_C(node);
  violation |= INT07_C(node);
  violation |= INT12_C(node);
  violation |= INT13_C(node);
  violation |= INT32_C(node);
  violation |= INT33_C(node);
  return violation;
}
