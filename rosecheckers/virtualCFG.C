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

// Generates a CFG dot file from a C file.

#include "rose.h"
#include "cfgToDot.h"
using namespace std;

int main(int argc, char *argv[]) {
  // Build the AST used by ROSE
  SgProject* sageProject = frontend(argc,argv);

  // Run internal consistency tests on AST
  AstTests::runAllTests(sageProject);

  // Print the CFG view of each procedure in the AST in Dot format to cout
  NodeQuerySynthesizedAttributeType functions = NodeQuery::querySubTree(sageProject, V_SgFunctionDefinition);
  for (NodeQuerySynthesizedAttributeType::const_iterator i = functions.begin(); i != functions.end(); ++i) {
    SgFunctionDefinition* proc = isSgFunctionDefinition(*i);
    if (proc) {
      // Print out the full CFG, including bookkeeping nodes
      //      VirtualCFG::cfgToDotForDebugging(cout, proc->get_declaration()->get_name(), proc->cfgForBeginning());
      // Print out only those nodes which are "interesting" for analyses
      VirtualCFG::cfgToDot(cout, proc->get_declaration()->get_name(), VirtualCFG::makeInterestingCfg(proc));
    }
  }

  return 0;
}
