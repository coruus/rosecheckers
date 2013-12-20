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
 * \file ENV.C
 */

#include "rose.h"
#include "utilities.h"
#include <boost/regex.hpp>

/**
 *
 Returns true if the char * represented by var is modified by a function.
 *
 */
bool isStringModifiedInSubnodes(const SgInitializedName *var, const SgNode *node) {

  // Loop through function searching if string is modified.
  FOREACH_SUBNODE(node, nodes, i, V_SgFunctionCallExp) {

    // Get function call.
    const SgFunctionCallExp *fnCall = (const SgFunctionCallExp *)(*i);
    unsigned int numArgs = fnCall->get_args()->get_expressions().size();

    // Check to see if string is modified by function call.
    for (unsigned int itr = 0; itr < numArgs; itr++) {

      // Skip c string functions.
      std::string fnName = fnCall->getAssociatedFunctionSymbol()->get_name().getString();
      if (fnName == "strchr" ||
          fnName == "strrchr" ||
          fnName == "strcmp" ||
          fnName == "strncmp" ||
          fnName == "strspn" ||
          fnName == "strcspn" ||
          fnName == "strlen" ||
          fnName == "strpbrk" ||
          fnName == "strstr" ||
          fnName == "strdup") {
        continue;
      }

      // Skip strcpy, strcat if string is src.
      if (itr == 1 &&
          (fnName == "strcpy" ||
           fnName == "strncpy" ||
           fnName == "strcat" ||
           fnName == "strncat")) {
        continue;
      }

      // Return true if char * is passed to function.
      const SgVarRefExp *arg = isSgVarRefExp(getFnArg(fnCall, itr));
      if (arg == NULL) {
        continue;
      }
      SgName argName = arg->get_symbol()->get_name();
      if (argName == var->get_name()) {
        return true;
      }
    }
  }
  return false;
}

/**
 * Do not store the pointer to the string returned by getenv()
 *
 * \todo Do we also need to check for putenv/setenv in the loop?
 */
bool ENV00_C(const SgNode *node) {
	// find call of getenv
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;
	if (!isCallOfFunctionNamed(fnRef, "getenv"))
		return false;
	// find var saved into
	const SgInitializedName *var = getVarAssignedTo(fnRef, NULL);
	if (!var)
		return false;
	bool getenv_found = false;
	bool write = false;
	bool after = false;

	// traverse down, searching for copy functions (break)
	FOREACH_SUBNODE(findParentOfType(fnRef, SgFunctionDefinition), nodes, i, V_SgExpression) {
		const SgExpression * expr = isSgExpression(*i);
		const SgFunctionRefExp *iFnRef = isSgFunctionRefExp(*i);
		assert(expr);
		if (!after) {
			if (iFnRef == fnRef)
				after = true;
			continue;
		}
		// search for another call to getenv
		if (iFnRef && isCallOfFunctionNamed(iFnRef, "getenv")) {
			getenv_found = true;
			if (var == getVarAssignedTo(isSgFunctionRefExp(expr), NULL))
				return false;
			continue;
		}
		if (!getenv_found)
			continue;

		// search for write (break)
		const SgVarRefExp *iVar = isSgVarRefExp(expr);
		if (!iVar || (getRefDecl(iVar) != var))
			continue;

		const SgFunctionCallExp *iFnCall = findParentOfType(iVar, SgFunctionCallExp);
		if (!iFnCall) {
			if (varWrittenTo(iVar))
				return false;
			write = true;
			break;
		}
		const SgFunctionRefExp *iFn = isSgFunctionRefExp(iFnCall->get_function());
		assert(iFn);
		if (isCallOfFunctionNamed(iFn, "strcpy")
        ||isCallOfFunctionNamed(iFn, "memset")
        ||isCallOfFunctionNamed(iFn, "strdup"))
			return false;
		write = true;
		break;
	}

	// if read & getenv then error
	if (getenv_found && write) {
		print_error(node, "ENV00-C", "Do not store the pointer to the string returned by getenv()", true);
		return true;
	}
	return false;
}

/**
 * Beware of multiple environment variables with the same effective name
 */
bool ENV02_C(const SgNode *node) {
	static std::set<std::string> origStrs;
	static std::set<std::string> normStrs;
	static std::map<std::string, const SgFunctionRefExp *> strToNode;

	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;

	if (!(isCallOfFunctionNamed(fnRef, "getenv")
        ||isCallOfFunctionNamed(fnRef, "setenv")
        ||isCallOfFunctionNamed(fnRef, "putenv"))) {
		return false;
	}
	const SgStringVal *strVal = isSgStringVal(removeImplicitPromotions(getFnArg(fnRef,0)));
	if (!strVal)
		return false;
	std::string str = strVal->get_value();
	if (const unsigned int eq = str.find_first_of("="))
		str = str.substr(0,eq);

	if (origStrs.find(str) != origStrs.end())
		return false;
	origStrs.insert(str);

	std::transform(str.begin(), str.end(), str.begin(), tolower);

	if (normStrs.find(str) != normStrs.end()) {
		print_error(node, "ENV02-C", "Beware of multiple environment variables with the same effective name", true);
		print_error(strToNode[str], "ENV02-C", "Previous reference was here", true);
		return true;
	}

	normStrs.insert(str);
	strToNode[str] = fnRef;
	return false;
}


/**
 * Do not modify the string returned by getenv()
 */
bool ENV30_C(const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;
	if (!isCallOfFunctionNamed(fnRef, "getenv"))
		return false;
	const SgInitializedName *var = getVarAssignedTo(fnRef, NULL);
	if (!var)
		return false;
	if (isConstType(var->get_type()->dereference()))
		return false;

  const SgFunctionDeclaration *fnDecl = findParentOfType(node, SgFunctionDeclaration);

  if (isStringModifiedInSubnodes(var, fnDecl)) {
    print_error(node, "ENV30-C", "Do not modify the string returned by getenv()");
    return true;
  }

	return false;
}

/**
 * Do not rely on an environment pointer following an operation that may
 * invalidate it 
 */
bool ENV31_C(const SgNode *node) {
	const SgVarRefExp *varRef = isSgVarRefExp(node);
	if (!varRef)
		return false;
	if (getRefDecl(varRef)->get_name().getString() != "envp")
		return false;
	bool violation = false;
	FOREACH_SUBNODE(getRefDecl(varRef)->get_scope(), nodes, i, V_SgExpression) {
		if (varRef == isSgVarRefExp(*i))
			break;
		const SgFunctionRefExp *iFn = isSgFunctionRefExp(*i);
		if (!iFn)
			continue;
		if (isCallOfFunctionNamed(iFn, "putenv")
        ||isCallOfFunctionNamed(iFn, "setenv")) {
			violation = true;
			break;
		}
	}
	if (violation) {
		print_error(node, "ENV31-C", "Do not rely on an environment pointer following an operation that may invalidate it");
	}
	return violation;
}

/**
 * No atexit handler should terminate in any way other than by returning
 * \note This catches calls to exit, _exit, _abort, _Exit, longjmp and
 * siglongjmp
 */
bool ENV32_C(const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!(fnRef && isCallOfFunctionNamed(fnRef, "atexit")))
		return false;

	const SgFunctionRefExp* ref = isSgFunctionRefExp(getFnArg(fnRef, 0));
	assert(ref);

	const SgFunctionDeclaration *fnDecl = ref->get_symbol()->get_declaration();
	assert(fnDecl);

	bool violation = false;

	FOREACH_SUBNODE(fnDecl,nodes,i,V_SgFunctionRefExp) {
		const SgFunctionRefExp *iFn = isSgFunctionRefExp(*i);
		assert(iFn);
		if (isCallOfFunctionNamed(iFn, "exit")
        || isCallOfFunctionNamed(iFn, "_exit")
        || isCallOfFunctionNamed(iFn, "abort")
        || isCallOfFunctionNamed(iFn, "_Exit")
        || isCallOfFunctionNamed(iFn, "longjmp")
        || isCallOfFunctionNamed(iFn, "siglongjmp")) {
			print_error(fnDecl, "ENV32-C", "All exit handlers must return normally");
			violation = true;
		}
	}

	return violation;
}

/**
 * Do not use system()
 *
 * \note As written, these tests catch template declarations only if instantiated.
 */
bool ENV33_C(const SgNode *node) {
	const SgFunctionRefExp *fnRef = isSgFunctionRefExp(node);
	if (!fnRef)
		return false;
	if (!(isCallOfFunctionNamed(fnRef, "system")
        ||isCallOfFunctionNamed(fnRef, "popen")))
		return false;

	const SgStringVal *command = isSgStringVal(removeCasts(getFnArg(fnRef,0)));
	if (command) {
	  const std::string commandStr = command->get_value();
	  boost::regex r("`.*`");
	  if (regex_search(commandStr,r)) {
	    return false;
	  }
	}

	print_error(node, "ENV33-C", "Do not call system()", true);
	return true;
}

bool ENV_C(const SgNode *node) {
	bool violation = false;
	violation |= ENV00_C(node);
	violation |= ENV02_C(node);
	violation |= ENV30_C(node);
	violation |= ENV31_C(node);
	violation |= ENV32_C(node);
	violation |= ENV33_C(node);
	return violation;
}


/// C++ checkers

bool ENV_CPP(const SgNode *node) {
  bool violation = false;
  violation |= ENV_C(node);
  return violation;
}

