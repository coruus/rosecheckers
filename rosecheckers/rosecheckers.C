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

/**
 * \file rosecheckers.C
 */

//#include <iostream>
#include "rose.h"

extern bool PRE_C(const SgNode *node);
extern bool DCL_C(const SgNode *node);
extern bool EXP_C(const SgNode *node);
extern bool ARR_C(const SgNode *node);
extern bool FLP_C(const SgNode *node);
extern bool INT_C(const SgNode *node);
extern bool STR_C(const SgNode *node);
extern bool MEM_C(const SgNode *node);
extern bool FIO_C(const SgNode *node);
extern bool ENV_C(const SgNode *node);
extern bool SIG_C(const SgNode *node);
extern bool ERR_C(const SgNode *node);
extern bool MSC_C(const SgNode *node);
extern bool POS_C(const SgNode *node);

class CVisitorTraversal : public AstSimpleProcessing {
public :
  CVisitorTraversal () {}
  virtual void visit(SgNode* node) {
    PRE_C(node);
    DCL_C(node);
    EXP_C(node);
    ARR_C(node);
    FLP_C(node);
    INT_C(node);
    STR_C(node);
    MEM_C(node);
    FIO_C(node);
    ENV_C(node);
    SIG_C(node);
    ERR_C(node);
    MSC_C(node);
    POS_C(node);
  }
};


extern bool PRE_CPP(const SgNode *node);
extern bool DCL_CPP(const SgNode *node);
extern bool EXP_CPP(const SgNode *node);
extern bool ARR_CPP(const SgNode *node);
extern bool FLP_CPP(const SgNode *node);
extern bool INT_CPP(const SgNode *node);
extern bool STR_CPP(const SgNode *node);
extern bool MEM_CPP(const SgNode *node);
extern bool FIO_CPP(const SgNode *node);
extern bool ENV_CPP(const SgNode *node);
extern bool SIG_CPP(const SgNode *node);
extern bool ERR_CPP(const SgNode *node);
extern bool OBJ_CPP(const SgNode *node);
extern bool MSC_CPP(const SgNode *node);

class CPPVisitorTraversal : public AstSimpleProcessing {
public :
  CPPVisitorTraversal () {}
  virtual void visit(SgNode* node) {
    PRE_CPP(node);
    DCL_CPP(node);
    EXP_CPP(node);
    ARR_CPP(node);
    FLP_CPP(node);
    INT_CPP(node);
    STR_CPP(node);
    MEM_CPP(node);
    FIO_CPP(node);
    ENV_CPP(node);
    SIG_CPP(node);
    ERR_CPP(node);
    OBJ_CPP(node);
    MSC_CPP(node);
  }
};


int main( int argc, char* argv[]) {
  SgProject* project = frontend(argc,argv);
  ROSE_ASSERT( project );
  AstSimpleProcessing* visitorTraversal;
  if (project->get_C_only())
    visitorTraversal = new CVisitorTraversal();
  else
    visitorTraversal = new CPPVisitorTraversal();
  visitorTraversal->traverseInputFiles( project, preorder);
  delete visitorTraversal;
  return 0;
}

