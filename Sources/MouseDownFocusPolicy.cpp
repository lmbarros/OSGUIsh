/******************************************************************************\
* MouseDownFocusPolicy.cpp                                                     *
* A focus policy that sets the focus on a node that gets "MouseDown" event.    *
*                                                                              *
* Copyright (C) 2006-2010 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#include <OSGUIsh/MouseDownFocusPolicy.hpp>


namespace OSGUIsh
{
   // - MouseDownFocusPolicy::MouseDownFocusPolicy -----------------------------
   MouseDownFocusPolicy::MouseDownFocusPolicy (NodePtr& focusedNode)
      : FocusPolicy (focusedNode)
   { }



   // - MouseDownFocusPolicy::updateFocus --------------------------------------
   void MouseDownFocusPolicy::updateFocus (const osgGA::GUIEventAdapter& ea,
                                           const NodePtr nodeUnderMouse)
   {
      if (ea.getEventType() == osgGA::GUIEventAdapter::PUSH)
         setFocusedNode (nodeUnderMouse);
   }

}
