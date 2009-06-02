/**
 * \file ERR.C
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
#include "utilities_cpp.h"

/**
 * Understand the termination behavior of assert() and abort()
 */
bool ERR06_C(const SgNode *node) {
  const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
  if (!(fnRef && isCallOfFunctionNamed(fnRef, "abort")))
    return false;

  std::set<const SgInitializedName *> fds;

  FOREACH_SUBNODE(findParentOfType(fnRef, SgFunctionDefinition), nodes, i, V_SgFunctionRefExp) {
    const SgFunctionRefExp *iFn = isSgFunctionRefExp(*i);
    assert(iFn);
    if (iFn == fnRef)
      break;

    if (isCallOfFunctionNamed(iFn, "open")
        ||isCallOfFunctionNamed(iFn, "fopen")
        ||isCallOfFunctionNamed(iFn, "mkstemp")) {
      const SgInitializedName *var = getVarAssignedTo(iFn, NULL);
      if (var && !(isGlobalVar(var) || isStaticVar(var))) {
        fds.insert(var);
      }
    } else if (isCallOfFunctionNamed(iFn, "close")
               ||isCallOfFunctionNamed(iFn, "open")) {
      const SgVarRefExp *varRef = isSgVarRefExp(removeImplicitPromotions(getFnArg(iFn, 0)));
      if (!varRef)
        continue;
      const SgInitializedName *var = getRefDecl(varRef);
      assert(var);
      fds.erase(fds.find(var));
    }
  }

  if (fds.size() > 0) {
    print_error(node, "ERR06-C", "Understand the termination behavior of assert() and abort()", true);
    return true;
  }

  return false;
}

/**
 * Don't redefine errno
 *
 * \note If the program includes errno.h than errno is a macro, so we won't be
 * able to flag this declaration
 */
bool ERR31_C(const SgNode *node) {
  const SgVariableDeclaration *varDecl = isSgVariableDeclaration(node);
  if (!varDecl)
    return false;
  bool violation = false;
  FOREACH_INITNAME(varDecl->get_variables(), i) {
    const SgInitializedName *var = isSgInitializedName(*i);
    assert(var);
    if (var->get_name().getString() == "errno") {
      print_error(node, "ERR31-C", "Don't redefine errno");
      violation = true;
    }
  }
  return violation;
}

bool ERR_C(const SgNode *node) {
  bool violation = false;
  violation |= ERR06_C(node);
  violation |= ERR31_C(node);
  return violation;
}


/// C++ checkers

enum Exp { ExpAppropriate = 0, ExpPointer, ExpStd, ExpOtherBad };

Exp isNotAppropriateExceptionType( Type t ) {
  if( const SgClassDeclaration *classdecl = t.getClassDeclaration() ) {
    // only type thrown from std namespace should be derived from std::exception
    if( isClassDeclaredInStdNamespace( classdecl ) ) {
      if( !isStdExceptionOrTypeDerivedFromStdException( classdecl ) )
        return ExpStd;
    }
  }
  else if( t.isPointer() ) { // it's a pointer to something: bad
    return ExpPointer;
  }
  else { // it's not a class or a pointer, violation of rule
    return ExpOtherBad;
  }
  return ExpAppropriate;
}

/* ERR08-CPP. Prefer special-purpose types for exceptions */
bool ERR08_CPP( const SgNode *node ) {
  bool result = false;
  if( const SgThrowOp *throwop = isSgThrowOp( node ) ) {
    if( throwop->get_throwKind() == SgThrowOp::throw_expression ) { // not a rethrow or exception-specification
      const SgExpression *expr = throwop->get_operand();
      Type t( expr->get_type() );
      switch( isNotAppropriateExceptionType( t ) ) {
      case ExpAppropriate:
        break;
      case ExpPointer:
        result = true;
        print_error(node, "ERR08-CPP", "Should throw a special-purpose exception object, not a pointer.", true);
        break;
      case ExpStd:
        result = true;
        print_error(node, "ERR08-CPP", "Do not throw standard types (except standard exceptions)." , true);
        break;
      case ExpOtherBad:
        result = true;
        print_error(node, "ERR08-CPP", "Should throw a special-purpose exception object.", true);
        break;
      }
    }
  }
  return result;
}

bool doTryCatch( const SgTryStmt *trystmt ) {
  bool result = false;
  //    const SgStatement *trybody = trystmt->get_body();
  //    const SgStatementPtrList &stats = trybody->get_statements();
  const SgCatchStatementSeq *catchseq = trystmt->get_catch_statement_seq_root();
  const SgStatementPtrList &clauses = catchseq->get_catch_statement_seq();
  size_t clauseNumber = 0;
  size_t ellipsisClause = 0;
  for( SgStatementPtrList::const_iterator i = clauses.begin(); i != clauses.end(); ++i ) {
    if( const SgCatchOptionStmt *clause = isSgCatchOptionStmt( *i ) ) {
      ++clauseNumber;
      const SgVariableDeclaration *decl = clause->get_condition();
      const SgInitializedNamePtrList &vars = decl->get_variables();
      const SgInitializedName *arg = vars.front();
      Type argType( arg->get_type() );
      const std::string argName = arg->get_name().getString();
      // const SgBasicBlock *body = clause->get_body();
      if( argType.isEllipsis() ) {
        if( ellipsisClause ) {
          // Note:  The compiler seems to catch this one, typically.
          result = true;
          print_error(clause, "ERR09-CPP", "Repeated ellipsis catch-clause.", true);
        }
        ellipsisClause = clauseNumber;
      }
      else {
        if( !argType.isReference() ) {
          result = true;
          print_error(clause, "ERR09-CPP", "Should catch by reference.", true);
        }
        else {
          // even if it is a reference, it may be a ref to an inappropriate type
          switch( isNotAppropriateExceptionType( argType.dereference() ) ) {
          case ExpAppropriate:
            break;
          case ExpPointer:
            result = true;
            print_error(clause, "ERR09-CPP", "Should catch objects, not pointers.", true);
            break;
          case ExpStd:
            result = true;
            print_error(clause, "ERR09-CPP", "Do not catch standard types (except standard exceptions).", true);
            break;
          case ExpOtherBad:
            result = true;
            print_error(clause, "ERR09-CPP", "Should catch a special-purpose exception object.", true);
            break;
          }
        }
      }
    }
  }
  if( ellipsisClause && (ellipsisClause != clauseNumber) ) {
    // Note:  The compiler seems to catch this one, typically.
    result = true;
    print_error(catchseq, "ERR09-CPP", "Ellipsis should be last catch-clause.", true);
  }
  return result;
}

/* ERR09-CPP. Throw anonymous temporaries and catch by reference */
bool ERR09_CPP(const SgNode *node ) {
  if( const SgThrowOp *throwop = isSgThrowOp( node ) ) {
    if( throwop->get_throwKind() == SgThrowOp::throw_expression ) { // not a rethrow or exception-specification
      const SgExpression *expr = throwop->get_operand();
      ROSE_ASSERT( expr );

      //??? Don't know how torecognize anonymous temporaries!
    }
  }
  else if( const SgTryStmt *trystmt = isSgTryStmt( node ) )
    return doTryCatch( trystmt );
  else
    return false;

  return false;
}

bool ERR_CPP(const SgNode *node) {
  bool violation = false;
  violation |= ERR08_CPP(node);
  violation |= ERR09_CPP(node);
  return violation;
}

