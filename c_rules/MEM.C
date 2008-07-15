/**
 * \file MEM.C
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
#include <algorithm>

/**
 * Checks to see if node is an assignment with var as the lhs and not in
 * the rhs
 */
bool isAssignToVar( const SgNode *node, const SgVarRefExp *var) {
	const SgAssignOp *assignOp = isSgAssignOp(node);
	if (!assignOp)
		return false;

	// Ensure that we are assigning to the variable in the LHS
	const SgVarRefExp *lhsVar = isSgVarRefExp(assignOp->get_lhs_operand());
	assert(lhsVar);
	if (getRefDecl(var) != getRefDecl(lhsVar))
		return false;

	// Ensure variable does not appear in RHS
	const Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree(const_cast< SgNode*>( node), V_SgVarRefExp);
	Rose_STL_Container<SgNode *>::const_iterator i = nodes.begin();
	Rose_STL_Container<SgNode *>::const_iterator end = nodes.end();
	SgVarRefExp *rhsVar;
	for (++i; i < end; i++) {
		rhsVar = isSgVarRefExp(*i);
		assert(rhsVar);
		if (getRefDecl(var) == getRefDecl(rhsVar))
			return false;
	}
	return true;
}

/**
 * \param node
 * \param ref ignored if NULL
 * \param after if true, only items afte ref, else before
 * \param var
 * \param violation
 */
bool hasAssignToVar(const SgNode* node,
	const SgVarRefExp* ref,
	bool after,
	const SgInitializedName* var,
	bool* violation) {

	const Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree(const_cast< SgNode*>( node), V_SgVarRefExp);

	Rose_STL_Container<SgNode *>::const_iterator i = nodes.begin();
	Rose_STL_Container<SgNode *>::const_iterator end = nodes.end();
	if (ref) {
		if (after) {
			i = find(i, end, ref) + 1;
			if (i == end) return false;
		} else end = find(i, end, ref);
	}
	if (i == nodes.end()) return false;

	// if i does not point to our var ref, move it to next one.
	const SgVarRefExp* i_ref = isSgVarRefExp(*i);
	if (i_ref == NULL || getRefDecl(i_ref) != var)
		i = nextVarRef(nodes, i, var);

	for (; i != end && i != nodes.end(); i = nextVarRef(nodes, i, var)) {
		const SgVarRefExp* i_ref = isSgVarRefExp(*i);
		assert(i_ref != NULL);

		if (isTestForNullOp(i_ref))
			continue;

		if (isAssignToVar(findParentNodeOfType(i_ref, V_SgAssignOp).first, i_ref)) {
			return true;
		} else {
			(*violation) = true;
			return false;
		}
	}
	return false;
}

/**
 * Store a new value in pointers immediately after free()
 */
bool MEM01_A( const SgNode *node ) {
	if (!isCallOfFunctionNamed(node, "free")) return false;
	//	bool violation = false;

	// Figure out which variable is being freed
	const SgExpression *argExp = getFnArg(isSgFunctionRefExp(node), 0);
	assert(argExp);
	const SgVarRefExp *argVar = isSgVarRefExp(argExp);
	if (!argVar)
		return false;
	bool longlifetime = isGlobalVar(argVar) || isStaticVar(argVar);
	/* Block where the variable is defined */
	const SgBasicBlock* defBlock = isSgBasicBlock(findParentNodeOfType(
		argVar->get_symbol()->get_declaration(),V_SgBasicBlock).first);

	// Pop up to the BasicBlock so that we can find the next line of code
	const SgStatement* nextStat = findInBlockByOffset(node,1);
	// block in which the free statement is enclosed
	const SgBasicBlock* block = isSgBasicBlock(findParentNodeOfType(node,V_SgBasicBlock).first);
	assert(block);

	while (nextStat == NULL) {
		// If the we're in a for-loop and imediately assign in the increment,
		// that is OK
		const SgForStatement *forLoop = isSgForStatement(block->get_parent());
		if (forLoop && isAssignToVar(forLoop->get_increment(), argVar))
			return false;
		// If this block is the one in which the variable is defined, that is
		// OK
		if ((block == defBlock) && !longlifetime)
			return false;
		// Pop up to the next block
		nextStat = findInBlockByOffset(block,1);
		block = isSgBasicBlock(findParentNodeOfType(block,V_SgBasicBlock).first);
		assert(block);
	}

	// Return Statements are also OK, but only for local vars
	if (isSgReturnStmt(nextStat) && (!longlifetime)) {
		return false;
	} else {
		// Assignments to the pointer are OK
		const SgExprStatement *nextExpr = isSgExprStatement(nextStat);
		if(nextExpr && isAssignToVar(nextExpr->get_expression(), argVar))
			return false;
	}

	print_error(node, "MEM01-A", "Store a new value in pointers immediately after free()", true);

	return true;
}

/**
 * Do not perform zero length allocations
 */
bool MEM04_A( const SgNode *node ) {
	const SgExpression *allocArg = removeImplicitPromotions(getAllocFunctionExpr(isSgFunctionRefExp(node)));
	if (!allocArg)
		return false;

	const SgInitializedName *var = getRefDecl(isSgVarRefExp(allocArg));
	const SgValueExp *val = isSgValueExp(allocArg);
	if (var) {
		const SgFunctionDefinition *fn = isSgFunctionDefinition((findParentNodeOfType(node,V_SgFunctionDefinition).first));
		const SgVarRefExp *ref = NULL;
		/**
		 * First try, just look for a check against NULL
		 */
		FOREACH_SUBNODE(fn, nodes1, i, V_SgVarRefExp) {
			ref = isSgVarRefExp(*i);
			assert(ref);
			if ((var == getRefDecl(ref)) && isTestForNullOp(ref)) {
				return false;
			}
		}
		/**
		 * If there isn't one, maybe the progammer has assigned this var to
		 * something sane and doesn't need to check
		 */
		FOREACH_SUBNODE(fn, nodes2, i, V_SgAssignOp) {
			const SgAssignOp *op = isSgAssignOp(*i);
			assert(op);
			ref = isSgVarRefExp(op->get_lhs_operand());
			if (!ref || (getRefDecl(ref) != var))
				continue;
			if (!isZeroVal(removeCasts(op->get_rhs_operand())))
				return false;
		}
		const SgAssignInitializer *init = isSgAssignInitializer(var->get_initptr());
		if(init && !isZeroVal(removeCasts(init->get_operand()))) {
			return false;
		}

	} else if (val && !isZeroVal(val)) {
		/** compile time constant that is 0 */
		return false;
	} else {
		/** sizeof or something else we can't handle */
		return false;
	}

	print_error(node,"MEM04-A", "Do not perform zero length allocations", true);
	return true;
}

/**
 * Ensure that the arguments to calloc() when multiplied can be represented as
 * a size_t
 */
bool MEM07_A( const SgNode *node ) {
	if (!isCallOfFunctionNamed(node, "calloc"))
		return false;
	const SgFunctionRefExp* fnRef = isSgFunctionRefExp(node);
	assert(fnRef);

	const SgExpression* nmembExp = getFnArg(fnRef, 0);
	const SgExpression* sizeExp = getFnArg(fnRef, 1);
	assert(nmembExp && sizeExp);

	const SgVarRefExp* nmembRef = isSgVarRefExp(nmembExp);
	size_t nmembVal;
	if (!nmembRef && !getSizetVal(nmembExp, &nmembVal))
		return false;
	const SgVarRefExp* sizeRef = isSgVarRefExp(sizeExp);
	size_t sizeVal;
	if (!sizeRef && !getSizetVal(sizeExp, &sizeVal))
		return false;

	if (nmembRef || sizeRef) {
		/* Search for the previous line, see if it is a check for overflow */
		/**
		 * \todo We need to find a more rigorous way to find checks for
		 * overflow
		 */
		const SgStatement *prevStat = findInBlockByOffset(node, -1);
		if (prevStat) {
			FOREACH_SUBNODE(prevStat, nodes, i, V_SgBinaryOp) {
				const SgBinaryOp *binOp = isSgBinaryOp(*i);
				assert(binOp);
				if (!(isSgEqualityOp(binOp)
					||isSgGreaterOrEqualOp(binOp)
					||isSgGreaterThanOp(binOp)
					||isSgLessOrEqualOp(binOp)
					||isSgLessThanOp(binOp)
					||isSgNotEqualOp(binOp)))
					continue;
				const SgVarRefExp *lhs = isSgVarRefExp(binOp->get_lhs_operand());
				const SgVarRefExp *rhs = isSgVarRefExp(binOp->get_rhs_operand());
				if((lhs && nmembRef && (getRefDecl(lhs)==getRefDecl(nmembRef)))
				|| (lhs && sizeRef  && (getRefDecl(lhs)==getRefDecl(sizeRef)))
				|| (rhs && nmembRef && (getRefDecl(rhs)==getRefDecl(nmembRef)))
				|| (rhs && sizeRef  && (getRefDecl(rhs)==getRefDecl(sizeRef))))
					return false;
			}
		}
	} else {
		if (nmembVal <= (std::numeric_limits<size_t>::max() / sizeVal))
			return false;
	}

	print_error(node, "MEM07-A", "Ensure that the arguments to calloc() when multiplied can be represented as a size_t", true);
	return true;
}

/**
 * Use realloc() only to resize dynamically allocated arrays
 */
bool MEM08_A( const SgNode *node ) {
	if (!isCallOfFunctionNamed(node, "realloc"))
		return false;

	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);

	const SgExpression *arg = removeImplicitPromotions(getFnArg(fnRef, 0));
	assert(arg);

	const SgCastExp* cast = isSgCastExp(node->get_parent()->get_parent());
	if (!cast)
		return false;

	if (cast->get_type() != arg->get_type()) {
		print_error(node, "MEM08-A", "Use realloc() only to resize dynamically allocated arrays", true);
		return true;
	}

	return false;
}

/**
 * Ensure that freed pointers are not reused
 *
 * \bug Need to check for conditional return statements
 */
bool MEM30_C( const SgNode *node ) {
	if (!isCallOfFunctionNamed( node, "free")) return false;

	// Get variable as first arg
	const SgVarRefExp* ref = isSgVarRefExp( getFnArg( isSgFunctionRefExp( node), 0));
	if (ref == NULL) return false;
	const SgInitializedName* var = getRefDecl( ref);
	assert(var != NULL);

	bool violation = false;
	bool assignmentFound = false;

	// check for existence of for loop
	const SgForStatement *forLoop = isSgForStatement(findParentNodeOfType(node, V_SgForStatement).first);
	if (forLoop) {
		// check loop body after free
		assignmentFound = hasAssignToVar( forLoop->get_loop_body(), ref, true, var, &violation);
		// check loop increment
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( forLoop->get_increment(), NULL, true, var, &violation);
		// check loop test
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( forLoop->get_test(), NULL, true, var, &violation);
		// check loop body up to the free
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( forLoop->get_loop_body(), ref, false, var, &violation);
	}
	// check for existence of while loop
	const SgWhileStmt *whileLoop = isSgWhileStmt(findParentNodeOfType(node, V_SgWhileStmt).first);
	if (!violation && whileLoop) {
		// check body of loop after call to free
		assignmentFound = hasAssignToVar( whileLoop->get_body(), ref, true, var, &violation);
		// check loop condition
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( whileLoop->get_condition(), NULL, true, var, &violation);
			// check loop body up to the free
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( whileLoop->get_body(), ref, false, var, &violation);
	}
	// check for existence of do-while loop
	const SgDoWhileStmt *doWhileLoop = isSgDoWhileStmt(findParentNodeOfType(node, V_SgDoWhileStmt).first);
	if (!violation && doWhileLoop) {
		// check body of loop after call to free
		assignmentFound = hasAssignToVar( doWhileLoop->get_body(), ref, true, var, &violation);
		// check loop condition
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( doWhileLoop->get_condition(), NULL, true, var, &violation);
			// check loop body up to the free
		if (!assignmentFound && !violation)
			assignmentFound = hasAssignToVar( doWhileLoop->get_body(), ref, false, var, &violation);
	}

	// Find usages of variable after that free in function body
	if (!violation) 
		hasAssignToVar( findParentNodeOfType( node, V_SgFunctionDefinition).first, ref, true, var, &violation);

	if (violation)
		print_error( node, "MEM30-C", "Do not access freed memory");
	return violation;
}

/**
 * Free dynamically allocated memory exactly once
 *
 * If the variable is passed by reference to a function which allocates memory this could throw a false positive.
 * If the variable is freed multiple times by a single loop this could throw a false negative.
 */
bool MEM31_C( const SgNode *node ) {
	if (!isCallOfFunctionNamed( node, "free")) return false;
	const SgVarRefExp* ref = isSgVarRefExp( getFnArg( isSgFunctionRefExp( node), 0));
		
	//	const SgVarRefExp* ref2 = NULL;

	if (ref == NULL) return false;
	const SgInitializedName* ref_var = getRefDecl( ref);
	//	const SgInitializedName* ref2_var = NULL;

	const SgFunctionDefinition* top = isSgFunctionDefinition( findParentNodeOfType( node, V_SgFunctionDefinition).first);
	const Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( const_cast< SgFunctionDefinition*>( top), V_SgNode);

	Rose_STL_Container<SgNode *>::const_iterator i = nodes.begin();
	Rose_STL_Container<SgNode *>::const_iterator end = nodes.end();

	while(*i != node) i++;
	i++;

	while(i != end) {
		/**
		 * Checking for return statements lowers the false positive rate by
		 * allowing conditional frees followed by returns
		 */
		if(isSgReturnStmt(*i))
			return false;
		if(isCallOfFunctionNamed(*i, "free")) {
			const SgVarRefExp* ref2 = isSgVarRefExp( getFnArg( isSgFunctionRefExp( *i), 0));
			const SgInitializedName* ref2_var = getRefDecl( ref2);

			if (ref_var == ref2_var) {
				print_error(node, "MEM31-C", "Free dynamically allocated memory exactly once.");
				return true;
			}
		}

		if(isSgAssignOp(*i) != NULL) {
			const SgVarRefExp *ref2 = isSgVarRefExp(isSgAssignOp(*i)->get_lhs_operand());
			const SgInitializedName* ref2_var = getRefDecl( ref2);

			if (ref_var == ref2_var)
				return false;
		}

		i++;
	}

	return false;
}

/**
 * Use the correct syntax for flexible array members
 */
bool MEM33_C( const SgNode *node ) {
	const SgClassDefinition* def = isSgClassDefinition(node);
	if (!def)
		return false;

	const SgVariableDeclaration* varDecl = isSgVariableDeclaration(def->get_members().back());
	assert(varDecl);

	if (varDecl->get_variables().size() != 1)
		return false;

	const SgInitializedName *varName = varDecl->get_variables().front();
	assert(varName);

	const SgArrayType *arrT = isSgArrayType(varName->get_type());
	if (!arrT)
		return false;

	const SgValueExp* arrSize = isSgValueExp(arrT->get_index());
	if (!arrSize)
		return false;

	if (isVal(arrSize,0) || isVal(arrSize,1)) {
		print_error(varDecl, "MEM33-C", "Use the correct syntax for flexible array members");
		return true;
	}
	return false;
}

bool MEM(const SgNode *node) {
  bool violation = false;
  violation |= MEM01_A(node);
  violation |= MEM04_A(node);
  violation |= MEM07_A(node);
  violation |= MEM08_A(node);
  violation |= MEM30_C(node);
  violation |= MEM31_C(node);
  violation |= MEM33_C(node);
  return violation;
}
