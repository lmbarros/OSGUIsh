/******************************************************************************\
* MouseOverFocusPolicy.cpp                                                     *
* Puts the focus on the node under the mouse.                                  *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <OSGUIsh/MouseOverFocusPolicy.hpp>


namespace OSGUIsh
{
   // - MouseOverFocusPolicy::MouseOverFocusPolicy -----------------------------
   MouseOverFocusPolicy::MouseOverFocusPolicy (NodePtr& focusedNode)
      : FocusPolicy (focusedNode)
   { }



   // - MouseOverFocusPolicy::updateFocus -----------------------------------------
   void MouseOverFocusPolicy::updateFocus (const osgGA::GUIEventAdapter& ea,
                                           const NodePtr nodeUnderMouse)
   {
      setFocusedNode (nodeUnderMouse);
   }

}
