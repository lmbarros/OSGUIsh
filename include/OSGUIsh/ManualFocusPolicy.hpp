/******************************************************************************\
* ManualFocusPolicy.hpp                                                        *
* A focus policy that doesn't change the focus.                                *
*                                                                              *
*                                                                              *
* Copyright (C) 2006 by Leandro Motta Barros.                                  *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _OSGUISH_MANUAL_FOCUS_POLICY_HPP_
#define _OSGUISH_MANUAL_FOCUS_POLICY_HPP_

#include <OSGUIsh/FocusPolicy.hpp>


namespace OSGUIsh
{
   /** A focus policy that doesn't change the focus. That is, it leaves all the
    *  work to be done manually by the programmer.
    */
   class ManualFocusPolicy: public FocusPolicy
   {
      public:
         /// Constructs a \c ManualFocusPolicy.
         ManualFocusPolicy (NodePtr& focusedNode);

         // (inherits documentation)
         virtual void updateFocus (const osgGA::GUIEventAdapter& ea,
                                   const NodePtr nodeUnderMouse);
   };

}

#endif // _OSGUISH_MANUAL_FOCUS_POLICY_HPP_
