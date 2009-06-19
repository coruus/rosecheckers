/*
 * Copyright (C) 2007-2009 by Carnegie Mellon University.
 * All rights reserved.
 *
 * Permission to use this software and its documentation for any purpose is hereby granted,
 * provided that the above copyright notice appear and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that the name of CMU not
 * be used in advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 * CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, RISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
