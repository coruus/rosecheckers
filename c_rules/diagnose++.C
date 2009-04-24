/**
 * \file diagnose++.C
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


#include <iostream>
#include "rose.h"
#include "utilities.h"


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

class visitorTraversal : public AstSimpleProcessing {
public :
  visitorTraversal () {}
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
  visitorTraversal exampleTraversal;
  exampleTraversal.traverseInputFiles( project, preorder);
  return 0;
}
