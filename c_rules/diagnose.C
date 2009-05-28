/**
 * \file diagnose.C
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
extern bool RES_CPP(const SgNode *node);

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
    RES_CPP(node);
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
  return 0;
}

