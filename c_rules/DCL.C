/**
 * \file DCL.C
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
 * Do not reuse variable names in subscopes
 */
bool DCL01_A( const SgNode *node ) {
	const SgInitializedName *varInitName = isSgInitializedName(node);
	if (!varInitName)
		return false;
	const SgName varName = varInitName->get_name();
	const SgScopeStatement *varScope = varInitName->get_scope();
	assert(varScope);
	while(!isSgGlobal(varScope)) {
		varScope = varScope->get_scope();
		if(varScope->symbol_exists(varName)) {
			print_error(node, "DCL01-A", "Do not reuse variable names in subscopes", true);
			return true;
		}
	}

	return false;
}

bool DCL(const SgNode *node) {
  bool violation = false;
  violation |= DCL01_A(node);
  return violation;
}
