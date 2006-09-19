/******************************************************************************\
* ManualFocusPolicy.hpp                                                        *
*                                                                              *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _OSGUISH_MANUAL_FOCUS_POLICY_HPP_
#define _OSGUISH_MANUAL_FOCUS_POLICY_HPP_

#include <OSGUIsh/FocusPolicy.hpp>


namespace OSGUIsh
{

   class ManualFocusPolicy: public FocusPolicy
   {
      public:
         ManualFocusPolicy (NodePtr& focusedNode);

         virtual void updateFocus (const osgGA::GUIEventAdapter& ea,
                                   const NodePtr nodeUnderMouse);
   };

}


#endif // _OSGUISH_MANUAL_FOCUS_POLICY_HPP_
