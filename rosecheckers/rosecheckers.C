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


void check_usage(int argc, char* argv[]) {
  int flag = 0;
  for (int i = 0; i < argc; i++) {
    int s = strlen(argv[i]);
    if (s > 2 && (!strcmp( ".c", argv[i] + s-2) || !strcmp( ".C", argv[i] + s-2))) {
      flag = 1; break;}
    if (s > 4 && (!strcmp( ".cpp", argv[i] + s-4) || !strcmp( ".cxx", argv[i] + s-4))) {
      flag = 1; break;}
  }
  if (flag) {return;}

  fprintf(stderr, "Invalid arguments...must specify a C/C++ source file:\n");
  abort();
}


int main( int argc, char* argv[]) {
  for (int i = 0; i < argc; i++) {
    fprintf(stderr, " %s", argv[i]);
  }
  fputs("\n", stderr);
  check_usage( argc, argv);
  SgProject* project = frontend(argc,argv);
  ROSE_ASSERT( project );
  AstSimpleProcessing* visitorTraversal;
  if (project->get_C_only())
    visitorTraversal = new CVisitorTraversal();
  else
    visitorTraversal = new CPPVisitorTraversal();
  visitorTraversal->traverseInputFiles( project, preorder);
  delete visitorTraversal;
  fputs("rosecheckers complete\n", stderr);
  return 0;
}

