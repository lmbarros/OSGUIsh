/******************************************************************************\
* FocusPolicy.cpp                                                              *
* Interface for policies used to change the focus from one node to another.    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <OSGUIsh/FocusPolicy.hpp>

namespace OSGUIsh
{
   // - FocusPolicy::FocusPolicy -----------------------------------------------
   FocusPolicy::FocusPolicy (NodePtr& focusedNode)
      : focusedNode_(focusedNode)
   { }
}
