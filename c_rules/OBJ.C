/**
 * \file OBJ.C
 *
 * Copyright (c) 2009 Carnegie Mellon University.
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
#include <list>
#include <string>
#include "rose.h"
#include "utilities_cpp.h"


bool OBJ_CPP(const SgNode *node) {
  bool violation = false;
  return violation;
}


class IsPublicData {
  public:
	IsPublicData()
		: foundPublic_(false), lastPublicVarDecl_(0) {}
	bool foundPublic() const
		{ return foundPublic_; }
	const SgVariableDeclaration *publicDeclaration() const
		{ return lastPublicVarDecl_; }
	bool operator ()( const SgDeclarationStatement *ds ) {
		if( const SgVariableDeclaration *vardecl = isSgVariableDeclaration( ds ) ) {
			const SgAccessModifier &mod
				= const_cast<SgVariableDeclaration *>(vardecl)->get_declarationModifier().get_accessModifier();
			if( mod.isPublic() ) {
				lastPublicVarDecl_ = vardecl;
				foundPublic_ = true;
				return true;
			}
		}
		return false;
	}
  private:
	bool foundPublic_;
	const SgVariableDeclaration *lastPublicVarDecl_;
};

/*XXX no isPODClass method
 
bool OBJ00_CPP( const SgNode *node ) { // Declare data members private
	// This works for unions too...
	if( const SgClassDefinition *classdef = isSgClassDefinition( node ) ) {
		if( !isPODClass( classdef ) ) {
			IsPublicData op;
			forEachClassMemberShortCircuit( classdef, op );
			if( op.foundPublic() ) {
				print_error(op.publicDeclaration(), "OBJ00-CPP", "is public data.", true);
				//diagnostic( "OBJ00-A", op.publicDeclaration(), "is public data." );
				return true;
			}
		}
	}
	return false;
}
*/

class ConvOpCount {
  public:
	ConvOpCount()
		: count_(0) {}
	size_t count() const
		{ return count_; }
	bool operator ()( const SgNode *node ) {
		if( const SgFunctionDeclaration *fdecl = isSgFunctionDeclaration( node ) ) {
			if( fdecl->get_specialFunctionModifier().isConversion() ) {
				++count_;
				return true;
			}
		}
		return false;
	}
  private:
	size_t count_;
};

bool OBJ01_CPP( const SgNode *node ) { // Be careful with the definition of conversion operators
	// I interpret "being careful" to not having more than one conversion function (though no such functions is better!)
	if( const SgClassDefinition *classdef = isSgClassDefinition( node ) ) {
		ConvOpCount op;
		forEachClassMember( classdef, op );
		if( op.count() > 1 ) {
			print_error( classdef,  "OBJ01_CPP", ("class has " + utostring(op.count()) + " conversion functions.").c_str() , true);
			//diagnostic( "OBJ01_A", classdef, "class has " + utostring(op.count()) + " conversion functions." );
			return true;
		}
	}
	return false;
}

/*XXX No isCopyAssignment() function
 
bool OBJ02_CPP( const SgNode *node ) { // Do not hide inherited non-virtual member functions
	bool result = false;
	if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
		SgDeclarationStatementPtrList dlist;
		const SgDeclarationStatementPtrList &members = cdef->get_members();
		MemberNameInfoList names;
		getMemberInfo( members, names );
		for( MemberNameInfoListIterator i = names.begin(); i != names.end(); ++i ) { // for each name
			if( isCopyAssignmentDeclaration( i->node_ ) ) // Note copy assign is not inherited!
				continue;
			if( searchInheritedMembers( cdef, dlist, IsNonVirtualFunctionDeclarationNamed(i->id_) ) ) {
				print_error( i->node_,  "OBJ02_CPP",  (std::string( i->id_ ) + " hides inherited nonvirtual function.").c_str() , true);
				//diagnostic( "OBJ02-A", i->node_, std::string( i->id_ ) + " hides inherited nonvirtual function." );
				result = true;
			}
		}
	}
	return result;
}
*/

bool OBJ03_CPP( const SgNode *node ) { // Prefer not to overload virtual functions
	bool result = false;
	if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
		SgDeclarationStatementPtrList dlist;
		const SgDeclarationStatementPtrList &members = cdef->get_members();
		for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) { // n**2 in members
			if( const SgFunctionDeclaration *fdec1 = isSgFunctionDeclaration( *i ) ) {
				if( fdec1->get_functionModifier().isVirtual() ) {
					const std::string name1 = fdec1->get_name().getString();
					for( SgDeclarationStatementPtrList::const_iterator j = members.begin(); j != members.end(); ++j ) {
						if( *j == *i ) // don't compare with self
							continue;
						if( const SgFunctionDeclaration *fdec2 = isSgFunctionDeclaration( *j ) ) {
							const std::string name2 = fdec2->get_name().getString();
							if( name1 == name2 ) {
								result = true;
								print_error( fdec1,  "OBJ03_CPP",  ("overloads virtual function on line " + utostring( fdec2->get_file_info()->get_line() )).c_str() , true);
							}
						}
					}
				}
			}
		}
	}
	return result;
}

bool OBJ04_CPP( const SgNode *node ) { // Prefer not to give virtual functions default argument initializers
	// Note member templates can't be virtual.
	bool result = false;
	if( const SgClassDefinition *cdef = isSgClassDefinition( node ) ) {
		SgDeclarationStatementPtrList dlist;
		const SgDeclarationStatementPtrList &members = cdef->get_members();
		for( SgDeclarationStatementPtrList::const_iterator i = members.begin(); i != members.end(); ++i ) { // n**2 in members
			if( SgFunctionDeclaration *fdec = isSgFunctionDeclaration( *i ) ) {
				if( fdec->get_functionModifier().isVirtual() ) {
					const SgInitializedNamePtrList &args = fdec->get_args();
					for( SgInitializedNamePtrList::const_iterator i = args.begin(); i != args.end(); ++i ) {
						if( (*i)->get_initializer() ) {
							result = true;
							print_error( fdec,  "OBJ04-A",  (fdec->get_name().getString() + " is a virtual function with default initializer.").c_str() , true);
							break; // avoid multiple reports for multiple initializers
						}
					}
				}
			}
		}
	}
	return result;
}

/* XXX No isCopyCtorDeclaration() function
 *
bool OBJ32_CPP( const SgNode *node ) { // Ensure that single-argument constructors are marked "explicit"
	//XXX Check for templates and member templates as well!
	// Note:  skip check for copy ctors and member template copy-like ctors.
	if( !isCopyCtorDeclaration( node ) && !isTemplateCopyLikeCtorDeclaration( node ) && isSingleArgCtorDeclaration( node ) ) {
		const SgFunctionDeclaration *fdecl = isSgFunctionDeclaration( node );
		const SgFunctionModifier &mod = fdecl->get_functionModifier();
		if( !mod.isExplicit() ) {
			//XXX returns 0 for instantiated template
			print_error( fdecl,  "OBJ32_CPP",  (fdecl->get_name().getString() + " is a non-explicit single-argument constructor.").c_str() , false);
			return true;
		}
	}
	return false;
}
*/

bool OBJ( SgProject *project ) {
	bool violation = false;
	Rose_STL_Container<SgNode *> nodes = NodeQuery::querySubTree( project, V_SgNode ); // all nodes.  should do visit
	for( Rose_STL_Container<SgNode *>::iterator i = nodes.begin(); i != nodes.end(); ++i ) {
		//violation |= OBJ00_CPP( *i );
		violation |= OBJ01_CPP( *i );
		//violation |= OBJ02_CPP( *i );
		violation |= OBJ03_CPP( *i );
		violation |= OBJ04_CPP( *i );
		//violation |= OBJ32_CPP( *i );
	}
	return violation;
}
