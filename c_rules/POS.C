/*
** Copyright (C) 2007-2009 by Carnegie Mellon University.
**
** @OPENSOURCE_HEADER_START@
**
** Use of the ROSE Checkers system and related source code is subject to
** the terms of the following license:
**
** GNU Public License (GPL) Rights pursuant to Version 2, June 1991
**
** NO WARRANTY
**
** ANY INFORMATION, MATERIALS, SERVICES, INTELLECTUAL PROPERTY OR OTHER
** PROPERTY OR RIGHTS GRANTED OR PROVIDED BY CARNEGIE MELLON UNIVERSITY
** PURSUANT TO THIS LICENSE (HEREINAFTER THE "DELIVERABLES") ARE ON AN
** "AS-IS" BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY
** KIND, EITHER EXPRESS OR IMPLIED AS TO ANY MATTER INCLUDING, BUT NOT
** LIMITED TO, WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE,
** MERCHANTABILITY, INFORMATIONAL CONTENT, NONINFRINGEMENT, OR ERROR-FREE
** OPERATION. CARNEGIE MELLON UNIVERSITY SHALL NOT BE LIABLE FOR INDIRECT,
** SPECIAL OR CONSEQUENTIAL DAMAGES, SUCH AS LOSS OF PROFITS OR INABILITY
** TO USE SAID INTELLECTUAL PROPERTY, UNDER THIS LICENSE, REGARDLESS OF
** WHETHER SUCH PARTY WAS AWARE OF THE POSSIBILITY OF SUCH DAMAGES.
** LICENSEE AGREES THAT IT WILL NOT MAKE ANY WARRANTY ON BEHALF OF
** CARNEGIE MELLON UNIVERSITY, EXPRESS OR IMPLIED, TO ANY PERSON
** CONCERNING THE APPLICATION OF OR THE RESULTS TO BE OBTAINED WITH THE
** DELIVERABLES UNDER THIS LICENSE.
**
** Licensee hereby agrees to defend, indemnify, and hold harmless Carnegie
** Mellon University, its trustees, officers, employees, and agents from
** all claims or demands made against them (and any related losses,
** expenses, or attorney's fees) arising out of, or relating to Licensee's
** and/or its sub licensees' negligent use or willful misuse of or
** negligent conduct or willful misconduct regarding the Software,
** facilities, or other rights or assistance granted by Carnegie Mellon
** University under this License, including, but not limited to, any
** claims of product liability, personal injury, death, damage to
** property, or violation of any laws or regulations.
**
** @OPENSOURCE_HEADER_END@
*/

/**
 * \file POS.C
 */

#include <list>
#include <string>
#include <vector>
#include <algorithm>
#include "rose.h"
#include "utilities.h"
#include <assert.h>

/**
 * Use the readlink() function properly
 *
 * \note, we do this by ensuring the next line after a readlink is comparison
 * of the return to -1
 */
bool POS30_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;
	if (!isCallOfFunctionNamed(fnRef, "readlink"))
		return false;

	const SgInitializedName *var = getVarAssignedTo(fnRef, NULL);
	/**
	 * First, let's see if we are checking the return value as part of a
	 * complicated expression
	 */
	const SgBinaryOp *compOp = findParentOfType(fnRef, SgEqualityOp);
	if (!compOp)
		compOp = findParentOfType(fnRef, SgNotEqualOp);
	/**
	 * If not, let's try to look for it on the next line
	 */
	if (!compOp && var) {
		const SgStatement *nextSt = findInBlockByOffset(fnRef, 1);
		if (nextSt) {
			FOREACH_SUBNODE(nextSt, nodes, i, V_SgBinaryOp) {
				const SgBinaryOp *iOp = isSgBinaryOp(*i);
				assert(iOp);
				/**
				 * We are looking for == or !=
				 */
				if (!(isSgEqualityOp(iOp) || isSgNotEqualOp(iOp)))
					continue;
				/**
				 * And one of the operands has to be our saved return value
				 */
				const SgVarRefExp *iVar = isSgVarRefExp(removeImplicitPromotions(iOp->get_lhs_operand()));
				if (!iVar)
					iVar = isSgVarRefExp(removeImplicitPromotions(iOp->get_rhs_operand()));
				if (iVar && (getRefDecl(iVar) == var)) {
					compOp = iOp;
					break;
				}
			}
		}
	}
	/**
	 * Now that we found our comparison, make sure it's being checked against
	 * the correct value
	 */
	if (compOp) {
		const SgExpression *expr = removeImplicitPromotions(compOp->get_lhs_operand());
		if (isVal(expr, -1))
			return false;
		expr = removeImplicitPromotions(compOp->get_rhs_operand());
		if (isVal(expr, -1))
			return false;
	}
	/**
	 * If for whatever reason we didn't find a good comparison, that's cause
	 * to worry
	 */
	print_error(node, "POS30-C", "Use the readlink() function properly");
	return true;;
}

/**
 * Do not use vfork (was overload operator&&)
 */
bool POS33_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "vfork")))
		return false;
	print_error( node, "POS33-C", "Do not use vfork()");
	return true;
}

/**
 * Do not call putenv() with auto var
 */
bool POS34_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "putenv")))
		return false;

	// ok, bail iff putenv's arg is a char* (not char[])
	const SgExpression *arg0 = getFnArg(fnRef, 0);
	assert( arg0 != NULL);

	// bail iff putenv's arg is a static variable
	const SgVarRefExp* var = isSgVarRefExp( arg0);
	if (var == NULL) return false; // we don't handle non-var putenv args

	const SgInitializedName* decl = getRefDecl(var);
	assert(decl);
	if (isGlobalVar(decl) || isStaticVar(decl))
		return false;

	/**
	 * \todo We only know how to deal with arrays for now
	 */
	if (!isSgArrayType(arg0->get_type())) {
	  
	  //TODO - Need to try to catch AssignOps first, and return if we do.

	  /**
	   * Catch assignments like char *var = &auto[0];
	   */

	  const SgAssignInitializer *init = isSgAssignInitializer(decl->get_initializer());
	  if(init != NULL) {
	    const SgExpression *rhs = init->get_operand();
	    if(isSgArrayType(rhs->get_type())) {
	      print_error( node, "POS34-C", "Do not call putenv() with an automatic variable");
	      return true;
	    }
	  }

	  return false;
	}	

	print_error( node, "POS34-C", "Do not call putenv() with an automatic variable");
	return true;
}

/**
 * Avoid race conditions while checking for the existence of a symbolic link
 */
bool POS35_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;

	bool need_fileno = false;
	if (isCallOfFunctionNamed(fnRef, "fopen"))
		need_fileno = true;
	else if (!isCallOfFunctionNamed(fnRef, "open"))
		return false;

	const SgExpression *fnExp = removeImplicitPromotions(getFnArg(fnRef,0));
	assert(fnExp);

	const SgVarRefExp *ref = isSgVarRefExp(fnExp);
	if (!ref)
		return false;
	const SgInitializedName *var = getRefDecl(ref);

	const SgInitializedName *fd = getVarAssignedTo(fnRef, NULL);
	if (!fd) {
		/**
		 * Ignoring the return value here is a violation of EXP12-C
		 * Or alternatively, we might just be returning the value
		 */
		return false;
	}

	const SgFunctionRefExp *iFn = NULL;
	const SgVarRefExp *iVar = NULL;

	bool lstat = false;
	bool after = false;
	bool fstat = false;

	FOREACH_SUBNODE(findParentOfType(node, SgFunctionDefinition), nodes, i, V_SgFunctionRefExp) { 
		iFn = isSgFunctionRefExp(*i);
		assert(iFn);

		if (iFn == fnRef) {
			after = true;
			continue;
		}

		if (!(isCallOfFunctionNamed(iFn, "lstat")
			||isCallOfFunctionNamed(iFn, "fstat")))
			continue;

		iVar = isSgVarRefExp(removeImplicitPromotions(getFnArg(iFn,0)));
		if (!iVar)
			continue;

		if (!after && isCallOfFunctionNamed(iFn, "lstat")) {
			if (getRefDecl(iVar) == var)
				lstat = true;
			continue;
		}

		if (after && need_fileno && !fstat
		&& isCallOfFunctionNamed(iFn, "fileno")) {
			if (getRefDecl(iVar) == fd) {
				fd = getVarAssignedTo(iFn, NULL);
				if (!fd) break;
				need_fileno = false;
			}
			continue;
		}

		if (after && isCallOfFunctionNamed(iFn, "fstat")) {
			if (getRefDecl(iVar) == fd)
				fstat = true;
			break;
		}
	}

	if (lstat && (!fstat || need_fileno)) {
//		if (isCallOfFunctionNamed(node, "fopen"))
//			print_error(node, "FIO32-C", "Do not perform operations on devices that are only appropriate for files");
//		else
			print_error(node, "POS35-C", "Avoid race conditions while checking for the existence of a symbolic link");
		return true;
	}

	return true;
}

/**
 * Observe correct revocation order while relinquishing privileges
 *
 * \note Since there's really no clean way to do this, we'll just traverse up
 * to the first SgBasicBlock above a setgid(), then find the previous
 * statement, and see if it contains a call to setuid()
 */
bool POS36_C( const SgNode *node ) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "setgid")))
		return false;

	const SgStatement *prevStat = findInBlockByOffset(fnRef, -1);
	if (!prevStat)
		return false;

	FOREACH_SUBNODE(prevStat, nodes, i, V_SgFunctionRefExp) {
		if (isCallOfFunctionNamed(isSgFunctionRefExp(*i), "setuid")) {
			print_error(node, "POS36-C", "Observe correct revocation order while relinquishing privileges");
			return true;
		}
	}
	return false;
}

bool POS_C(const SgNode *node) {
  bool violation = false;
  violation |= POS30_C(node);
  violation |= POS33_C(node);
  violation |= POS34_C(node);
  violation |= POS35_C(node);
  violation |= POS36_C(node);
  return violation;
}
