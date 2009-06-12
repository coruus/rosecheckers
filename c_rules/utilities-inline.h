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
 * \file utilities-inline.h
 */

#ifndef UTILITIES_INLINE_H
#define UTILITIES_INLINE_H

#define findParentOfType(node, t) \
	is ## t ( findParentNodeOfType(node, V_ ##t ))

/**
 * Iterate over all initializers "i" in a list of variables
 * nodes should be something like SgVariableDeclaration->get_variables()
 */
#define FOREACH_INITNAME(nodes,i) \
	for(SgInitializedNamePtrList::const_iterator (i) = (nodes).begin(); (i) < (nodes).end(); (i)++)

/** Iterate over all nodes of type type below node, name the collection nodes
 * and name the iteroator i
 * 
 * \param[in] node Root node of the tree
 * \param[out] nodes What to call the collection
 * \param[out] i What to call the iterator
 * \param[in] type Only iterate over nodes of this type
 */
#define FOREACH_SUBNODE(node, nodes, i, type) \
	assert(node);\
	Rose_STL_Container<SgNode *> (nodes) = NodeQuery::querySubTree( const_cast<SgNode*>((const SgNode *) (node)), (type) ); \
	Rose_STL_Container<SgNode *>::iterator (i) = (nodes).begin(); \
	for (; (i) != (nodes).end(); ++(i) )

/**
 * Remove all modifiers such as const or volatile, but leave the typedefs
 */
inline const SgType *stripModifiers(const SgType *type) {
	assert(type);
	return type->stripType(SgType::STRIP_MODIFIER_TYPE);
}

/**
 * Remove all typedefs, but leave the modifiers
 */
inline const SgType *stripTypedefs(const SgType *type) {
	assert(type);
	return type->stripType(SgType::STRIP_TYPEDEF_TYPE);
}

/**
 * Checks to see if the type has the const modifier
 */
inline bool isConstType(const SgType *t) {
	assert(t);
	const SgModifierType *mt = isSgModifierType(stripTypedefs(t));
	return mt && mt->get_typeModifier().get_constVolatileModifier().isConst();
}
/**
 * Checks to see if the type has the volatile modifier
 */
inline bool isVolatileType(const SgType *t) {
	assert(t);
	const SgModifierType *mt = isSgModifierType(stripTypedefs(t));
	return mt && mt->get_typeModifier().get_constVolatileModifier().isVolatile();
}

/**
 * Returns a variable's declaration, given a reference to that var
 */
inline const SgInitializedName* getRefDecl(const SgVarRefExp* ref) {
	assert(ref);
	return ref->get_symbol()->get_declaration();
}

/**
 * Checks to see if the variable was declared static
 */
inline bool isStaticVar(const SgInitializedName *var) {
	assert(var);
	return const_cast<SgInitializedName*>(var)->get_declaration()->get_declarationModifier().get_storageModifier().isStatic();
}

/**
 * Checks to see if the variable was declared extern
 */
inline bool isExternVar(const SgInitializedName * var) {
	return const_cast<SgInitializedName*>(var)->get_declaration()->get_declarationModifier().get_storageModifier().isExtern();
}

/**
 * Checks the scope of the variable to see if it is global or not
 */
inline bool isGlobalVar(const SgInitializedName *var) {
	assert(var);
	return isSgGlobal(var->get_scope());
}

/**
 * Until ROSE implements these types natively, we will have to beat the
 * names out of the types with unparseToString()
 */
inline bool isTypeWcharT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "wchar_t";
}

inline bool isTypeTimeT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "time_t";
}
inline bool isTypeSizeT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "size_t";
}

inline bool isTypeRSizeT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "rsize_t";
}

inline bool isTypeUintptrT(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "uintptr_t";
}

inline bool isTypeVoid(const SgType *t) {
	assert(t);
	return stripModifiers(t)->unparseToString() == "void";
}

inline bool isTypeVoidStar(const SgType *t) {
	assert(t);
	const SgType *d = const_cast<SgType *>(t)->dereference();
	return (t !=d ) && isTypeVoid(d);
}

inline bool isTypeFile(const SgType *t) {
	assert(t);
	std::string str = stripModifiers(t)->unparseToString();
	return str == "FILE" || str == "struct _IO_FILE";
}

inline bool isCompilerGeneratedNode(const SgNode *node) {
	assert(node);
	return node->get_file_info()->isCompilerGenerated();
}

#endif
