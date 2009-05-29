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
#include "utilities_cpp.h"
#include <fcntl.h>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

/**
 * Do not mistake sizeof( type*) for sizeof( type)
 *
 * We compare two types, one is inside the malloc, which prob looks like
 * X * sizeof( T1)
 * The other type is explicitly typecast:  (T2*) malloc(...)
 * Rule fails if T1 != T2 (usually because T1 == *T2)
 */
bool EXP01_C( const SgNode *node ) {
	const SgExpression* arg0 = getAllocFunctionExpr(isSgFunctionRefExp(node));
	if (arg0 == NULL)
		return false;

	const SgType* t1 = NULL;
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
		t1 = sizeOfVar->get_type();
		assert(t1 != NULL);
		break;
	}
	if (t1 == NULL)
		return false; // no sizeof() op in malloc call
	if (isSgArrayType(t1))
		return false; // explicit arrays are OK
//	Type t1(sg_t1);

	const SgNode* parent = node->get_parent();
	assert( parent != NULL);
	const SgCastExp* typecast = isSgCastExp( parent->get_parent());
	if (typecast == NULL)
		return false;
//	Type t2_ptr_type( typecast->get_type());
	const SgType *t2_ptr_type = typecast->get_type();
	if (!isSgPointerType(t2_ptr_type) && !isSgArrayType(t2_ptr_type))
		return false; // memory not allocated for array
	/**
	 * \bug ROSE is missing const derefence()
	 */
	const SgType *t2 = const_cast<SgType*>(t2_ptr_type)->dereference()->dereference();

	if (t1 == t2) {
		return false;
	}
	print_error( node, "EXP01-C", "Do not take the sizeof a pointer to determine the sizeof a type", true);
	return true;
}

/**
 * Do not perform byte-by-byte comparisons between structures
 */
bool EXP04_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "memcmp")))
		return false;
	const SgType *dstT = getFnArg(fnRef, 0)->get_type();
	assert(dstT);
	const SgType *srcT = getFnArg(fnRef, 1)->get_type();
	assert(srcT);
	if (isSgNamedType(dstT->findBaseType()) || isSgNamedType(srcT->findBaseType())) {
		print_error(node, "EXP04-C", "Do not perform byte-by-byte comparisons between structures", true);
		return true;
	}

	return false;
}

/**
 * Do not cast away a const qualification
 *
 * We check cast expressions to make sure that if they don't posses a const
 * tag, then neither does the operand.
 *
 * \bug Don't know how to get modifiers in example below:
 * var = (const char *)const_char_pointer;
 *
 * \note GCC catches implicit casts, we just focus on explicit ones because
 * GCC assumes the programmer is right in those cases
 *
 * \see EXP32_C
 */
bool EXP05_C( const SgNode *node ) {
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

	/**
	 * This allows things like a = (int) b, where b is const and is not
	 */
	const SgType *exprType = expr->get_type();
	if (!(isSgPointerType(exprType) || isSgArrayType(exprType)))
		return false;

	bool castIsConst = isConstType(cast->get_type());
	bool exprIsConst = isConstType(expr->get_type()->dereference());

	if(exprIsConst && !castIsConst) {
		print_error(node, "EXP05-C", "Do not cast away a const qualification", true);
		return true;
	}

	return false;
}

/**
 * Operands to the sizeof operator should not contain side effects 
 */
bool EXP06_C( const SgNode *node ) {
	const SgSizeOfOp *op = isSgSizeOfOp(node);
	if (!op)
		return false;

	bool violation = false;
	FOREACH_SUBNODE(op, nodes, i, V_SgExpression) {
		if(isSgFunctionCallExp(*i)
		|| isAnyAssignOp(*i)
		|| isSgMinusMinusOp(*i)
		|| isSgPlusPlusOp(*i)) {
			print_error(*i, "EXP06-C", "Operands to the sizeof operator should not contain side effects", true);
			violation = true;
		}
	}

	return violation;
}

/**
 * Ensure pointer arithmetic is used correctly
 *
 * \todo Finish writing this
 */
bool EXP08_C( const SgNode *node ) {
	// get inside of a *alloc, [], or pointer arith
	const SgExpression *pntrMath = getAllocFunctionExpr(isSgFunctionRefExp(node));
	if (!pntrMath) {
		const SgBinaryOp *op = isSgPntrArrRefExp(node);
		if (!op)
			op = isSgPntrArrRefExp(node);
		if (op)
			pntrMath = op->get_rhs_operand();
	}

	// Evaluate the number, make sure it has type byte 
	return false;
}

/**
 * Use sizeof to determine the size of a type or variable -jp
 *
 * If this alloc expr is being cast to a type char* or char[], bail, it's OK
 */
bool EXP09_C( const SgNode *node ) {
	const SgExpression* exp = getAllocFunctionExpr(isSgFunctionRefExp(node));
	if (exp == NULL)
		return false;

	const SgNode* parent = node->get_parent();
	assert( parent != NULL);

	const SgCastExp* typecast = isSgCastExp( parent->get_parent());
	if (typecast != NULL) {
		const SgType *alloc_type = typecast->get_type()->stripType(
			 SgType::STRIP_REFERENCE_TYPE
			|SgType::STRIP_POINTER_TYPE
			|SgType::STRIP_ARRAY_TYPE);
		if (isSgTypeChar(alloc_type)
		  ||isSgTypeSignedChar(alloc_type)
		  ||isSgTypeUnsignedChar(alloc_type))
			return false;
	}

	/**
	 * We should allow size_t or rsize_t arguments
	 */
	const SgType *t = stripModifiers(exp->get_type());
	if (isSgTypedefType(t)
	&& (isTypeSizeT(t) || isTypeRSizeT(t)))
		return false;

	// Find a sizeof operator inside argument
	if (isSgSizeOfOp(exp))
		return false;

	FOREACH_SUBNODE(exp, nodes, i, V_SgSizeOfOp) {
		return false;
	}

	print_error(node, "EXP09-C", "malloc called using something other than sizeof()", true);
	return true;
}

/**
 * Do not apply operators expecting one type to data of an incompatible type
 *
 * \see FLP33_C
 * \see INT07_C
 */
bool EXP11_C( const SgNode *node ) {
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

	/**
	 * \todo Find a better way to do this w/o unparseToString()
	 */
	const std::string lhsBase = stripModifiers(lhsSgType->findBaseType())->unparseToString();

	const SgExpression* castExpr = cast->get_operand();

	/// Exception b/c NULL is an int *
	if (isZeroVal(castExpr))
		return false;

	/// Exception b/c MEM02_C
	if (isTypeVoidStar(castExpr->get_type()))
		return false;

	/// Exception b/c strings are not pointers but can be assigned to them
	std::string castBase = stripModifiers(castExpr->get_type()->findBaseType())->unparseToString();
	if(castBase == "char*") castBase = "char";


	if(lhsBase != castBase) {
		print_error(cast, "EXP11-C", "Do not apply operators expecting one type to data of an incompatible type", true);
		return true;
	}

	return false;
}

/**
 * Do not ignore values returned by functions
 */
bool EXP12_C( const SgNode *node ) {
	const SgFunctionRefExp *ref = isSgFunctionRefExp(node);
	/** WHITE LIST */
	if ((ref == NULL)
	|| isCallOfFunctionNamed(ref, "close")
	|| isCallOfFunctionNamed(ref, "fclose")
	|| isCallOfFunctionNamed(ref, "free")
	|| isCallOfFunctionNamed(ref, "fflush")
	|| isCallOfFunctionNamed(ref, "memcpy")
	|| isCallOfFunctionNamed(ref, "memset")
	|| isCallOfFunctionNamed(ref, "memmove")
	|| isCallOfFunctionNamed(ref, "strcpy")
	|| isCallOfFunctionNamed(ref, "strncpy")
	|| isCallOfFunctionNamed(ref, "pthread_mutex_lock")
	|| isCallOfFunctionNamed(ref, "pthread_mutex_unlock")
	|| (getPrintfFormatString(ref) != -1))
		return false;
	const SgFunctionCallExp *fn = isSgFunctionCallExp(ref->get_parent());
	if (!fn)
		return false;

	if (isTypeVoid(fn->get_type()))
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
			if (isTypeVoid(isSgCastExp(parent)->get_type()))
				return false;
		} else if (isSgExprStatement(parent)) {
		  if (isSgSwitchStatement(parent->get_parent()))
		    return false;


			std::string msg = "Do not ignore values returned by functions: " + ref->unparseToString();
			print_error(node, "EXP12-C", msg.c_str(), true);
			return true;
		} else {
			return false;
		}
	}
}

/**
 * This is a helper class for EXP30 because we need to do a traversal within
 * sequence points
 */
class traverseSequencePoints: public AstPrePostProcessing {
	private:
	const SgNode * ignore_node;
	std::map<const SgInitializedName*, bool> seen;

	protected:
	virtual void preOrderVisit(SgNode *node) {
		/** Do nothing if we are in a subtree set off by a sequence point */
		if (ignore_node)
			return;

		/**
		 * Set a flag when entering a different sequence point
		 * \see C99 Annex C
		 */
		if(isSgConditionalExp(node)
		|| isSgFunctionCallExp(node)) {
			ignore_node = node;
			return;
		}

		/** We only care about variables that are being written to */
		const SgVarRefExp* var = isSgVarRefExp(node);
		if (!var || !varWrittenTo(var))
			return;

		/** 
		 * Trigger a violation if we write to the same variable more than
		 * once
		 */
		if (seen[getRefDecl(var)]) {
			violation = true;
		} else {
			seen[getRefDecl(var)] = true;
		}
	}

	virtual void postOrderVisit(SgNode *node) {
		/** Clear flag once we leave the subtree */
		if (ignore_node == node)
			ignore_node = NULL;
	}

	public:
	bool violation;

	traverseSequencePoints() {
		ignore_node = NULL;
		violation = false;
	}
};

	
/**
 * Do not depend on order of evaluation between sequence points
 *
 * \todo We could probably tune the sequence point matching better by looking
 * at all SgExpressions, not just SgExprStatements... we'd also need to
 * incorporate all of the sequence points from Annex C into the traversal
 * class above, right now many of them are implicit in the ExprStatement match
 */
bool EXP30_C( const SgNode *node ) {
  if (!isSgExprStatement(node) && !isSgForStatement(node))
    return false;

	traverseSequencePoints traversal;

	if(isSgExprStatement(node))
	  traversal.traverse(const_cast<SgNode *>(node));
	else if(isSgForStatement(node)) {
	  traversal.traverse(const_cast<SgExpression *>(isSgForStatement(node)->get_increment()));
	}

	if (traversal.violation) {
		print_error(node, "EXP30-C", "Do not depend on order of evaluation between sequence points");
		return true;
	} else {
		return false;
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
 * \see EXP05_C
 */
bool EXP32_C( const SgNode *node ) {
	if (isCompilerGeneratedNode(node))
		return false;

	const SgCastExp * cast = isSgCastExp(node);
	if(!cast)
		return false;

	const SgExpression *expr = cast->get_operand();
	assert(expr);

	bool castIsVolatile = isVolatileType(cast->get_type());
	bool exprIsVolatile = isVolatileType(expr->get_type()->dereference());

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
		if (findParentOfType(i_ref, SgSizeOfOp))
			continue;

		if (isTestForNullOp( i_ref)) return false;
		const SgAssignOp* next_assignment = findParentOfType( i_ref, SgAssignOp);
		if (next_assignment == NULL) {
			print_error( node, "EXP34-C", "Ensure pointer is valid before dereferencing it");
			return true;
		}
	}

	// var never referenced again. (bad, but not caught by us)
	return false;
}

/**
 * Do not convert pointers into more strictly aligned pointer types
 */
bool EXP36_C( const SgNode *node ) {
	const SgCastExp *cast = isSgCastExp(node);
	if(!cast)
		return false;
	const SgPointerType *lhsP = isSgPointerType(cast->get_type());
	if (!lhsP)
		return false;

	const SgType *rhsT = cast->get_operand()->get_type();
	assert(rhsT);
	const SgPointerType *rhsP = isSgPointerType(rhsT);
	if (!rhsP && isSgArrayType(rhsT)) {
		/**
		 * \bug ROSE is missing const dereference
		 */
		rhsP = isSgPointerType(const_cast<SgType *>(rhsT)->dereference());
	}
	if (!rhsP)
		return false;

	/*
	 * \bug ROSE is missing const dereference
	 */
	if (isTypeVoidStar(lhsP) || isZeroVal(removeCasts(cast)))
		return false;
	const unsigned int lhsSize = sizeOfType(const_cast<SgPointerType *>(lhsP)->dereference());
	const unsigned int rhsSize = sizeOfType(const_cast<SgPointerType *>(rhsP)->dereference());
	/**
	 * Allow casting to char's for pointer arith
	 */
	if (lhsSize == 1)
		return false;

	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( const_cast<SgNode*>(node), V_SgFunctionRefExp );
	Rose_STL_Container<SgNode *>::iterator i = nodes.begin();
	if(i != nodes.end()) {
	  if(isCallOfFunctionNamed(isSgFunctionRefExp(*i), "malloc"))
	    return false;
	}

	/**
	 * If we see a void * and the cast is implicit, then also flag
	 */
	if ((!isTypeVoidStar(rhsP) && (lhsSize > rhsSize))
	|| (isTypeVoidStar(rhsP) && isCompilerGeneratedNode(cast))) {
		print_error(cast->get_operand(), "EXP36-C", "Do not convert pointers into more strictly aligned pointer types");
		return true;
	}

	return false;
}

/**
 * Call functions with the arguments intended by the API
 */
bool EXP37_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "open")))
		return false;

	bool o_creat = false;
	FOREACH_SUBNODE(getFnArg(fnRef, 1), nodes, i, V_SgExpression) {
		/**
		 * If there's a variable present, we have no idea what the flags could
		 * be
		 */
		if (isSgVarRefExp(*i))
			return false;
		if (isVal(isSgValueExp(*i), O_CREAT))
			o_creat = true;
	}

	const SgFunctionCallExp *fnCall = isSgFunctionCallExp(fnRef->get_parent());
	assert(fnCall);
	int numArgs = fnCall->get_args()->get_expressions().size();
	if ((o_creat && (numArgs == 3))
	|| (!o_creat && (numArgs == 2)))
		return false;

	print_error(fnCall, "EXP37-C", "Call functions with the arguments intended by the API");
	return true;
}




/*************
 * CPP Rules *
 *************/

// Note:  As written, these tests catch template declarations only if instantiated.

//XXX other casting advice, not reint if const, etc.
bool EXP00_CPP( const SgNode *node ) { // Do not use C-style casts
	//XXX This doesn't work when the type being casted to is a class. X(12) or (X)12, for instance.
	//XXX ROSE seems to recognize only reinterpret_cast as new; others are old.
	return false; //XXX Disabled due to false positives
	bool result = false;
	if( const SgCastExp *cast = isSgCastExp( node ) ) {
		switch( cast->get_cast_type() ) {
		case SgCastExp::e_unknown:
		case SgCastExp::e_default:
			break;
		case SgCastExp::e_C_style_cast:
			// Note that a cast node might be an impicit promotion.
			if( !isCompilerGeneratedNode( node ) ) {
			  print_error(node, "EXP00-CPP", "Avoid old-style casts.", false);
				result = true;
			}
			break;
		case SgCastExp::e_const_cast:
		case SgCastExp::e_static_cast:
		case SgCastExp::e_dynamic_cast:
		case SgCastExp::e_reinterpret_cast:
			break;
		}
	}
	//XXX probably should check for "call" of a ctor here...
	return result;
}

bool EXP02_CPP( const SgNode *node ) { // Do not overload the logical AND and OR operators
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const std::string n = fd->get_name().getString();
		if( n == "operator&&" || n == "operator||" ) {
		  print_error(fd, "EXP02-CPP", "Overloaded && or || operator.", false);
			return true;
		}
	}
	return false;
}

bool EXP03_CPP( const SgNode *node ) { // Do not overload the & operator
	// Note this is for the unary & operator only, not binary.
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const std::string n = fd->get_name().getString();
		if( n == "operator&" ) {
			int args = argCount( fd );
			bool isMember = isSgMemberFunctionDeclaration( fd );
			if( (isMember && args == 0) || (!isMember && args == 1) ) {
			  print_error(fd, "EXP03-CPP", "Overloaded unary & operator.", false);
				return true;
			}
		}
	}
	return false;
}

bool EXP04_CPP( const SgNode *node ) { // Do not overload the comma operator
	if( const SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
		const std::string n = fd->get_name().getString();
		if( n == "operator," ) {
		  print_error(fd, "EXP04-CPP", "Overloaded , operator.", false);
			return  true;
		}
	}
	return false;
}


/*
bool EXP08_CPP( const SgNode *node ) { // A switch statement should have a default clause unless every enumeration value is tested
	// Synopsis:  If the switch expression has enum type, get the values of all the enumerators and ensure
	// that each value is used by a case label.  (Note that we are going by the enum and label values, not names.)
	// If that test fails, check for a default.

	bool result = false;

	if( const SgSwitchStatement *s = isSgSwitchStatement( node ) ) {
		const SgStatement *stat = s->get_item_selector();
		ROSE_ASSERT( stat );
		const SgExprStatement *estat = isSgExprStatement( stat );
		//XXXINFO it seems that expression statements (and returns?) no longer have a pointer to a wrapper class.
		//XXXINFO also, not all valid switch syntaxes are handled by ROSE, like "switch( x ) while( a ) case 1: a = 12;"
		const SgExpression *expr = estat->get_expression();
		expr = removeImplicitIntegralPromotions( expr );
		SgType et = *(expr->get_type());
		if( const SgEnumDeclaration *edecl = et.getEnumDeclaration() ) {
			// Get the set of enumerator values, and the set of case label values, and see if
			// the intersection is complete.  If not, check for a default label.
			std::vector<int> evalues; // values of enumerators
			getEnumeratorValues( edecl, evalues );
			const SgBasicBlock *body = s->get_body();
			std::vector<int> cvalues; // values of case labels
			bool defaultExists = getCaseValues( body, cvalues );
			std::sort( evalues.begin(), evalues.end() );
			std::sort( cvalues.begin(), cvalues.end() ); // this one can't have duplicates (if the compiler's working!)

			size_t original_esize = evalues.size();
			evalues.erase( std::unique( evalues.begin(), evalues.end() ), evalues.end() );
			if( original_esize != evalues.size() ) {
				// duplicate enumerator values
				result = true;
				print_error(node, "EXP08-CPP", "Switching over an enum that has duplicate enumerator values.", true);
			}

			std::vector<int> difference; // temp for set operation results
			std::set_difference( cvalues.begin(), cvalues.end(),
						evalues.begin(), evalues.end(), back_inserter(difference) );
			if( !difference.empty() ) {
				// there are cases that do not correspond to enumerator
				result = true;
				const std::string msg = "Cases for values that do not correspond to any enumerator value: " + utostring( difference.size() );
				print_error(node, "EXP08-CPP", msg.c_str(), true);
			}
			if( evalues.size() > cvalues.size() ) {
				// fewer case labels than enumerators
				if( !defaultExists ) {
					result = true;
					print_error(node, "EXP08-CPP", "Not all enumerator values have corresponding cases.", false);
				}
			}
			else if( evalues.size() == cvalues.size() ) {
				// same size, see if the values match up
				if( !std::equal( evalues.begin(), evalues.end(), cvalues.begin() ) ) {
					result = true;
					print_error(node, "EXP08-CPP", "Case labels do not match enumerator values.", false);
				}
			}
		}
	}
	return result;
}

*/


/*XXXXXXXXXXXXXXXXXXXXXXXXX BROKEN
bool EXP09_CPP( const SgNode *node ) { // Treat relational and equality operators as if they were nonassociative
	// Here, we examine only predefined relational and equality operators, because who knows what an
	// overloaded operator might intend?
	if( const SgBinaryOp *bop = isSgBinaryOp( node ) ) {
		if( isEqRelop( bop ) ) {
			const SgExpression *lhs = bop->get_lhs_operand();
			lhs = removeImplicitIntegralOrFloatingPromotions( lhs );
			const SgExpression *rhs = bop->get_rhs_operand();
			rhs = removeImplicitIntegralOrFloatingPromotions( rhs );
			if( isEqRelop( lhs ) || isEqRelop( rhs ) ) {
			  print_error(node, "EXP09-CPP", "Associative treatment of equality or relational operators.", false);
				return true;
			}
		}
	}
	return false;
}
*/

bool EXP10_CPP( const SgNode *node ) { // Prefer the prefix forms of ++ and --.
	return false; //XXXXXXXXXXXX this needs work
	const SgUnaryOp *uop = 0;
	if( (uop = isSgPlusPlusOp( node )) || (uop = isSgMinusMinusOp( node )) ) {
		// Predefined ++ or --.
	}
	else if( const SgFunctionCallExp *fcall = isSgFunctionCallExp( node ) ) {
		// Function call; see if user-defined ++ or --.
		bool postfix = false;
		const SgExpression *fexpr = fcall->get_function();
		const size_t numArgs = fcall->get_args()->get_expressions().size();
		if( const SgFunctionRefExp *fref = isSgFunctionRefExp( fexpr ) ) {
			const SgFunctionSymbol *fsym = fref->get_symbol();
			const SgFunctionDeclaration *fdecl = fsym->get_declaration();
			const std::string n = fdecl->get_name().getString();
			if( n == "operator++" || n == "operator--" ) {
				if( fref )
					postfix = (numArgs == 2);
				const SgMemberFunctionRefExp *mfref = isSgMemberFunctionRefExp( fexpr );
				if( !mfref ) {
					const SgBinaryOp *bop = 0;
					if( (bop = isSgArrowExp( fexpr )) || (bop = isSgDotExp( fexpr )) ) {
						const SgExpression *rhs = bop->get_rhs_operand();
						if( (mfref = isSgMemberFunctionRefExp( rhs )) != 0 )
							postfix = numArgs == 1;
					}
				}
				if( postfix ) {
					// Postfix use of user-defined ++ or --.  Issue print_error if not part of a
					// larger expression.
					const SgNode *parent = fcall->get_parent();
					if( !isSgExpression( parent ) ) {
					  print_error(node, "EXP10-CPP", "Unnecessary use of postfix increment or decrement.", true);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool EXP36_CPP( const SgNode *node ) { // Do not cast or delete pointers to incomplete classes
	//XXX problems with determingin whether a declaration is complete or incomplete.
	//XXX punt for now.
	return false; //XXX
	if( const SgCastExp *cexp = isSgCastExp( node ) ) {
		const SgExpression *expr = cexp->get_operand();
		if( expressionIsPointerToIncompleteClass( expr ) ) {
		  print_error(node, "EXP36-CPP", "Cast involving pointer to incomplete class.", false);
			return true;
		}
	}
	else if( const SgDeleteExp *dexp = isSgDeleteExp( node ) ) {
		if( expressionIsPointerToIncompleteClass( dexp->get_variable() ) ) {
			//if( isSgThisExp(dexp->get_variable()) ) return false; //XXX hack
		  print_error(node, "EXP36-CPP", "Deletion of pointer to incomplete class.", false);
			return true;
		}
	}
	return false;
}

void handleCallToVirtualMemberOfThisClass( const SgFunctionCallExp *fcall, const SgClassDefinition *cdtorClassDef ) {
	const SgExpression *function = fcall->get_function();
	const SgMemberFunctionRefExp *mfre = 0;
	const SgBinaryOp *bop = 0;
	if( (mfre = isSgMemberFunctionRefExp( function )) != 0 )
		/* nothing */;
	else if( (bop = isSgArrowExp( function )) || (bop = isSgDotExp( function )) ) {
		const SgExpression *lhs = bop->get_lhs_operand();
		if( isSgThisExp( lhs ) ) {
			const SgExpression *rhs = bop->get_rhs_operand();
			mfre = isSgMemberFunctionRefExp( rhs );
		}
	}
	//if( SgPointerDerefExp *ptrderef = isSgPointerDerefExp( function ) )
	//	std::cout << "PTR DEREF" << fcall->get_file_info()->get_line() << std::endl;
	if( mfre ) {
		const SgMemberFunctionSymbol *mfs = mfre->get_symbol();
		const SgMemberFunctionDeclaration *mfd = mfs->get_declaration();
		const SgClassDefinition *functionCallClassDef = mfd->get_class_scope();
		bool isVirtual = mfd->get_functionModifier().isVirtual();
		if(isVirtual) {
			//XXX not quite right:  non-virtual call of virtual incorrectly flagged.
			//XXX How to find out if the function name is qualified?  These do nothing:
			//int vc = mfre->get_virtual_call();
			//int nq = mfre->get_need_qualifier();
			// XXXcan't seem to find a use of SgQualifiedName anywhere either...
			if( functionCallClassDef == cdtorClassDef ) {
			  print_error(fcall, "EXP38-CPP", "Calling base class virtual in constructor or destructor", false);
			}
			else if( isBaseOf( cdtorClassDef, functionCallClassDef ) ) {
			  print_error(fcall, "EXP38-CPP", "Calling base class virtual in constructor or destructor", false);
			}
		}
	}
}


/*
bool EXP38_CPP( const SgNode *node ) { // Avoid calling your own virtual functions in constructors and destructors.
	if( const SgFunctionDefinition *fdef = isSgFunctionDefinition( node ) ) {
		const SgFunctionDeclaration *fdec = fdef->get_declaration();
		if( fdec->get_specialFunctionModifier().isConstructor() || fdec->get_specialFunctionModifier().isDestructor() ) {
			if( const SgMemberFunctionDeclaration *mfdec = isSgMemberFunctionDeclaration( fdec ) ) {
				// get the type of the class to which ctor or dtor belongs
				const SgClassDefinition *cdef = mfdec->get_class_scope();
				const SgClassDeclaration *cdecl = cdef->get_declaration();
				//SgClassType *ctype = cdecl->get_type();
				const SgBasicBlock *body = fdef->get_body();
				//SgStatementPtrList &stats = body->get_statements();
				FunctionBodyTraversal bt( cdef );
				bt.traverse( const_cast<SgBasicBlock *>(body), postorder );
			}
		}
	}
	return false;
}
*/



/***************************
 * Violation checking code *
 ***************************/

bool EXP_C(const SgNode *node) {
  bool violation = false;
  violation |= EXP01_C(node);
  violation |= EXP04_C(node);
  violation |= EXP05_C(node);
  violation |= EXP06_C(node);
  violation |= EXP08_C(node);
  violation |= EXP09_C(node);
  violation |= EXP11_C(node);
  violation |= EXP12_C(node);
  violation |= EXP30_C(node);
  violation |= EXP32_C(node);
  violation |= EXP34_C(node);
  violation |= EXP36_C(node);
  violation |= EXP37_C(node);
  return violation;
}

/// C++ checkers

bool EXP_CPP(const SgNode *node) {
  bool violation = false;
  violation |= EXP00_CPP(node);
  violation |= EXP02_CPP(node);
  violation |= EXP03_CPP(node);
  violation |= EXP04_CPP(node);
  violation |= EXP10_CPP(node);
  violation |= EXP36_CPP(node);
  return violation;
}

