#ifndef UTILITIES_H
#define UTILITIES_H


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
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <list>
#include <string>
#include "rose.h"
#include "type.h"

//
// Questions about switch statements
//
bool switchHasDefault( const SgSwitchStatement *theSwitch );
bool getCaseValues( const SgBasicBlock *body, std::vector<int> &values ); // Return value indicates whether there was a default case.

//
// Questions having to do with function declarations.
//
const SgType *getArgType( const SgFunctionDeclaration *fdec, int n ); // note: first arg is arg 1, not arg 0.
int argCount( const SgFunctionDeclaration *fdec );
size_t CountLinesInFunction( const SgFunctionDeclaration *funcDecl );
bool isSingleArgFunctionDeclaration( const SgNode *node );

//
// Questions about function calls.
//
const SgFunctionSymbol *isCallOfFunctionNamed( const SgNode *node, const std::string &name );

// The two function templates below simply execute the same operations as the two non-template
// functions above, but will accept a sequence of names instead of a single name.
template <typename In>
const SgFunctionSymbol *isCallOfFunctionNamedAnyOf( const SgNode *node, In b, In e ) {
	while( b != e )
		if( SgFunctionSymbol *f = isCallOfFunctionNamed( *b++ ) )
			return f;
	return 0;
}

//
// Miscellaneous questions and manipulations.
//
bool isCompilerGeneratedNode( const SgNode *node );
bool isLocalDeclaration( const SgNode *node );
const SgExpression *removeImplicitIntegralPromotions( const SgExpression *e );
const SgExpression *removeImplicitIntegralOrFloatingPromotions( const SgExpression *e );

class IsFunctionDeclarationNamed : public std::unary_function<SgFunctionDeclaration *,bool> {
  public:
	IsFunctionDeclarationNamed( const std::string &id )
		: id_(id) {}
	bool operator ()( SgNode *node ) const {
		if( SgFunctionDeclaration *fd = isSgFunctionDeclaration( node ) ) {
			std::string fid = fd->get_name().getString();
			return fid == id_;
		}
		return false;
	}
  private:
	std::string id_;
};

std::pair<const SgNode *,size_t> findParentNodeOfType( const SgNode *start, int nodeType ); //XXX and do a version with a vector of nodeTypes, or just overload
const SgNode *getForStatmentTest( const SgNode *node );


void print_error(const SgNode* node, const char* rule, const char* desc,bool warning = false);


// Returns True if node is inside an expression that tests its value
// to see if it is NULL
bool isTestForNullOp(const SgNode* node);

// Returns reference to ith argument of function reference. Dives
// through typecasts. Returns NULL if no such parm
const SgExpression* getFnArg(const SgFunctionRefExp* node, int i);
const SgExpression* getFnArg(const SgFunctionCallExp* fnCall, int i);


// Fills list with all nodes in enclosing function
const Rose_STL_Container<SgNode*> getNodesInFn( const SgNode* node);

// Returns a variable's declaration, given a reference to that var
const SgInitializedName* getRefDecl(const SgVarRefExp* ref);

// Returns iterator of next node that refers to same variable as ref.
// Returns nodes.end() if unsuccessful
Rose_STL_Container<SgNode *>::const_iterator nextVarRef(const Rose_STL_Container<SgNode *>& nodes,
					       Rose_STL_Container<SgNode *>::const_iterator i,
					       const SgInitializedName* ref);

// Returns true if function( ref) appears in code somewhere after ref
bool isVarUsedByFunction(const char* function, const SgVarRefExp* ref);

#endif
