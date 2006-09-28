/******************************************************************************\
* EventHandler.cpp                                                             *
* An event handler providing GUI-like events                                   *
*                                                                              *
*                                                                              *
* Copyright (C) 2006 by Leandro Motta Barros.                                  *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#include "OSGUIsh/EventHandler.hpp"
#include <boost/lexical_cast.hpp>


namespace OSGUIsh
{

   // - EventHandler::EventHandler ---------------------------------------------
   EventHandler::EventHandler(
      osgProducer::Viewer& viewer,
      const FocusPolicyFactory& kbdPolicyFactory,
      const FocusPolicyFactory& wheelPolicyFactory)
      : viewer_(viewer),
        kbdFocusPolicy_(kbdPolicyFactory.create (kbdFocus_)),
        wheelFocusPolicy_(wheelPolicyFactory.create (wheelFocus_))
   {
      addNode (NodePtr());

      for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
      {
         nodeThatGotMouseDown_[i] = NodePtr();
         nodeThatGotClick_[i] = NodePtr();
         timeOfLastClick_[i] = -1.0;
      }
   }



   // - EventHandler::handle ---------------------------------------------------
   bool EventHandler::handle (const osgGA::GUIEventAdapter& ea,
                              osgGA::GUIActionAdapter&)
   {
      switch (ea.getEventType())
      {
         case osgGA::GUIEventAdapter::FRAME:
            handleFrameEvent (ea);
            break;

         case osgGA::GUIEventAdapter::PUSH:
            handlePushEvent (ea);
            break;

         case osgGA::GUIEventAdapter::RELEASE:
            handleReleaseEvent (ea);
            break;

         case osgGA::GUIEventAdapter::KEYDOWN:
            handleKeyDownEvent (ea);
            break;

         case osgGA::GUIEventAdapter::KEYUP:
            handleKeyUpEvent (ea);
            break;

         case osgGA::GUIEventAdapter::SCROLL:
            handleScrollEvent (ea);
            break;

         default:
            break;
      }

      kbdFocusPolicy_->updateFocus (ea, nodeUnderMouse_);
      wheelFocusPolicy_->updateFocus (ea, nodeUnderMouse_);

      return handleReturnValues_[ea.getEventType()];
   }



   // - EventHandler::addNode --------------------------------------------------
   void EventHandler::addNode (const osg::ref_ptr<osg::Node> node)
   {
#     define OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT) \
         signals_[node][#EVENT] = SignalPtr (new EventHandler::Signal_t());

      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseEnter);
      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseLeave);
      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseMove);
      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseDown);
      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseUp);
      OSGUISH_EVENTHANDLER_ADD_EVENT (Click);
      OSGUISH_EVENTHANDLER_ADD_EVENT (DoubleClick);
      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseWheelUp);
      OSGUISH_EVENTHANDLER_ADD_EVENT (MouseWheelDown);
      OSGUISH_EVENTHANDLER_ADD_EVENT (KeyUp);
      OSGUISH_EVENTHANDLER_ADD_EVENT (KeyDown);

#     undef OSGUISH_EVENTHANDLER_ADD_EVENT
   }


   // - EventHandler::getSignal ------------------------------------------------
   EventHandler::SignalPtr EventHandler::getSignal(
      const NodePtr node, const std::string& signal)
   {
       SignalsMap_t::const_iterator signalsCollectionIter =
          signals_.find (node);

      if (signalsCollectionIter == signals_.end())
      {
         throw std::runtime_error(
            ("Trying to get a signal of an unknown node: '" + node->getName()
             + "' (" + boost::lexical_cast<std::string>(node.get())
             + ").").c_str());
      }

      SignalCollection_t::const_iterator signalIter =
         signalsCollectionIter->second.find (signal);

      if (signalIter == signalsCollectionIter->second.end())
      {
         throw std::runtime_error (("Trying to get an unknown signal: '"
                                    + signal + "'.").c_str());
      }

      return signalIter->second;
   }



   // - EventHandler::setKeyboardFocus -----------------------------------------
   void EventHandler::setKeyboardFocus (const NodePtr node)
   {
      kbdFocus_ = node;
   }



   // - EventHandler::setMouseWheelFocus ---------------------------------------
   void EventHandler::setMouseWheelFocus (const NodePtr node)
   {
      wheelFocus_ = node;
   }



   // - EventHandler::setKeyboardFocusPolicy -----------------------------------
   void EventHandler::setKeyboardFocusPolicy(
      const FocusPolicyFactory& policyFactory)
   {
      kbdFocusPolicy_ = policyFactory.create (kbdFocus_);
   }



   // - EventHandler::setMouseWheelFocusPolicy ---------------------------------
   void EventHandler::setMouseWheelFocusPolicy(
      const FocusPolicyFactory& policyFactory)
   {
      wheelFocusPolicy_ = policyFactory.create (wheelFocus_);
   }



   // - EventHandler::getObservedNode ------------------------------------------
   NodePtr EventHandler::getObservedNode (const osg::NodePath& nodePath)
   {
      typedef osg::NodePath::const_reverse_iterator iter_t;
      for (iter_t p = nodePath.rbegin(); p != nodePath.rend(); ++p)
      {
         if (signals_.find (NodePtr(*p)) != signals_.end())
            return NodePtr(*p);
      }

      return NodePtr();
   }



   // - EventHandler::handleFrameEvent -----------------------------------------
   void EventHandler::handleFrameEvent (const osgGA::GUIEventAdapter& ea)
   {
      // Find out who is, and who was under the mouse pointer
      hitList_.clear();
      viewer_.computeIntersections (ea.getX(), ea.getY(), hitList_);

      NodePtr currentNodeUnderMouse;
      osg::Vec3 currentPositionUnderMouse;

      if (hitList_.size() > 0)
      {
         currentNodeUnderMouse = getObservedNode (hitList_[0].getNodePath());
         assert (signals_.find (currentNodeUnderMouse) != signals_.end()
                 && "'getObservedNode()' returned an invalid value!");

         currentPositionUnderMouse = hitList_[0].getLocalIntersectPoint();
      }

      NodePtr prevNodeUnderMouse = nodeUnderMouse_;
      osg::Vec3 prevPositionUnderMouse = positionUnderMouse_;

      nodeUnderMouse_ = currentNodeUnderMouse;
      positionUnderMouse_ = currentPositionUnderMouse;

      // Trigger the events
      if (currentNodeUnderMouse == prevNodeUnderMouse)
      {
         if (prevNodeUnderMouse.valid()
             && currentPositionUnderMouse != prevPositionUnderMouse)
         {
            signals_[currentNodeUnderMouse]["MouseMove"]->operator()(
               ea, currentNodeUnderMouse);
         }
      }
      else // currentNodeUnderMouse != prevNodeUnderMouse
      {
         if (prevNodeUnderMouse.valid())
         {
            signals_[prevNodeUnderMouse]["MouseLeave"]->operator()(
               ea, prevNodeUnderMouse);
         }

         if (currentNodeUnderMouse.valid())
         {
            signals_[currentNodeUnderMouse]["MouseEnter"]->operator()(
               ea, currentNodeUnderMouse);
         }
      }
   }



   // - EventHandler::handlePushEvent ------------------------------------------
   void EventHandler::handlePushEvent (const osgGA::GUIEventAdapter& ea)
   {
      // Trigger a "MouseDown" signal.
      if (nodeUnderMouse_.valid())
      {
         signals_[nodeUnderMouse_]["MouseDown"]->operator()(
            ea, nodeUnderMouse_);
      }

      // Do the bookkeeping for "Click" and "DoubleClick"
      MouseButton button = getMouseButton (ea);
      nodeThatGotMouseDown_[button] = nodeUnderMouse_;
   }



   // - EventHandler::handleReleaseEvent ---------------------------------------
   void EventHandler::handleReleaseEvent (const osgGA::GUIEventAdapter& ea)
   {
      const double DOUBLE_CLICK_INTERVAL = 0.3;

      if (nodeUnderMouse_.valid())
      {
         MouseButton button = getMouseButton (ea);

         // First the trivial case: the "MouseUp" event
         signals_[nodeUnderMouse_]["MouseUp"]->operator()(
            ea, nodeUnderMouse_);

         // Now, the trickier ones: "Click" and "DoubleClick"
         if (nodeUnderMouse_ == nodeThatGotMouseDown_[button])
         {
            signals_[nodeUnderMouse_]["Click"]->operator()(
               ea, nodeUnderMouse_);

            const double now = ea.getTime();

            if (now - timeOfLastClick_[button] < DOUBLE_CLICK_INTERVAL
                && nodeUnderMouse_ == nodeThatGotClick_[button])
            {
               signals_[nodeUnderMouse_]["DoubleClick"]->operator()(
                  ea, nodeUnderMouse_);
            }

            nodeThatGotClick_[button] = nodeUnderMouse_;
            timeOfLastClick_[button] = now;
         }
      }
   }



   // - EventHandler::handleKeyDownEvent ---------------------------------------
   void EventHandler::handleKeyDownEvent (const osgGA::GUIEventAdapter& ea)
   {
      signals_[kbdFocus_]["KeyDown"]->operator()(ea, kbdFocus_);
   }



   // - EventHandler::handleKeyUpEvent -----------------------------------------
   void EventHandler::handleKeyUpEvent (const osgGA::GUIEventAdapter& ea)
   {
      signals_[kbdFocus_]["KeyUp"]->operator()(ea, kbdFocus_);
   }



   // - EventHandler::handleScrollEvent ----------------------------------------
   void EventHandler::handleScrollEvent (const osgGA::GUIEventAdapter& ea)
   {
      switch (ea.getScrollingMotion())
      {
         case osgGA::GUIEventAdapter::SCROLL_UP:
         {
            signals_[wheelFocus_]["MouseWheelUp"]->operator()(ea, wheelFocus_);
            break;
         }

         case osgGA::GUIEventAdapter::SCROLL_DOWN:
         {
            signals_[wheelFocus_]["MouseWheelDown"]->operator()(ea, wheelFocus_);
            break;
         }

         default:
            break; // ignore other events
      }
   }

   // - EventHandler::getMouseButton -------------------------------------------
   EventHandler::MouseButton EventHandler::getMouseButton(
      const osgGA::GUIEventAdapter& ea)
   {
      switch (ea.getButton())
      {
         case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
            return LEFT_MOUSE_BUTTON;
         case osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON:
            return MIDDLE_MOUSE_BUTTON;
         case osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON:
            return RIGHT_MOUSE_BUTTON;
         default:
         {
            assert (false && "Got an invalid mouse button code. Is 'ea' really "
                    "a mouse event?");
         }
      }
   }

} // namespace OSGUIsh
