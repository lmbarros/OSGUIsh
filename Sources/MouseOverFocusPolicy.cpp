/******************************************************************************\
* MouseOverFocusPolicy.cpp                                                     *
* Puts the focus on the node under the mouse.                                  *
*                                                                              *
* Copyright (C) 2006-2010 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#include <OSGUIsh/MouseOverFocusPolicy.hpp>


namespace OSGUIsh
{
   // - MouseOverFocusPolicy::MouseOverFocusPolicy -----------------------------
   MouseOverFocusPolicy::MouseOverFocusPolicy(NodePtr& focusedNode)
      : FocusPolicy(focusedNode)
   {
      // empty...
   }



   // - MouseOverFocusPolicy::updateFocus --------------------------------------
   void MouseOverFocusPolicy::updateFocus(const osgGA::GUIEventAdapter& ea,
                                          const NodePtr nodeUnderMouse)
   {
      setFocusedNode(nodeUnderMouse);
   }

} // namespace OSGUIsh
