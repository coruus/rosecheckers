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
#include <map>
#include <algorithm>
#include <functional>
#include "rose.h"
#include "utilities.h"

const SgType *getArgType( const SgFunctionDeclaration *fdec, int n ) { // note: fisrt arg is arg 1, not arg 0
	const SgFunctionParameterList *plist = fdec->get_parameterList();
	const SgInitializedNamePtrList &parms = plist->get_args();
	if( parms.size() < n || n <= 0 )
		return 0;
	SgInitializedNamePtrList::const_iterator i = parms.begin();
	std::advance( i, n-1 );
	const SgInitializedName *arg = *i;
	const SgType *argType = arg->get_type();
	return argType;
}

int argCount( const SgFunctionDeclaration *fdec ) {
	const SgFunctionParameterList *plist = fdec->get_parameterList();
	const SgInitializedNamePtrList &parms = plist->get_args();
	return parms.size();
}

bool isSingleArgFunctionDeclaration( const SgNode *node ) {
	// Note: result is single arg function or function that has defaults for trailing arguments
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) {
		const SgFunctionParameterList *plist = fdec->get_parameterList();
		const SgInitializedNamePtrList &parms = plist->get_args();
		switch( parms.size() ) {
		case 0: // no args
			return false;
		case 1:
			return true;
		default: // check to see if 2nd arg on have default inits
			SgInitializedNamePtrList::const_iterator i = parms.begin();
			return (*++i)->get_initptr() != 0; // need to check just 2nd arg...
		}
	}
	return false;
}

bool isZeroArgFunctionDeclaration( const SgNode *node ) {
	// Note: result is single arg function or function that has defaults for trailing arguments
	if( const SgFunctionDeclaration *fdec = isSgFunctionDeclaration(node) ) {
		const SgFunctionParameterList *plist = fdec->get_parameterList();
		const SgInitializedNamePtrList &parms = plist->get_args();
		switch( parms.size() ) {
		case 0: // no args
			return true;
		default: // check to see if 2nd arg on have default inits
			SgInitializedNamePtrList::const_iterator i = parms.begin();
			return (*i)->get_initptr() != 0; // need to check just 1st arg...
		}
	}
	return false;
}


bool isLocalDeclaration( const SgNode *node ) {
	if( node )
		do {
			if( isSgFunctionDefinition( node ) )
				return true;
		} while( node = node->get_parent() );
	return false;
}

bool isMemberStatement( const SgNode *node ) { //XXXXXXXXXXXXXX This doesn't work yet...
	if( const SgStatement *stat = isSgStatement( node ) ) {
		if( const SgScopeStatement *scope = stat->get_scope() ) {
			std::cout << "\tSCOPE:  " << scope->get_qualified_name().getString();
			if( const SgSymbolTable *tab = scope->get_symbol_table() ) {
			//	std::cout << "\ttable name: " << tab->get_name().getString();
			}
		}
	}
	return false;
}

const SgExpression *removeImplicitIntegralPromotions( const SgExpression *e ) {
	Type t( e->get_type() );
	if( t.isInt() || t.isUnsignedInt() ) {
		if( const SgCastExp *cast = isSgCastExp( e ) ) {
			if( isCompilerGeneratedNode( cast ) ) { // implicit promotions seem to be implemented as casts
				e = cast->get_operand();
			}
		}
	}
	return e;
}

const SgExpression *removeImplicitIntegralOrFloatingPromotions( const SgExpression *e ) {
	Type t( e->get_type() );
	if( t.isInt() || t.isUnsignedInt() || t.isFloat() || t.isDouble() ) {
		if( const SgCastExp *cast = isSgCastExp( e ) ) {
			if( isCompilerGeneratedNode( cast ) ) {
				e = cast->get_operand();
			}
		}
	}
	return e;
}

void getEnumeratorValues( const SgEnumDeclaration *edecl, std::vector<int> &values ) {
	values.clear();
	const SgInitializedNamePtrList &enums = edecl->get_enumerators();
	int enumeratorValue = -1; // so first value will default to 0
	for( SgInitializedNamePtrList::const_iterator i = enums.begin(); i != enums.end(); ++i ) {
		const SgInitializer *init = (*i)->get_initializer();
		if( !init )
			++enumeratorValue;
		else {
			const SgAssignInitializer *ainit = isSgAssignInitializer( init );
			const SgExpression *init_i = ainit->get_operand_i();
			const SgIntVal *intval = isSgIntVal( init_i );
			enumeratorValue = intval->get_value();
		}
		values.push_back( enumeratorValue );
	}
}

// Note that return value indicates whether there was a default case.
bool getCaseValues( const SgBasicBlock *body, std::vector<int> &values ) {
	bool sawDefault = false;
	values.clear();
	const SgStatementPtrList &stats = body->get_statements();
	for( SgStatementPtrList::const_iterator i = stats.begin(); i != stats.end(); ++i ) {
		if( const SgCaseOptionStmt *caseopt = isSgCaseOptionStmt( *i ) ) {
			const SgExpression *key = caseopt->get_key();
			const SgIntVal *keyval = isSgIntVal( key );
			values.push_back( keyval->get_value() );
		}
		else if( const SgDefaultOptionStmt *defaultopt = isSgDefaultOptionStmt( *i ) ) {
			sawDefault = true;
		}
	}
	return sawDefault;
}

bool isEqRelop( const SgNode *e ) {
	return
		isSgEqualityOp( e ) ||
		isSgGreaterOrEqualOp( e ) ||
		isSgGreaterThanOp( e ) ||
		isSgLessOrEqualOp( e ) ||
		isSgLessThanOp( e ) ||
		isSgNotEqualOp( e );
}

const SgFunctionSymbol *isCallOfFunctionNamed( const SgNode *node, const std::string &name ) { 
	if( const SgFunctionRefExp *f = isSgFunctionRefExp(node)) {
		const SgFunctionSymbol *sym = f->get_symbol();
		if( sym->get_name().getString() == name ) //XXX what about qualified names?
			return sym;
	}
	return 0;
}

std::pair<const SgNode *,size_t> findParentNodeOfType( const SgNode *start, int nodeType ) { //XXX and do a version with a vector of nodeTypes, or just overload
	size_t depth = 0;
	const SgNode *parent = start->get_parent();
	for( ; parent; parent = parent->get_parent() ) {
		++depth;
		if( parent->variantT() == nodeType )
			break;
	}
	return std::make_pair(parent,depth);
}

const SgNode *getForStatmentTest( const SgNode *node ) {
	if( const SgForStatement *forStatement = isSgForStatement(node) )
		return forStatement->get_test();
	else
		return 0;
}

bool isCompilerGeneratedNode( const SgNode *node ) {
	if( const Sg_File_Info *info = node->get_file_info() )
		if( info->isCompilerGenerated() )
			return true;
	return false;
}


bool isInlineFunctionDeclaration( const SgNode *node ) {
	if( const SgFunctionDeclaration *fdecl = isSgFunctionDeclaration( node ) ) {
		const SgFunctionModifier mod = fdecl->get_functionModifier();
		return mod.isInline();
	}
	return false;
}

size_t CountLinesInFunction( const SgFunctionDeclaration *funcDecl ) {
	if( const SgFunctionDefinition *funcDef = funcDecl->get_definition() ) {
		const Sg_File_Info *start = funcDef->get_body()->get_startOfConstruct();
		const Sg_File_Info *end = funcDef->get_body()->get_endOfConstruct();
		int lineS = start->get_line();
		int lineE = end->get_line();
		int loc_actual = lineE-lineS;
		return loc_actual > 0 ? loc_actual : 0;
	}
	else
		return 0;
}

bool switchHasDefault( const SgSwitchStatement *theSwitch ) {
	if( const SgBasicBlock *block = theSwitch->get_body() ) {
		const SgStatementPtrList &stmts = block->get_statements();
		for( SgStatementPtrList::const_iterator i = stmts.begin(); i != stmts.end(); ++i )
			if( isSgDefaultOptionStmt( *i ) )
				return true;
	}
	return false;
}


void print_error(const SgNode* node, const char* rule, const char* desc, bool warning) {
  const Sg_File_Info* fi = node->get_file_info();
  std::string filename = fi->get_filename();
  size_t found = filename.rfind("/");
  if( found != std::string::npos)
    filename.erase( 0, found+1);
  std::cerr << filename << ':' << fi->get_line() // don't use << ':' << fi->get_col(), not flymake-compliant 
	    << ": " << (warning ? "warning" : "error")
	    << ": " << rule << ": " << desc << std::endl;
}


// Returns True if node is inside an expression that tests its value
// to see if it is NULL
bool isTestForNullOp(const SgNode* node) {
  if (node == NULL) return false;
  const SgNode* parent = node->get_parent();
  assert(parent != NULL);
    // std::cerr << parent->class_name() << std::endl;
  if (isSgEqualityOp( parent)) return true; // if (expr == ...
  else if (isSgNotEqualOp( parent)) return true; // if (ptr != ...
  else if (isSgCastExp( parent)) {
    const SgCastExp* cast = isSgCastExp( parent);
    assert(cast != NULL);
    if (isSgTypeBool( cast->get_type())) return true;  // if (ptr) ...
  }

  const SgAssignOp* assignment = isSgAssignOp( parent);
  return (assignment != NULL) ? isTestForNullOp( assignment) : false;
}


// Returns reference to ith argument of function reference. Dives
// through typecasts. Returns NULL if no such parm
const SgExpression* getFnArg(const SgFunctionRefExp* node, int i) {
  if (node == NULL) return NULL;

  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  assert( fnRef != NULL);
  const SgFunctionCallExp *fnCall = isSgFunctionCallExp( fnRef->get_parent());
  assert( fnCall != NULL);
  const SgExprListExp* fnArgs = fnCall->get_args();
  assert( fnArgs != NULL);
  Rose_STL_Container<SgExpression*>::const_iterator iterator = fnArgs->get_expressions().begin();
  for (int j = 0; j < i; j++) iterator++;
  const SgExpression* fnArg = *iterator;
  assert( fnArg != NULL);
  const SgCastExp* castArg = isSgCastExp( fnArg);
  return (castArg != NULL) ? castArg->get_operand() : fnArg;
}


// Fills list with all nodes in enclosing function
const Rose_STL_Container<SgNode*> getNodesInFn( const SgNode* node) {
  const SgFunctionDefinition* block
    = isSgFunctionDefinition( findParentNodeOfType( node,
						    V_SgFunctionDefinition).first);
  assert( block != NULL);
  return NodeQuery::querySubTree( const_cast< SgFunctionDefinition*>( block),
				  V_SgVarRefExp);
}


// Returns a variable's declaration, given a reference to that var
const SgInitializedName* getRefDecl(const SgVarRefExp* ref) {
  if (ref == NULL) return NULL;
  const SgVariableSymbol* sym = ref->get_symbol();
  assert(sym != NULL);
  return sym->get_declaration();
}


// Returns iterator of next node that refers to same variable as ref.
// Returns nodes.end() if unsuccessful
Rose_STL_Container<SgNode *>::const_iterator nextVarRef(const Rose_STL_Container<SgNode *>& nodes,
					       Rose_STL_Container<SgNode *>::const_iterator i,
					       const SgInitializedName* var) {
  for (++i; i != nodes.end(); ++i ) {
    const SgVarRefExp* ref = isSgVarRefExp(*i);
    assert( ref != NULL);
    const SgInitializedName* ref_var = getRefDecl( ref);
    if (ref_var == var) return i;
  }
  return nodes.end();
}

// Returns true if function( ref) appears in code somewhere after ref
bool isVarUsedByFunction(const char* function, const SgVarRefExp* ref) {
  if (ref == NULL) return false;
  const SgInitializedName* var = getRefDecl( ref);
  assert(var != NULL);

  // Find reference in containing function's node list
  Rose_STL_Container<SgNode *> nodes = getNodesInFn( ref);
  Rose_STL_Container<SgNode *>::const_iterator i;
  for (i = nodes.begin(); i != nodes.end(); ++i ) if (*i == ref) break;
  assert(i != nodes.end());

  // Now to future variable references, find one with bad usage
  for (i = nextVarRef( nodes, i, var); i != nodes.end();
       i = nextVarRef( nodes, i, var)) {
    const SgFunctionCallExp* fn
      = isSgFunctionCallExp( findParentNodeOfType( *i, V_SgFunctionCallExp).first);
    if (fn == NULL) continue;
    const SgFunctionRefExp* fn_ref = isSgFunctionRefExp( fn->get_function());
    assert( fn_ref != NULL);

    if (isCallOfFunctionNamed( fn_ref, function))
      return true;
  }

  return false;
}
