/******************************************************************************\
* FocusPolicy.cpp                                                              *
* An interface used to implement different policies for changing focus from    *
* one node to another.                                                         *
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
