/******************************************************************************\
* ManualFocusPolicy.cpp                                                        *
*                                                                              *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <OSGUIsh/ManualFocusPolicy.hpp>


namespace OSGUIsh
{
   // - ManualFocusPolicy::ManualFocusPolicy -----------------------------------
   ManualFocusPolicy::ManualFocusPolicy (NodePtr& focusedNode)
      : FocusPolicy (focusedNode)
   { }



   // - ManualFocusPolicy::updateFocus -----------------------------------------
   void ManualFocusPolicy::updateFocus (const osgGA::GUIEventAdapter& ea,
                                        const NodePtr nodeUnderMouse)
   {
      // Do nothing. That's what "manual" is about.
   }

}
