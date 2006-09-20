/******************************************************************************\
* MouseDownFocusPolicy.cpp                                                     *
* A focus policy that sets the focus on a node that gets "MouseDown" event.    *
* Leandro Motta Barros                                                         *
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
