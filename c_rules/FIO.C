/**
 * \file FIO.C
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

/**
 * Prefer fseek() to rewind()
 */
bool FIO07_A( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "rewind")) return false;
  print_error( node, "FIO07-A", "Prefer fseek() to rewind()", true);
  return true;
}

/**
 * Take care when calling remove() on an open file
 */
bool FIO08_A( const SgNode *node ) {
	if(!isCallOfFunctionNamed(node, "remove"))
		return false;

	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	assert(fnRef);

	const SgExpression *fnExp = removeImplicitPromotions(getFnArg(fnRef,0));
	assert(fnExp);

	const SgVarRefExp *ref = isSgVarRefExp(fnExp);
	if(!ref)
		return false;
	const SgInitializedName *var = getRefDecl(ref);

	bool opened = false;
	bool closed = false;
	/* *open or *close */
	const SgFunctionRefExp *iFn = NULL;
	/* Argument to *open or *close */
	const SgVarRefExp *iVar = NULL;
	/* File descriptor return by *open */
	const SgInitializedName *fd = NULL;

	FOREACH_SUBNODE(findParentNodeOfType(node, V_SgFunctionDefinition).first, nodes, i, V_SgFunctionRefExp) {
		iFn = isSgFunctionRefExp(*i);
		assert(iFn);

		if(iFn == fnRef)
			break;

		iVar = isSgVarRefExp(removeImplicitPromotions(getFnArg(iFn,0)));
		if (!iVar)
			continue;

		if(isCallOfFunctionNamed(iFn, "open")
		|| isCallOfFunctionNamed(iFn, "fopen")
		|| isCallOfFunctionNamed(iFn, "freopen")) {
			fd = getVarAssignedTo(iFn, NULL);
			assert(fd);

			if (getRefDecl(iVar) == var) {
				closed = false;
				opened = true;
			}
		}

		if (opened
		&& (isCallOfFunctionNamed(iFn, "close"))
		|| isCallOfFunctionNamed(iFn, "fclose")) {
			if (getRefDecl(iVar) == fd) {
				closed = true;
				opened = false;
			}
		}
	}


	if (opened && !closed) {
		print_error(node,"FIO08-A", "Take care when calling remove() on an open file", true);
		return true;
	}

	return false;
}

/**
 * Take care when specifying the mode parameter of fopen()
 */
bool FIO11_A( const SgNode *node ) {
	if (!isCallOfFunctionNamed(node, "fopen"))
		return false;

	const SgStringVal* mode = isSgStringVal(getFnArg(isSgFunctionRefExp(node), 1));
	if(!mode)
		return false;

	const std::string str = mode->get_value();

	if((str == "r")
	|| (str == "w")
	|| (str == "a")
	|| (str == "rb")
	|| (str == "wb")
	|| (str == "ab")
	|| (str == "r+")
	|| (str == "w+")
	|| (str == "r+b")
	|| (str == "rb+")
	|| (str == "w+b")
	|| (str == "wb+")
	|| (str == "a+b")
	|| (str == "ab+"))
		return false;

	print_error(node, "FIO11-A", "Take care when specifying the mode parameter of fopen()", true);
	return true;
}

/**
 * Prefer setvbuf() to setbuf()
 */
bool FIO12_A( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "setbuf")) return false;
  print_error( node, "FIO12-A", "Prefer setvbuf() to setbuf()", true);
  return true;
}

/**
 * Exclude user input from format strings
 *
 * We make sure that the format argument to *printf family of functions is
 * either const or a string
 */
bool FIO30_C( const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if(!fnRef)
		return false;

	unsigned int argNum;
	if(isCallOfFunctionNamed(fnRef, "printf")
	|| isCallOfFunctionNamed(fnRef, "scanf")
	|| isCallOfFunctionNamed(fnRef, "vprintf")
	|| isCallOfFunctionNamed(fnRef, "vscanf")) { argNum = 0; }
	else if(isCallOfFunctionNamed(fnRef, "fprintf")
	|| isCallOfFunctionNamed(fnRef, "fscanf")
	|| isCallOfFunctionNamed(fnRef, "sprintf")
	|| isCallOfFunctionNamed(fnRef, "sscanf")
	|| isCallOfFunctionNamed(fnRef, "vfprintf")
	|| isCallOfFunctionNamed(fnRef, "vfscanf")
	|| isCallOfFunctionNamed(fnRef, "vsprintf")
	|| isCallOfFunctionNamed(fnRef, "vsscanf")) { argNum = 1; }
	else if(isCallOfFunctionNamed(fnRef, "snprintf")
	|| isCallOfFunctionNamed(fnRef, "vsnprintf")) {argNum = 2; }
	else return false;

	const SgExpression *frmt = removeImplicitPromotions(getFnArg(fnRef,argNum));
	assert(frmt);
	const SgType *frmtType = frmt->get_type();
	assert(frmtType);

	/**
	 * for some reason we can't find the const version of dereference
	 */
	bool isConst = Type((const_cast<SgType *>(frmtType))->dereference()).isConst();
	if(!(isConst || isSgTypeString(frmtType))) {
		print_error(node, "FIO30-C", "Exclude user input from format strings");
		return true;
	}

	return false;
}

/**
 * Use int to capture the return value of character IO functions
 */
bool FIO34_C( const SgNode *node) {
	if(!isSgFunctionRefExp(node))
		return false;

	if(!isCallOfFunctionNamed( node, "fputc") && !isCallOfFunctionNamed( node, "putc") &&
		!isCallOfFunctionNamed( node, "putchar") && !isCallOfFunctionNamed( node, "ungetc") &&
		!isCallOfFunctionNamed( node, "fgetc") && !isCallOfFunctionNamed( node, "getc") &&
		!isCallOfFunctionNamed( node, "getchar"))
		return false;

	assert(node->get_parent());
	assert(node->get_parent()->get_parent());

	SgNode const *parent = node->get_parent()->get_parent();

	SgCastExp const *cast;
	if(!(cast = isSgCastExp(parent)))
		return false;

	Type t = Type(cast->get_type());
	if(t.isPlainInt() || t.isSignedInt() || t.isPlainLong() ||
		t.isSignedLong() || (t.isLongLong() && !t.isUnsignedLongLong()))
		return false;

	print_error( node, "FIO34-C", "Use int to capture the return value of character I/O functions");
	return true;
}

/**
 * Do not use tmpfile()
 */
bool FIO43_C( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "tmpfile")) return false;
  print_error( node, "FIO43-C", "Do not use tmpfile()");
  return true;
}

/**
 * Do not use fopen() on the results of tmpnam()
 */
bool FIO43_C_2( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "tmpnam")) return false;

  if (!isVarUsedByFunction("fopen", isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 0))))
    return false;

  print_error( node, "FIO43-C", "Do not use fopen() on the results of tmpnam()");
  return true;
}

/**
 * Do not use open() on the results of mktemp()
 */
bool FIO43_C_3( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "mktemp")) return false;

  if (!isVarUsedByFunction("open", isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 0))))
    return false;

  print_error( node, "FIO43-C", "Do not use open() on the results of mktemp()");
  return true;
}

/**
 * Do not use fopen_s() on the results of tmpnam_s()
 */
bool FIO43_C_4( const SgNode *node ) {
  if (!isCallOfFunctionNamed( node, "tmpnam_s")) return false;

  if (!isVarUsedByFunction("fopen_s", isSgVarRefExp( getFnArg( isSgFunctionRefExp(node), 1))))
    return false;

  print_error( node, "FIO43-C", "Do not use fopen_s() on the results of tmpnam_s()");
  return true;
}

bool FIO(const SgNode *node) {
  bool violation = false;
  violation |= FIO07_A(node);
  violation |= FIO08_A(node);
  violation |= FIO12_A(node);
  violation |= FIO30_C(node);
  violation |= FIO34_C(node);
  violation |= FIO43_C(node);
  violation |= FIO43_C_2(node);
  violation |= FIO43_C_3(node);
  violation |= FIO43_C_4(node);
  return violation;
}
