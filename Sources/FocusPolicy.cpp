/******************************************************************************\
* FocusPolicy.cpp                                                              *
* Interface for policies used to change the focus from one node to another.    *
*                                                                              *
* Copyright (C) 2006-2010 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#include <OSGUIsh/FocusPolicy.hpp>

namespace OSGUIsh
{
   // - FocusPolicy::FocusPolicy -----------------------------------------------
   FocusPolicy::FocusPolicy (NodePtr& focusedNode)
      : focusedNode_(focusedNode)
   { }
}
