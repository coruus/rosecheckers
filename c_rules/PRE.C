/**
 * \file PRE.C
 *
 * \note As written, these tests catch template declarations only if
 * instantiated.
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
 * Do not reuse a standard header file name
 *
 * \todo BROKEN, NEEDS ROSE SUPPPORT
 */
bool PRE04_A( const SgNode *node ) {
/*	const SgIncludeDirectiveStatement *inc = isSgIncludeDirectiveStatement(node);
	std::cerr << " boo " << std::endl;
	if(isSgC_PreprocessorDirectiveStatement(node))
		std::cerr << "yay!" << std::endl;
	print_error(node,"PRE04","debug", true);
	if(!inc)
		return false;
	std::cerr << "1 " << (const_cast<SgIncludeDirectiveStatement*>(inc))->unparseToCompleteString() << std::endl;*/
//SgDeclarationStatement * 	get_definingDeclaration ()
	return false;
}

/**
 * \note we have to call this PREPRO because of a namespace clash with PRE
 */
bool PREPRO(const SgNode *node) {
  bool violation = false;
//  violation |= PRE04_A(node);
  return violation;
}
