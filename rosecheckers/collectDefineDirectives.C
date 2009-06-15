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

