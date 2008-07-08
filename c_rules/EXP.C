/**
 * \file EXP.C
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
 * Do not mistake sizeof( type*) for sizeof( type)
 *
 * We compare two types, one is inside the malloc, which prob looks like
 * X * sizeof( T1)
 * The other type is explicitly typecast:  (T2*) malloc(...)
 * Rule fails if T1 != T2 (usually because T1 == *T2)
 */
bool EXP01_A( const SgNode *node ) {
	const SgExpression* arg0 = getAllocFunctionExpr(isSgFunctionRefExp(node));
	if (arg0 == NULL)
		return false;

	const SgType* sg_t1 = NULL;
	FOREACH_SUBNODE(arg0, nodes, i, V_SgSizeOfOp) {
		const SgSizeOfOp* sizeOfOp = isSgSizeOfOp( *i);
		assert(sizeOfOp != NULL);
		const SgExpression* sizeOfExpr = sizeOfOp->get_operand_expr();
		/* We only care if there's a pointer inside the sizeof */
		if (!sizeOfExpr)
			continue;
		const SgVarRefExp* sizeOfVar = isSgVarRefExp(sizeOfExpr);
		if (!sizeOfVar)
			continue;
		sg_t1 = sizeOfVar->get_type();
		assert(sg_t1 != NULL);
		break;
	}
	if (sg_t1 == NULL)
		return false; // no sizeof() op in malloc call
	if (isSgArrayType(sg_t1))
		return false; // explicit arrays are OK
	Type t1(sg_t1);

	const SgNode* parent = node->get_parent();
	assert( parent != NULL);
	const SgCastExp* typecast = isSgCastExp( parent->get_parent());
	if (typecast == NULL)
		return false;
	Type t2_ptr_type( typecast->get_type());
	if (!t2_ptr_type.isPointer() && !t2_ptr_type.isArray())
		return false; // memory not allocated for array
	Type t2( t2_ptr_type.dereference().dereference());

	if (t1 == t2) {
		return false;
	}
	print_error( node, "EXP01-A", "Do not take the sizeof a pointer to determine the sizeof a type", true);
	return true;
}

/**
 * Do not cast away a const qualification
 *
 * We check cast expressions to make sure that if they don't posses a const
 * tag, then neither does the operand.
 *
 * \note GCC catches implicit casts, we just focus on explicit ones because
 * GCC assumes the programmer is right in those cases
 *
 * \see EXP32_C
 */
bool EXP05_A( const SgNode *node ) {
	const SgCastExp * cast = isSgCastExp(node);
	if(!cast)
		return false;
	/**
	 * Ignore compiler generated casts
	 */
	if(isCompilerGeneratedNode(node))
		return false;

	const SgExpression *expr = cast->get_operand();
	assert(expr);

	bool castIsConst = Type(cast->get_type()).isConst();
	bool exprIsConst = Type(expr->get_type()->dereference()).isConst();

	if(exprIsConst && !castIsConst) {
		print_error(node, "EXP05-A", "Do not cast away a const qualification", true);
		return true;
	}

	return false;
}

/**
 * Use sizeof to determine the size of a type or variable -jp
 *
 * If this alloc expr is being cast to a type char* or char[], bail, it's OK
 */
bool EXP09_A( const SgNode *node ) {
	const SgExpression* exp = getAllocFunctionExpr(isSgFunctionRefExp(node));
	if (exp == NULL)
		return false;

	const SgNode* parent = node->get_parent();
	assert( parent != NULL);

	const SgCastExp* typecast = isSgCastExp( parent->get_parent());
	if (typecast != NULL) {
		Type alloc_type = Type( typecast->get_type()).stripInitialPointersReferencesAndArrays();
		if (alloc_type.isAnyChar())
			return false;
	}

	/**
	 * We should allow size_t or rsize_t arguments
	 */
	const SgType *t = stripModifiers(exp->get_type());
	if (isSgTypedefType(t)
	&& ((t->unparseToString() == "size_t")
	 || (t->unparseToString() == "rsize_t")))
		return false;

	// Find a sizeof operator inside argument
	if (isSgSizeOfOp(exp))
		return false;

	FOREACH_SUBNODE(exp, nodes, i, V_SgSizeOfOp) {
		return false;
	}

	print_error(node, "EXP09-A", "malloc called using something other than sizeof()", true);
	return true;
}

/**
 * Do not apply operators expecting one type to data of an incompatible type
 *
 * \see FLP33_C
 * \see INT07_A
 */
bool EXP11_A( const SgNode *node ) {
	const SgBinaryOp *binOp = isSgBinaryOp(node);
	const SgInitializedName *var = isSgInitializedName(node);
	const SgType *lhsSgType;
	const SgExpression *rhs;

	if(binOp) {
		if (!isSgAssignOp(binOp))
			return false;
		lhsSgType = binOp->get_lhs_operand()->get_type();
		rhs = removeImplicitPromotions(binOp->get_rhs_operand());
	} else if(var) {
		lhsSgType = var->get_type();
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initializer());
		if(!init)
			return false;
		rhs = removeImplicitPromotions(init->get_operand());
	} else return false;

	const SgCastExp *cast = isSgCastExp(rhs);
	if (!(isSgPointerType(lhsSgType->stripTypedefsAndModifiers()) && cast))
		return false;

	const std::string lhsBase = stripModifiers(lhsSgType->findBaseType())->unparseToString();

	const SgExpression* castExpr = cast->get_operand();

	/**
	 * XXX all this hackery would be a lot simpler if we could just check for
	 * void *
	 */

	/// Exception b/c MEM02_A
	const SgFunctionRefExp *fnRef = isSgFunctionCallExp(castExpr) ? isSgFunctionRefExp(isSgFunctionCallExp(castExpr)->get_function()) : NULL;
	const bool castIsAlloc = fnRef
		&& (isCallOfFunctionNamed(fnRef, "malloc")
		|| isCallOfFunctionNamed(fnRef, "calloc")
		|| isCallOfFunctionNamed(fnRef, "realloc"));

	/// Exception b/c NULL is an int *
	const bool castIsZero = isZeroVal(castExpr);
	std::string castBase = stripModifiers(castExpr->get_type()->findBaseType())->unparseToString();

	/// Exception b/c strings are not pointers but can be assigned to them
	if(castBase == "char*") castBase = "char";

	if(!castIsZero && !castIsAlloc && (lhsBase != castBase)) {
		print_error(cast, "EXP11-A", "Do not apply operators expecting one type to data of an incompatible type", true);
		return true;
	}

	return false;
}

/**
 * Do not ignore values returned by functions
 */
bool EXP12_A( const SgNode *node ) {
	const SgFunctionRefExp *ref = isSgFunctionRefExp(node);
	/** WHITE LIST */
	if ((ref == NULL)
	|| isCallOfFunctionNamed(ref, "printf")
	|| isCallOfFunctionNamed(ref, "fprintf")
	|| isCallOfFunctionNamed(ref, "snprintf")
	|| isCallOfFunctionNamed(ref, "close")
	|| isCallOfFunctionNamed(ref, "fclose")
	|| isCallOfFunctionNamed(ref, "free")
	|| isCallOfFunctionNamed(ref, "fflush")
	|| isCallOfFunctionNamed(ref, "memcpy")
	|| isCallOfFunctionNamed(ref, "memset")
	|| isCallOfFunctionNamed(ref, "memmove")
	|| isCallOfFunctionNamed(ref, "strcpy")
	|| isCallOfFunctionNamed(ref, "strncpy"))
		return false;
	const SgFunctionCallExp *fn = isSgFunctionCallExp(ref->get_parent());
	if (!fn)
		return false;

	if (fn->get_type()->unparseToString() == "void")
		return false;

	const SgNode *parent = fn;
	while(1) {
		parent = parent->get_parent();
		assert(parent);
		if (isSgCastExp(parent)) {
			/**
			 * \bug Due to a bug in ROSE which ignores these casts, this
			 * condition will always be false :(
			 */
			if (isSgCastExp(parent)->get_type()->unparseToString() == "void")
				return false;
		} else if (isSgExprStatement(parent)) {
			print_error(node, "EXP12-A", "Do not ignore values returned by functions", true);
			return true;
		} else {
			return false;
		}
	}
}

/**
 * Do not cast away a volatile qualification
 *
 * We check cast expressions to make sure that if they don't posses a volatile
 * tag, then neither does the operand.
 *
 * \note GCC catches implicit casts, we just focus on explicit ones because
 * GCC assumes the programmer is right in those cases
 *
 * \see EXP05_A
 */
bool EXP32_C( const SgNode *node ) {
	const SgCastExp * cast = isSgCastExp(node);
	if(!cast)
		return false;

	const SgExpression *expr = cast->get_operand();
	assert(expr);

	bool castIsVolatile = Type(cast->get_type()).isVolatile();
	bool exprIsVolatile = Type(expr->get_type()->dereference()).isVolatile();

	if(exprIsVolatile && !castIsVolatile) {
		print_error(node, "EXP32-C", "Do not cast away a volatile qualification");
		return true;
	}

	return false;
}

/**
 * Ensure a null pointer is not dereferenced
 * We only check the output of malloc, calloc, and realloc. Don't check other
 * potential null ptrs.
 *
 * If allocated memory is passed to a variable x, ensure that x's next usage is something like: if (x == NULL) ...
 * First we get the var it is assigned to, which might be an assignment exp eg
 * x = malloc(...), or a decl initializer, eg char* x = malloc(...);
 */
bool EXP34_C( const SgNode *node ) {
	const SgExpression* exp = getAllocFunctionExpr(isSgFunctionRefExp(node));
	if (exp == NULL)
		return false;

	const SgInitializedName* var = NULL;
	// The node in the function where the variable first gets referred to
	const SgVarRefExp* ref = NULL;

	var = getVarAssignedTo(isSgFunctionRefExp(node), &ref);
	if (!var)
		return false;
	if (ref && isTestForNullOp( ref))
		return false;

	// Find all var references in function after malloc
	Rose_STL_Container<SgNode *> nodes = getNodesInFn( node);
	Rose_STL_Container<SgNode *>::const_iterator i = nodes.begin();
	if (ref) {
		for (; i != nodes.end(); ++i ) {
			if (ref == *i) {
				i++; break;
			} // var ref was malloc, start with next one
		}
	}
	if (i == nodes.end())
		return false;

	// Now to future variable references, find one with bad usage
	for (; i != nodes.end(); i = nextVarRef( nodes, i, var)) {
		const SgVarRefExp* i_ref = isSgVarRefExp(*i);
		assert( i_ref != NULL);
		if (var != getRefDecl( i_ref))
			continue;

		/**
		 * Allow references inside sizeof() since no actual memory derefencing
		 * happens
		 */
		if (findParentNodeOfType(i_ref, V_SgSizeOfOp).first)
			continue;

		if (isTestForNullOp( i_ref)) return false;
		const SgAssignOp* next_assignment
			= isSgAssignOp( findParentNodeOfType( i_ref, V_SgAssignOp).first);
		if (next_assignment == NULL) {
			print_error( node, "EXP34-C", "Ensure pointer is valid before dereferencing it");
			return true;
		}
	}

	// var never referenced again. (bad, but not caught by us)
	return false;
}


bool EXP(const SgNode *node) {
  bool violation = false;
  violation |= EXP01_A(node);
  violation |= EXP05_A(node);
  violation |= EXP09_A(node);
  violation |= EXP11_A(node);
  violation |= EXP12_A(node);
  violation |= EXP32_C(node);
  violation |= EXP34_C(node);
  return violation;
}
