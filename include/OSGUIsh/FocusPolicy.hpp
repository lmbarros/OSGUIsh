/******************************************************************************\
* FocusPolicy.hpp                                                              *
* An interface used to implement different policies for changing focus from    *
* one node to another.                                                         *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _OSGUISH_FOCUS_POLICY_HPP_
#define _OSGUISH_FOCUS_POLICY_HPP_

#include <boost/shared_ptr.hpp>
#include <osgGA/GUIEventAdapter>
#include <OSGUIsh/Types.hpp>


namespace OSGUIsh
{
   /** An abstract class defining an interface used to implement different
    *  policies for changing focus from one node to another.
    */
   class FocusPolicy
   {
      public:
         /**
          */
         FocusPolicy (NodePtr& focusedNode);

         /**
          */
         virtual void updateFocus (const osgGA::GUIEventAdapter& ea,
                                   const NodePtr nodeUnderMouse) = 0;

      protected:
         ///
         NodePtr getFocusedNode() const { return focusedNode_; }

         ///
         void setFocusedNode (NodePtr focusedNode)
         { focusedNode_ = focusedNode; }

      private:
         NodePtr& focusedNode_;
   };



   /// A (smart) pointer to a \c FocusPolicy.
   typedef boost::shared_ptr<FocusPolicy> FocusPolicyPtr;



   ///
   class FocusPolicyFactory
   {
      public:
         virtual FocusPolicyPtr create (NodePtr& focusedNode) const = 0;
   };


   template <class T>
   class ConcreteFocusPolicyFactory: public FocusPolicyFactory
   {
      public:
         virtual FocusPolicyPtr create (NodePtr& focusedNode) const
         {
            return FocusPolicyPtr (new T(focusedNode));
         }
   };


} // namespace OSGUIsh


#endif // _OSGUISH_FOCUS_POLICY_HPP_
