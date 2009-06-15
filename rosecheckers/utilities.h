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
 * \file utilities.h
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <list>
#include <string>
#include "rose.h"
#include "utilities-inline.h"
#include "categories.h"
#include "value.h"


/**
 * Do not use this function directly, use the findParentOfType macro
 */
const SgNode *findParentNodeOfType(const SgNode *node, VariantT t);

bool isCallOfFunctionNamed(const SgFunctionRefExp *node, const std::string &name);
const SgExpression *removeImplicitPromotions( const SgExpression *e );

/**
 * \return reference to ith argument of function reference. Dives through
 * typecasts.
 * \return NULL if no such parm
 */
const SgExpression* getFnArg(const SgFunctionRefExp* node, unsigned int i);
const SgExpression* getFnArg(const SgFunctionCallExp* fnCall, unsigned int i);

/**
 * Fills list with all nodes in enclosing function
 */
const Rose_STL_Container<SgNode*> getNodesInFn( const SgNode* node);

/**
 * \return iterator of next node that refers to same variable as ref.
 * \return nodes.end() if unsuccessful
 */
Rose_STL_Container<SgNode *>::const_iterator nextVarRef(const Rose_STL_Container<SgNode *>& nodes, Rose_STL_Container<SgNode *>::const_iterator i, const SgInitializedName* ref);

/**
 * \return true if function( ref) appears in code somewhere after ref
 */
bool isVarUsedByFunction(const char* function, const SgVarRefExp* ref);

const SgStatement * findInBlockByOffset(const SgNode *node, int delta);
const SgExpression* removeCasts(const SgExpression * expr);

const SgExpression* getAllocFunctionExpr(const SgFunctionRefExp *node);
SgValueExp* computeValueTree(SgValueExp* node);
int getScanfFormatString(const SgFunctionRefExp *node);
int getPrintfFormatString(const SgFunctionRefExp *node);

/**
 * Returns the number of bytes that an object of this type should take up
 */
size_t sizeOfType(const SgType *type);

/**
 * Use this to report errors and warnings
 */
void print_error(const SgNode* node, const char* rule, const char* desc,bool warning = false);

/****************************************************************************
 * Variable assignment functions                                            *
 ****************************************************************************/
bool varWrittenTo(const SgNode* var);
const SgInitializedName *getVarAssignedTo(const SgFunctionRefExp *fnRef, const SgVarRefExp **varRef_p);
bool valueVerified(const SgExpression *expr);
bool isCheckForZero(const SgStatement *stat, const SgVarRefExp *varRef);
/**
 * \return True if node is inside an expression that tests its value to see if
 * it is NULL
 */
bool isTestForNullOp(const SgNode* node);

/**
 * Checks to see if node is an assignment with var as the lhs and not in
 * the rhs
 */
bool isAssignToVar( const SgNode *node, const SgInitializedName *var);


/**
 * Given a node in a block, returns the node at the top of the block.
 *
 * \note This looks for a lot of blocks, but maybe not all of them?
 */
const SgNode *popBlock(const SgNode *node);


/**
 * Class to visit nodes following node in control flow
 */
class CFGVisitor {
private:
  /** Nodes that have already been visited */
  std::set<VirtualCFG::CFGNode> visited;
public:
  CFGVisitor() {}
  virtual ~CFGVisitor() {}
  /** Do something with this node, then visit its successors, in a bfs order */
  void visit(const VirtualCFG::CFGNode& node);
  /** Visit node's successors */
  void visitOthers(const VirtualCFG::CFGNode& node);
  /**
   * Do something with this node
   * /return true if visiting should continue
   */
  virtual bool doSomething(const SgNode* node) = 0; // returns true if visit should continue
};


/**
 * /param ref  Reference to a variable
 * /return list of succeeding variable references
 */
Rose_STL_Container<const SgVarRefExp*> next_var_references(const SgVarRefExp* ref);


/// C++ utiltiies

/* Returns a declaration of the given class */
const SgClassDeclaration *getClassDeclaration(const SgType* t);

/* Returns true if class is std */
bool isClassDeclaredInStdNamespace(const SgClassDeclaration *decl);

/* Returns true if class inherits from std::exception */
bool inheritsFromStdException(const SgClassDeclaration *cdecl);


#endif
