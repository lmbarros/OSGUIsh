/******************************************************************************\
* MouseDownFocusPolicy.hpp                                                     *
* A focus policy that sets the focus on a node that gets "MouseDown" event.    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _OSGUISH_MOUSE_DOWN_FOCUS_POLICY_HPP_
#define _OSGUISH_MOUSE_DOWN_FOCUS_POLICY_HPP_

#include <OSGUIsh/FocusPolicy.hpp>


namespace OSGUIsh
{
   /** A focus policy that sets the focus on a node that gets a "MouseDown"
    *  event.
    */
   class MouseDownFocusPolicy: public FocusPolicy
   {
      public:
         /// Constructs a \c MouseDownFocusPolicy.
         MouseDownFocusPolicy (NodePtr& focusedNode);

         // (inherits documentation)
         virtual void updateFocus (const osgGA::GUIEventAdapter& ea,
                                   const NodePtr nodeUnderMouse);
   };

}

#endif // _OSGUISH_MOUSE_DOWN_FOCUS_POLICY_HPP_
