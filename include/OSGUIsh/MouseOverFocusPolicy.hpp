/******************************************************************************\
* MouseOverFocusPolicy.hpp                                                     *
* Puts the focus on the node under the mouse.                                  *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _OSGUISH_MOUSE_OVER_FOCUS_POLICY_HPP_
#define _OSGUISH_MOUSE_OVER_FOCUS_POLICY_HPP_

#include <OSGUIsh/FocusPolicy.hpp>


namespace OSGUIsh
{
   /// A focus policy puts the focus on the node under the mouse pointer.
   class MouseOverFocusPolicy: public FocusPolicy
   {
      public:
         /// Constructs a \c MouseOverFocusPolicy.
         MouseOverFocusPolicy (NodePtr& focusedNode);

         // (inherits documentation)
         virtual void updateFocus (const osgGA::GUIEventAdapter& ea,
                                   const NodePtr nodeUnderMouse);
   };

}

#endif // _OSGUISH_MOUSE_OVER_FOCUS_POLICY_HPP_
