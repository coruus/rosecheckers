/**
 * \file utilities_cpp.C
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

const SgClassDeclaration *getClassDeclaration(const SgType* t) {
  if (const SgClassType *ct = isSgClassType(t)) {
    if (const SgDeclarationStatement *dec = ct->get_declaration()) {
      if (const SgClassDeclaration *cdec = isSgClassDeclaration(dec)) {
	//std::cout << "SgClassDeclaration: it's a declaration at line " << cdec->get_file_info()->get_line() << std::endl;
	return cdec;
      }
    }
  }
  return NULL;
}

const char *const nameOfStandardNamespaceScope = "::std";

bool isClassDeclaredInStdNamespace(const SgClassDeclaration *decl) {
  if (const SgScopeStatement *scope = decl->get_scope()) {
    const std::string scopeName(scope->get_qualified_name().getString());
    return scopeName == nameOfStandardNamespaceScope; // this check seems fragile...
  }
  return false;
}

const char *const nameOfStdExceptionClass = "exception";

bool inheritsFromStdException(const SgClassDeclaration *cdecl) {
  const std::string className(cdecl->get_name().getString());
  if (className == nameOfStdExceptionClass) {
    if (isClassDeclaredInStdNamespace(cdecl))
      return true;
  }

  const SgClassDefinition *cdef = cdecl->get_definition();
  if (cdef != NULL) {
    const SgBaseClassPtrList &bases = cdef->get_inheritances();
    for (SgBaseClassPtrList::const_iterator i = bases.begin(); i != bases.end(); ++i) {
      const SgClassDeclaration *baseDecl = (*i)->get_base_class();
      if (baseDecl != NULL) {
	if (inheritsFromStdException( baseDecl))
	  return true;
      }
    }
  }
  return false;
}

