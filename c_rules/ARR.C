/**
 * \file ARR.C
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

#include "rose.h"
#include "utilities.h"

/**
 * Do not use sizeof() on an incomplete array
 *
 * \todo Why are we using both SgType and Type? We should find a way to
 * implement get_index() in Type
 */
bool ARR01_A( const SgNode *node ) {
  const SgSizeOfOp* sizeOfOp = isSgSizeOfOp( node);
  if (sizeOfOp == NULL) return false;

  const SgExpression* sizeOfExpr = sizeOfOp->get_operand_expr();
  if (sizeOfExpr == NULL) return false; // ignore sizeof( type);

  const Type ref_type(sizeOfExpr->get_type());
  const Type type = ref_type.dereference();
  if (!type.isArray()) return false;
  const SgArrayType* aType = isSgArrayType( type.sgType());
  if (aType == NULL) return false;
  if (aType->get_index()) return false; // we know the size of the array

  print_error( node, "ARR01-A", "Do not use sizeof() on an incomplete array", true);
  return true;
}

/**
 * Explicitly specify array dimensions, even if implicitly defined by an
 * initializer
 *
 * \see STR36_A
 */
bool ARR02_A( const SgNode *node ) {
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
			print_error(*i, "ARR02-A", "Explicitly specify array dimensions, even if implicitly defined by an initializer", true);
		return true;
		}
	}

	return false;
}

/**
 * Tries to find sizeof(type)
 *
 * \note this function only works on the basic types, nothing fancy
 *
 * \return 0 on error
 */
size_t sizeOfType(const SgType *type) {
	const SgType *t = type->stripTypedefsAndModifiers();
	if (isSgTypeBool(t)) return sizeof(bool);
	else if (isSgTypeChar(t)) return sizeof(char);
	else if (isSgTypeDouble(t)) return sizeof(double);
	else if (isSgTypeFloat(t)) return sizeof(float);
	else if (isSgTypeInt(t)) return sizeof(int);
	else if (isSgTypeLong(t)) return sizeof(long);
	else if (isSgTypeLongDouble(t)) return sizeof(long double);
	else if (isSgTypeLongLong(t)) return sizeof(long long);
	else if (isSgTypeShort(t)) return sizeof(short);
	else if (isSgTypeSignedChar(t)) return sizeof(signed char);
	else if (isSgTypeSignedInt(t)) return sizeof(signed int);
	else if (isSgTypeSignedLong(t)) return sizeof(signed long);
	else if (isSgTypeSignedShort(t)) return sizeof(signed short);
	else if (isSgTypeUnsignedChar(t)) return sizeof(unsigned char);
	else if (isSgTypeUnsignedInt(t)) return sizeof(unsigned int);
	else if (isSgTypeUnsignedLong(t)) return sizeof(unsigned long);
	else if (isSgTypeUnsignedShort(t)) return sizeof(unsigned short);
	else if (isSgTypeWchar(t)) return sizeof(wchar_t);
	else return 0;
}

/**
 * Guarantee that copies are made into storage of sufficient size
 *
 * We make sure that the length argument to memcpy is at most the size of the
 * first argument.  We can only do this this if the first argument is a static
 * array.
 */
bool ARR33_C( const SgNode *node ) {
	if(!isCallOfFunctionNamed(node, "memcpy"))
		return false;
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	assert(fnRef);
	const SgExpression *dstExp = removeImplicitPromotions(getFnArg(fnRef,0));
	const SgExpression *lenExp = getFnArg(fnRef,2);
	assert(dstExp && lenExp);

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
		print_error(node, "ARR33-C", "Guarantee that copies are made into storage of sufficient size", true);
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
 * Do not add or subtract an integer to a pointer if the resulting value does
 * not refer to a valid array element
 *
 * \note GCC catches violations of this rule where subscripts to static arrays
 * are out of range.
 */
bool ARR38_C( const SgNode *node ) {
	return false;
}

bool ARR(const SgNode *node) {
  bool violation = false;
  violation |= ARR01_A(node);
  violation |= ARR02_A(node);
  violation |= ARR33_C(node);
  violation |= ARR34_C(node);
  violation |= ARR38_C(node);
  return violation;
}
