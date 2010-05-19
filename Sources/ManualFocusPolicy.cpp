/******************************************************************************\
* ManualFocusPolicy.cpp                                                        *
* A focus policy that doesn't change the focus.                                *
*                                                                              *
* Copyright (C) 2006-2010 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#include <OSGUIsh/ManualFocusPolicy.hpp>


namespace OSGUIsh
{
   // - ManualFocusPolicy::ManualFocusPolicy -----------------------------------
   ManualFocusPolicy::ManualFocusPolicy(NodePtr& focusedNode)
      : FocusPolicy(focusedNode)
   {
      // empty
   }



   // - ManualFocusPolicy::updateFocus -----------------------------------------
   void ManualFocusPolicy::updateFocus(const osgGA::GUIEventAdapter& ea,
                                       const NodePtr nodeUnderMouse)
   {
      // Do nothing. That's what "manual" is about.
   }

} // namespace OSGUIsh
