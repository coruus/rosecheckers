/*
 * Copyright (c) 2007-2012 Carnegie Mellon University.
 * 
 * All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following acknowledgments
 * and disclaimers.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the
 * distribution.
 * 
 * 3. The names “Carnegie Mellon University,” "CERT” and/or “Software
 * Engineering Institute" shall not be used to endorse or promote
 * products derived from this software without prior written
 * permission. For written permission, please contact
 * permission@sei.cmu.edu.
 * 
 * 4. Products derived from this software may not be called "CERT" nor
 * may "CERT" appear in their names without prior written permission
 * of permission@sei.cmu.edu.
 * 
 * 5. Redistributions of any form whatsoever must retain the following
 * acknowledgment:
 * 
 * "This product includes software developed by CERT with funding and
 * support from the Department of Defense under Contract No. FA
 * 8721-05-C-0003.  The U.S. Government's rights to use, modify,
 * reproduce, release, perform, display, or disclose this material are
 * restricted by the Rights in Technical Data-Noncommercial Items
 * clauses (DFAR 252-227.7013 and DFAR 252-227.7013 Alternate I
 * contained in the foregoing identified contract.
 * 
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY “AS IS” AND
 * CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AS TO ANY MATTER, AND ALL SUCH WARRANTIES,
 * INCLUDING WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE, ARE EXPRESSLY DISCLAIMED.  WITHOUT LIMITING THE
 * GENERALITY OF THE FOREGOING, CARNEGIE MELLON UNIVERSITY DOES NOT
 * MAKE ANY WARRANTY OF ANY KIND RELATING TO EXCLUSIVITY,
 * INFORMATIONAL CONTENT, ERROR-FREE OPERATION, RESULTS TO BE OBTAINED
 * FROM USE, FREEDOM FROM PATENT, TRADEMARK AND COPYRIGHT INFRINGEMENT
 *  AND/OR FREEDOM FROM THEFT OF TRADE SECRETS.”
 */

/**
 * \file ERR.C
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
  violation |= ERR_C(node);
  return violation;
}

