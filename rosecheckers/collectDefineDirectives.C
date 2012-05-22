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
 * \file collectDefineDirectives.C
 */

#include "rose.h"

using namespace std;

/**
 * \todo We need to figure out what this is all about and either finish or
 * delete it
 */

// Build a synthesized attribute for the tree traversal
class SynthesizedAttribute
   {
     public:
       // List of #define directives (save the PreprocessingInfo objects
       // so that we have all the source code position information).
          list<PreprocessingInfo*> accumulatedList;

          void display() const;
   };

void
SynthesizedAttribute::display() const
   {
     list<PreprocessingInfo*>::const_iterator i = accumulatedList.begin();
     while (i != accumulatedList.end())
        {
          printf ("CPP define directive = %s \n",(*i)->getString().c_str());
          i++;
        }
   }

class visitorTraversal : public AstBottomUpProcessing<SynthesizedAttribute>
   {
     public:
       // virtual function must be defined
          virtual SynthesizedAttribute evaluateSynthesizedAttribute ( 
                       SgNode* n, SynthesizedAttributesList childAttributes );
   };

SynthesizedAttribute
visitorTraversal::evaluateSynthesizedAttribute ( SgNode* n, SynthesizedAttributesList childAttributes )
   {
     SynthesizedAttribute localResult;

  // printf ("In evaluateSynthesizedAttribute(n = %p = %s) \n",n,n->class_name().c_str());

  // Build the list from children (in reverse order to preserve the final ordering)
     for (SynthesizedAttributesList::reverse_iterator child = childAttributes.rbegin(); child != childAttributes.rend(); child++)
        {
          localResult.accumulatedList.splice(localResult.accumulatedList.begin(),child->accumulatedList);
        }

  // Add in the information from the current node
     SgLocatedNode* locatedNode = isSgLocatedNode(n);
     if (locatedNode != NULL)
        {
          AttachedPreprocessingInfoType* commentsAndDirectives = locatedNode->getAttachedPreprocessingInfo();

          if (commentsAndDirectives != NULL)
             {
            // printf ("Found attached comments (to IR node at %p of type: %s): \n",locatedNode,locatedNode->class_name().c_str());
            // int counter = 0;

            // Use a reverse iterator so that we preserve the order when using push_front to add each directive to the accumulatedList
               AttachedPreprocessingInfoType::reverse_iterator i;
               for (i = commentsAndDirectives->rbegin(); i != commentsAndDirectives->rend(); i++)
                  {
                 // The different classifications of comments and directives are in ROSE/src/frontend/SageIII/rose_attributes_list.h
                    if ((*i)->getTypeOfDirective() == PreprocessingInfo::CpreprocessorDefineDeclaration)
                       {
#if 0
                         printf ("          Attached Comment #%d in file %s (relativePosition=%s): classification %s :\n%s\n",
                            counter++,(*i)->get_file_info()->get_filenameString().c_str(),
                            ((*i)->getRelativePosition() == PreprocessingInfo::before) ? "before" : "after",
                            PreprocessingInfo::directiveTypeName((*i)->getTypeOfDirective()).c_str(),
                            (*i)->getString().c_str());
#endif
                      // use push_front() to end up with source ordering of final list of directives
                         localResult.accumulatedList.push_front(*i);
                       }
                  }
             }
        }

  // printf ("localResult after adding current node info \n");
  // localResult.display();

     return localResult;
   }

int
main ( int argc, char* argv[] )
   {
     SgProject* project = frontend(argc,argv);

  // Build the traversal object and call "traverse" member function
     visitorTraversal exampleTraversal;
     SynthesizedAttribute results = exampleTraversal.traverse(project);

  // Output the results
     results.display();

     return 0;
   }

