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
 * Returns size argument to malloc, calloc, or realloc, if node is appropriate
 * function call, otherwise returns NULL
 *
 * \note As written, these tests catch template declarations only if
 * instantiated.
 *
 * \note This code is ugly :(
 */
const SgExpression* getAllocFunctionExpr(const SgNode *node) {
	return isCallOfFunctionNamed( node, "malloc") ? getFnArg( isSgFunctionRefExp( node), 0)
		: isCallOfFunctionNamed( node, "calloc") ? getFnArg( isSgFunctionRefExp( node), 1)
		: isCallOfFunctionNamed( node, "realloc") ? getFnArg( isSgFunctionRefExp( node), 1)
		: NULL;
}

/**
 * Do not mistake sizeof( type*) for sizeof( type)
 *
 * We compare two types, one is inside the malloc, which prob looks like
 * X * sizeof( T1)
 * The other type is explicitly typecast:  (T2*) malloc(...)
 * Rule fails if T1 != T2 (usually because T1 == *T2)
 */
bool EXP01_A( const SgNode *node ) {
	const SgExpression* arg0 = getAllocFunctionExpr( node);
	if (arg0 == NULL)
		return false;

	const SgType* sg_t1 = NULL;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( const_cast<SgExpression*>( arg0), V_SgSizeOfOp );
	for (Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		const SgSizeOfOp* sizeOfOp = isSgSizeOfOp( *i);
		assert(sizeOfOp != NULL);
		const SgExpression* sizeOfExpr = sizeOfOp->get_operand_expr();
		sg_t1 = (sizeOfExpr == NULL) ? sizeOfOp->get_operand_type() : sizeOfExpr->get_type();
		assert(sg_t1 != NULL);
		break;
	}
	if (sg_t1 == NULL)
		return false; // no sizeof() op in malloc call
	Type t1( Type( sg_t1).dereference());
	if (t1.isArray())
		return false; // explicit arrays are OK

	const SgNode* parent = node->get_parent();
	assert( parent != NULL);
	const SgCastExp* typecast = isSgCastExp( parent->get_parent());
	if (typecast == NULL)
		return false;
	Type t2_ptr_type( typecast->get_type());
	if (!t2_ptr_type.isPointer() && !t2_ptr_type.isArray())
		return false; // memory not allocated for array
	Type t2( t2_ptr_type.dereference().dereference());

	if (t1 == t2)
		return false;
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
 */
bool EXP05_A( const SgNode *node ) {
	const SgCastExp * cast = isSgCastExp(node);
	if(!cast)
		return false;
	/**
	 * Ignore compiler generated casts
	 */
	if(cast->get_file_info()->isCompilerGenerated())
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
	const SgExpression* exp = getAllocFunctionExpr( node);
	if (exp == NULL) return false;

	const SgNode* parent = node->get_parent();
	assert( parent != NULL);
	const SgCastExp* typecast = isSgCastExp( parent->get_parent());
	if (typecast != NULL) {
		Type alloc_type = Type( typecast->get_type()).stripInitialPointersReferencesAndArrays();
		if (alloc_type.isAnyChar())
			return false;
	}

	// Find a sizeof operator inside argument
	if (isSgSizeOfOp(exp)) return false;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree(const_cast<SgExpression*>( exp ), V_SgNode );
	for (Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		if (isSgSizeOfOp(*i))
			return false;
	}

	print_error(node, "EXP09-A", "malloc called using something other than sizeof()", true);
	return true;
}

/**
 * Do not cast away a volatile qualification
 *
 * We check cast expressions to make sure that if they don't posses a volatile
 * tag, then neither does the operand.
 *
 * \note GCC catches implicit casts, we just focus on explicit ones because
 * GCC assumes the programmer is right in those cases
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
		print_error(node, "EXP32-C", "Do not cast away a volatile qualification", true);
		return true;
	}

	return false;
}

/**
 * Ensure pointer is valid before dereferencing it
 * We only check the output of malloc, calloc, and realloc. Don't check other
 * potential null ptrs.
 *
 * If allocated memory is passed to a variable x, ensure that x's next usage is something like: if (x == NULL) ...
 * First we get the var it is assigned to, which might be an assignment exp eg
 * x = malloc(...), or a decl initializer, eg char* x = malloc(...);
 */
bool EXP34_C( const SgNode *node ) {
	const SgExpression* exp = getAllocFunctionExpr( node);
	if (exp == NULL) return false;

	const SgInitializedName* var = NULL;
	// The node in the function where the variable first gets referred to
	const SgVarRefExp* ref = NULL;

	const SgAssignOp* assignment
		= isSgAssignOp( findParentNodeOfType( node, V_SgAssignOp).first);
	if (assignment != NULL) {
		ref = isSgVarRefExp( assignment->get_lhs_operand());
		if (ref == NULL) return false; 
		// LHS is more complex than variable, it might be array ref
		// or struct member or pointer; we're only handling variables (for now)!!!
		var = getRefDecl( ref);
		assert( var != NULL);
		if (isTestForNullOp( ref)) return false;
	} else {
		const SgAssignInitializer* ass_init = isSgAssignInitializer( findParentNodeOfType( node, V_SgAssignInitializer).first);
		if (ass_init == NULL)
			return false; // malloc ptr not assigned.

		var = isSgInitializedName( ass_init->get_parent());
		assert( var != NULL);
	}

	// Find all var references in function after malloc
	Rose_STL_Container<SgNode *> nodes = getNodesInFn( node);
	Rose_STL_Container<SgNode *>::const_iterator i = nodes.begin();
	for (; i != nodes.end(); ++i ) {
		if (ref == NULL) break; // was initializer, all refs valid
		else if (ref == *i) {i++; break;} // var ref was malloc, start with next one
	}
	if (i == nodes.end()) return false;

	// Now to future variable references, find one with bad usage
	for (; i != nodes.end(); i = nextVarRef( nodes, i, var)) {
		const SgVarRefExp* i_ref = isSgVarRefExp(*i);
		assert( i_ref != NULL);
		if (var != getRefDecl( i_ref)) continue;

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
  violation |= EXP32_C(node);
  violation |= EXP34_C(node);
  return violation;
}
