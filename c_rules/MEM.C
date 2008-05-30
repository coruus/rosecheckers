/*
 *
 * Copyright (c) 2007 Carnegie Mellon University.
 * All rights reserved.

 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WSTRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "rose.h"
#include "utilities.h"
#include <algorithm>

bool hasAssignToVar(const SgNode* node,
	const SgVarRefExp* ref, /* ignore if NULL */
	bool after, /* if true, only items after ref, else before */
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
		// if (i != nodes.end()) std::cerr << "i: " << (*i)->get_parent()->unparseToString() << std::endl;

		if (isTestForNullOp(i_ref))
			continue;
		const SgAssignOp* next_assignment = isSgAssignOp(findParentNodeOfType(i_ref, V_SgAssignOp).first);
		if (next_assignment != NULL && isSgVarRefExp(next_assignment->get_lhs_operand()) && next_assignment->get_lhs_operand() == i_ref) {
			// we have an assignment to var, but must ensure var does
			// not appear on the RHS
		}

		//	if (next_assignment) std::cerr << "ass: " << next_assignment->unparseToString() << std::endl;
		//		std::cerr << "ref: " << i_ref->unparseToString() << std::endl;
		//		std::cerr << "var: " << var->unparseToString() << std::endl;
		(*violation) = true;
		return false;
	}
	return false;
}

// Checks to see if node is an assignment with var as the lhs
bool isAssignToVar( const SgNode *node, const SgVarRefExp *var) {
	const SgAssignOp *assignOp = isSgAssignOp(node);
	if (!assignOp) return false;
	const SgVarRefExp *lhsVar = isSgVarRefExp(assignOp->get_lhs_operand());
	assert(lhsVar);

	// Ensure that we are assigning to the pointer and not some other variable
	return (var->get_symbol()->get_name() == lhsVar->get_symbol()->get_name());
}

bool MEM01_A( const SgNode *node ) { // Store a new value in pointers immediately after free()
	if (!isCallOfFunctionNamed(node, "free")) return false;
	bool violation = false;

	// Figure out which variable is being freed
	const SgExpression *argExp = getFnArg(isSgFunctionRefExp(node), 0);
	assert(argExp);
	const SgVarRefExp *argVar = isSgVarRefExp(argExp);
	assert(argVar);
	bool global = isGlobalVar(argVar);

	// Pop up to the BasicBlock so that we can find the next line of code
	const SgFunctionCallExp *freeExp = isSgFunctionCallExp(node->get_parent());
	assert(freeExp);
	const SgExprStatement *freeExpr = isSgExprStatement(freeExp->get_parent());
	assert(freeExpr);
	const SgBasicBlock *freeBlock = isSgBasicBlock(freeExpr->get_parent());
	assert(freeBlock);

	// Iterate over the basic block until we find the position of the free
	// statement
	const SgStatementPtrList &stats = freeBlock->get_statements();
	SgStatementPtrList::const_iterator i = find(stats.begin(), stats.end(), freeExpr);
	// We should always find the free
	assert(i != stats.end());

	do {
		// Now go to the next statement and see what it is
		++i;

		// The free is allowed to be the last statement in a block only if
		// that block is a function definition and the variable is local
		// or if the increment of a for loop is an assignment to the variable
		if (i == stats.end()) {
			if (isSgFunctionDefinition(freeBlock->get_parent()) && !global)
				return false;
			const SgForStatement *forLoop = isSgForStatement(freeBlock->get_parent());
			if (forLoop && isAssignToVar(forLoop->get_increment(), argVar))
				return false;
			break;
		}

		// Return Statements are also OK, but only for local vars
		if (isSgReturnStmt(*i)) {
			if (!global)
				return false;
			break;
		}

		// Assignments to the pointer are OK
		const SgExprStatement *nextExpr = isSgExprStatement(*i);
		if(nextExpr && isAssignToVar(nextExpr->get_expression(), argVar))
			return false;
	} while (false);

	print_error(node, "MEM01_A", "Store a new value in pointers immediately after free()");

	return true;
}

bool MEM30_C( const SgNode *node ) { // Ensure that freed pointers are not reused
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


bool MEM(const SgNode *node) {
  bool violation = false;
  violation |= MEM01_A(node);
  violation |= MEM30_C(node);
  return violation;
}
