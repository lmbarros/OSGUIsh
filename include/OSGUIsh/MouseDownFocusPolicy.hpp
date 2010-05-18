/******************************************************************************\
* MouseDownFocusPolicy.hpp                                                     *
* A focus policy that sets the focus on a node that gets "MouseDown" event.    *
*                                                                              *
* Copyright (C) 2006-2010 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _OSGUISH_MOUSE_DOWN_FOCUS_POLICY_HPP_
#define _OSGUISH_MOUSE_DOWN_FOCUS_POLICY_HPP_

#include <OSGUIsh/FocusPolicy.hpp>


namespace OSGUIsh
{
   /**
    * A focus policy that sets the focus on a node that gets a "MouseDown"
    * event.
    */
   class MouseDownFocusPolicy: public FocusPolicy
   {
      public:
         /// Constructs a \c MouseDownFocusPolicy.
         MouseDownFocusPolicy(NodePtr& focusedNode);

         // (inherits documentation)
         virtual void updateFocus(const osgGA::GUIEventAdapter& ea,
                                  const NodePtr nodeUnderMouse);
   };

} // namespace OSGUIsh

#endif // _OSGUISH_MOUSE_DOWN_FOCUS_POLICY_HPP_
