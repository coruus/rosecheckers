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

/*
 * \file type.h
 */

#ifndef TYPE_H
#define TYPE_H

//
// Type is a thin layer over SgType, intended to
//	a) simplify use of SgType, and
//	b) provide additional functionality to ask questions about and manipulate types
//
// This layer ordinarily strips away initial typedefs from a type so that, for instance,
//
//	typedef int INT;
//
// both int and INT will return true for isInt(), but INT will return true for isTypedef() as well.
//

#include "rose.h"

class Type {
  public:
	explicit Type( const SgType * );

	bool operator==(const Type& that) const;

	// We'll let the compiler write these for us:
	// ~Type();
	// Type( const Type & );
	// Type &operator =( const Type & );

	std::string mangledName() const;

	Type dereference() const; // will deref pointer, reference, or array, else same type
	Type stripInitialTypedefs() const;
	Type stripInitialPointers() const; // strip sequence of typedefs and pointers, but leave trailing typedefs
	Type stripInitialReferences() const;
	Type stripInitialPointersAndReferences() const;
	Type stripInitialArrays() const;
	Type stripInitialPointersAndArrays() const;
	Type stripInitialPointersReferencesAndArrays() const;
	Type stripTypedefsAndModifiers() const;
	Type baseType() const;

	bool isTypedef() const;
	bool isEllipsis() const;
	bool isBool() const;
	bool isPlainChar() const;
	bool isSignedChar() const;
	bool isUnsignedChar() const;
	bool isAnyChar() const;
	bool isPlainShort() const;
	bool isSignedShort() const;
	bool isUnsignedShort() const;
	bool isShort() const;
	bool isPlainInt() const;
	bool isSignedInt() const;
	bool isUnsignedInt() const;
	bool isInt() const;
	bool isPlainLong() const;
	bool isSignedLong() const;
	bool isUnsignedLong() const;
	bool isLong() const;
	bool isLongLong() const;
	bool isUnsignedLongLong() const;
	// no signed long long?
	bool isFloat() const;
	bool isDouble() const;
	bool isLongDouble() const;
	bool isWchar() const;

	bool isSize_t() const;

	//
	// These follow the structure of the TR1 <type_traits> library
	//
	// primary type categories, 11.3
	bool isVoid() const;
	bool isIntegral() const;
	bool isFloatingPoint() const;
	bool isArray() const;
	bool isPointer() const;
	bool isReference() const;
	bool isMemberObjectPointer() const;
	bool isMemberFunctionPointer() const;
	bool isEnum() const;
	const SgEnumDeclaration *getEnumDeclaration() const;
	bool isUnion() const;
	bool isClass() const; // Note: may be struct but not union!
	const SgClassDeclaration *getClassDeclaration() const;
	const SgClassDefinition *getClassDefinition() const;
	bool isFunction() const;

	// composite type categories, 11.4
	bool isArithmetic() const;
	bool isFundamental() const; //XXX what is the status of bool?
	bool isObject() const;
	bool isScalar() const;
	bool isCompound() const;
	bool isMemberPointer() const;

	// type properties, 11.5
	bool isConst() const;
	bool isVolatile() const;
	bool isPOD() const;
	bool isEmpty() const;
	bool isPolymorphic() const;
	bool isAbstract() const;
	bool hasTrivialConstructor() const;
	bool hasTrivialCopy() const;
	bool hasTrivialAssign() const;
	bool hasTrivialDestructor() const;
	bool hasNothrowConstuctor() const;
	bool hasNothrowCopy() const;
	bool hasNothrowAssign() const;
	bool hasVirtualDestructor() const;
	bool isSigned() const;
	bool isUnsigned() const;
	size_t rank() const; // array dimension, 0 if not array
	size_t extent( unsigned n ) const; // bound of nth array modifier

	// type relationsips, 11.6
	bool isSame( Type ) const;
	bool isConvertible( Type ) const;
	bool isBaseOf( Type ) const;

	// type transformations, 11.7
	Type removeConst() const;
	Type removeVolatile() const;
	Type removeCV() const;
	Type addConst() const;
	Type addVolatile() const;
	Type addCV() const;
	Type removeReference() const;
	Type addReference() const;
	Type removePointer() const;
	Type addPointer() const;
	Type removeExtent () const;
	Type removeAllExtents() const; // same as stripInitialArrays

	// alignment, 11.8
	//size_t alignmentOf() const;
	//Type alignedStorage( ) const;

  private:
	const SgType *orig_; // SgType used to initialize
	const SgType *t_; // orig_ stripped of initial sequence of typedefs (note uses of orig_ vs. t_ in implementations)
	bool isConst_;
	bool isVolatile_;
};

#endif
