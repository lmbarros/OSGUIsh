/******************************************************************************\
* EventHandler.cpp                                                             *
* An event handler providing GUI-like events                                   *
*                                                                              *
* Copyright (C) 2006-2011 by Leandro Motta Barros.                             *
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
      const FocusPolicyFactory& kbdPolicyFactory,
      const FocusPolicyFactory& wheelPolicyFactory,
      double pickerRadius)
      : pickerRadius_(pickerRadius), ignoreBackFaces_(false),
        kbdFocusPolicy_(kbdPolicyFactory.create (kbdFocus_)),
        wheelFocusPolicy_(wheelPolicyFactory.create (wheelFocus_))
   {
      assert(pickerRadius_ >= 0.0 && "Cannot use negative picker radius");

      addNode (NodePtr());

      for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
      {
         nodeThatGotMouseDown_[i] = NodePtr();
         nodeThatGotClick_[i] = NodePtr();
         timeOfLastClick_[i] = -1.0;
      }

      pickingMasks_.push_back(0xFFFFFFFF); // by default, no restrictions
   }



   // - EventHandler::handle ---------------------------------------------------
   bool EventHandler::handle(const osgGA::GUIEventAdapter& ea,
                             osgGA::GUIActionAdapter& aa)
   {
      switch (ea.getEventType())
      {
         case osgGA::GUIEventAdapter::FRAME:
         {
            osg::View* view = dynamic_cast<osg::View*>(&aa);
            assert(view != 0 && "Needed an osg::View here.");
            handleFrameEvent(view, ea);
            break;
         }

         case osgGA::GUIEventAdapter::PUSH:
            handlePushEvent(ea);
            break;

         case osgGA::GUIEventAdapter::RELEASE:
            handleReleaseEvent(ea);
            break;

         case osgGA::GUIEventAdapter::KEYDOWN:
            handleKeyDownEvent(ea);
            break;

         case osgGA::GUIEventAdapter::KEYUP:
            handleKeyUpEvent(ea);
            break;

         case osgGA::GUIEventAdapter::SCROLL:
            handleScrollEvent(ea);
            break;

         default:
            break;
      }

      kbdFocusPolicy_->updateFocus(ea, nodeUnderMouse_);
      wheelFocusPolicy_->updateFocus(ea, nodeUnderMouse_);

      return handleReturnValues_[ea.getEventType()];
   }



   // - EventHandler::setPickingMask -------------------------------------------
   void EventHandler::setPickingMask(osg::Node::NodeMask newMask)
   {
      std::vector<osg::Node::NodeMask> masks;
      pickingMasks_ = std::vector<osg::Node::NodeMask>();
      pickingMasks_.push_back(newMask);
   }



   // - EventHandler::setPickingMasks ------------------------------------------
   void EventHandler::setPickingMasks(osg::Node::NodeMask newMask1,
                                      osg::Node::NodeMask newMask2)
   {
      std::vector<osg::Node::NodeMask> masks;
      pickingMasks_ = std::vector<osg::Node::NodeMask>();
      pickingMasks_.push_back(newMask1);
      pickingMasks_.push_back(newMask2);
   }



   // - EventHandler::setPickingMasks ------------------------------------------
   void EventHandler::setPickingMasks(osg::Node::NodeMask newMask1,
                                      osg::Node::NodeMask newMask2,
                                      osg::Node::NodeMask newMask3)
   {
      std::vector<osg::Node::NodeMask> masks;
      pickingMasks_ = std::vector<osg::Node::NodeMask>();
      pickingMasks_.push_back(newMask1);
      pickingMasks_.push_back(newMask2);
      pickingMasks_.push_back(newMask3);
   }



   // - EventHandler::setPickingMasks ------------------------------------------
   void EventHandler::setPickingMasks(
      const std::vector<osg::Node::NodeMask>& newMasks)
   {
      pickingMasks_ = newMasks;
   }



   // - EventHandler::addNode --------------------------------------------------
   void EventHandler::addNode(const osg::ref_ptr<osg::Node> node)
   {
#     define OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT) \
         signals_[node][EVENT] = SignalPtr(new EventHandler::Signal_t());

      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_ENTER);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_LEAVE);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_MOVE);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_DOWN);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_UP);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_CLICK);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_DOUBLE_CLICK);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_WHEEL_UP);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_MOUSE_WHEEL_DOWN);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_KEY_UP);
      OSGUISH_EVENTHANDLER_ADD_EVENT(EVENT_KEY_DOWN);

#     undef OSGUISH_EVENTHANDLER_ADD_EVENT
   }



   // - EventHandler::getSignal ------------------------------------------------
   EventHandler::SignalPtr EventHandler::getSignal(NodePtr node, Event signal)
   {
       SignalsMap_t::const_iterator signalsCollectionIter =
          signals_.find (node);

      if (signalsCollectionIter == signals_.end())
      {
         throw std::runtime_error(
            ("Trying to get a signal of an unknown node: '" + node->getName()
             + "' (" + boost::lexical_cast<std::string>(node) + ").").c_str());
      }

      SignalCollection_t::const_iterator signalIter =
         signalsCollectionIter->second.find(signal);

      assert(signalIter != signalsCollectionIter->second.end()
             && "Trying to get an unknown signal.");

      return signalIter->second;
   }



   // - EventHandler::setKeyboardFocus -----------------------------------------
   void EventHandler::setKeyboardFocus(const NodePtr node)
   {
      kbdFocus_ = node;
   }



   // - EventHandler::setMouseWheelFocus ---------------------------------------
   void EventHandler::setMouseWheelFocus(const NodePtr node)
   {
      wheelFocus_ = node;
   }



   // - EventHandler::setKeyboardFocusPolicy -----------------------------------
   void EventHandler::setKeyboardFocusPolicy(
      const FocusPolicyFactory& policyFactory)
   {
      kbdFocusPolicy_ = policyFactory.create(kbdFocus_);
   }



   // - EventHandler::setMouseWheelFocusPolicy ---------------------------------
   void EventHandler::setMouseWheelFocusPolicy(
      const FocusPolicyFactory& policyFactory)
   {
      wheelFocusPolicy_ = policyFactory.create(wheelFocus_);
   }



   // - EventHandler::getObservedNode ------------------------------------------
   NodePtr EventHandler::getObservedNode(const osg::NodePath& nodePath)
   {
      typedef osg::NodePath::const_reverse_iterator iter_t;
      for (iter_t p = nodePath.rbegin(); p != nodePath.rend(); ++p)
      {
         if (signals_.find(NodePtr(*p)) != signals_.end())
            return NodePtr(*p);
      }

      return NodePtr();
   }



   // - EventHandler::handleFrameEvent -----------------------------------------
   void EventHandler::handleFrameEvent(osg::View* view,
                                       const osgGA::GUIEventAdapter& ea)
   {
      assert(pickingMasks_.size() > 0);

      updatePickingData(view, ea);

      // Trigger the events
      if (nodeUnderMouse_ == prevNodeUnderMouse_)
      {
         if (prevNodeUnderMouse_.valid()
             && positionUnderMouse_ != prevPositionUnderMouse_)
         {
            HandlerParams params (nodeUnderMouse_, ea, hitUnderMouse_);
            signals_[nodeUnderMouse_][EVENT_MOUSE_MOVE]->operator()(params);
         }
      }
      else // nodeUnderMouse != prevNodeUnderMouse_
      {
         if (prevNodeUnderMouse_.valid())
         {
            HandlerParams params (prevNodeUnderMouse_, ea, hitUnderMouse_);
            signals_[prevNodeUnderMouse_][EVENT_MOUSE_LEAVE]
               ->operator()(params);
         }

         if (nodeUnderMouse_.valid())
         {
            HandlerParams params (nodeUnderMouse_, ea, hitUnderMouse_);
            signals_[nodeUnderMouse_][EVENT_MOUSE_ENTER]->operator()(params);
         }
      }
   }



   // - EventHandler::handlePushEvent ------------------------------------------
   void EventHandler::handlePushEvent(const osgGA::GUIEventAdapter& ea)
   {
      // Trigger a "MouseDown" signal.
      if (nodeUnderMouse_.valid())
      {
         HandlerParams params (nodeUnderMouse_, ea, hitUnderMouse_);
         signals_[nodeUnderMouse_][EVENT_MOUSE_DOWN]->operator()(params);
      }

      // Do the bookkeeping for "Click" and "DoubleClick"
      MouseButton button = getMouseButton (ea);
      nodeThatGotMouseDown_[button] = nodeUnderMouse_;
   }



   // - EventHandler::handleReleaseEvent ---------------------------------------
   void EventHandler::handleReleaseEvent(const osgGA::GUIEventAdapter& ea)
   {
      const double DOUBLE_CLICK_INTERVAL = 0.3;

      if (nodeUnderMouse_.valid())
      {
         MouseButton button = getMouseButton (ea);

         // First the trivial case: the "MouseUp" event
         HandlerParams params(nodeUnderMouse_, ea, hitUnderMouse_);
         signals_[nodeUnderMouse_][EVENT_MOUSE_UP]->operator()(params);

         // Now, the trickier ones: "Click" and "DoubleClick"
         if (nodeUnderMouse_ == nodeThatGotMouseDown_[button])
         {
            HandlerParams params(nodeUnderMouse_, ea, hitUnderMouse_);
            signals_[nodeUnderMouse_][EVENT_CLICK]->operator()(params);

            const double now = ea.getTime();

            if (now - timeOfLastClick_[button] < DOUBLE_CLICK_INTERVAL
                && nodeUnderMouse_ == nodeThatGotClick_[button])
            {
               HandlerParams params (nodeUnderMouse_, ea, hitUnderMouse_);
               signals_[nodeUnderMouse_][EVENT_DOUBLE_CLICK]
                  ->operator()(params);
            }

            nodeThatGotClick_[button] = nodeUnderMouse_;
            timeOfLastClick_[button] = now;
         }
      }
   }



   // - EventHandler::handleKeyDownEvent ---------------------------------------
   void EventHandler::handleKeyDownEvent(const osgGA::GUIEventAdapter& ea)
   {
      HandlerParams params(kbdFocus_, ea, hitUnderMouse_);
      signals_[kbdFocus_][EVENT_KEY_DOWN]->operator()(params);
   }



   // - EventHandler::handleKeyUpEvent -----------------------------------------
   void EventHandler::handleKeyUpEvent(const osgGA::GUIEventAdapter& ea)
   {
      HandlerParams params(kbdFocus_, ea, hitUnderMouse_);
      signals_[kbdFocus_][EVENT_KEY_UP]->operator()(params);
   }



   // - EventHandler::handleScrollEvent ----------------------------------------
   void EventHandler::handleScrollEvent(const osgGA::GUIEventAdapter& ea)
   {
      switch (ea.getScrollingMotion())
      {
         case osgGA::GUIEventAdapter::SCROLL_UP:
         {
            HandlerParams params(wheelFocus_, ea, hitUnderMouse_);
            signals_[wheelFocus_][EVENT_MOUSE_WHEEL_UP]->operator()(params);
            break;
         }

         case osgGA::GUIEventAdapter::SCROLL_DOWN:
         {
            HandlerParams params(wheelFocus_, ea, hitUnderMouse_);
            signals_[wheelFocus_][EVENT_MOUSE_WHEEL_DOWN]->operator()(params);
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
            assert(false && "Got an invalid mouse button code. Is 'ea' really "
                   "a mouse event?");
         }
      }
   }



   // - EventHandler::updatePickingData ----------------------------------------
   void EventHandler::updatePickingData(
      osg::View* view, const osgGA::GUIEventAdapter& ea)
   {
      assert(pickerRadius_ >= 0.0 && "Cannot use negative picker radius");

      if (pickerRadius_ > 0.0)
         updatePickingDataPolytope(view, ea);
      else
         updatePickingDataLine(view, ea);
   }



   // - EventHandler::updatePickingDataLine ------------------------------------
   void EventHandler::updatePickingDataLine(
      osg::View* view, const osgGA::GUIEventAdapter& ea)
   {
      const osg::Viewport* vp = view->getCamera()->getViewport();

      const float x = vp->x() + static_cast<int>(
         vp->width() * (ea.getXnormalized() * 0.5f + 0.5f));
      const float y = vp->y() + static_cast<int>(
         vp->height() * (ea.getYnormalized() * 0.5f + 0.5f));

      NodePtr currentNodeUnderMouse;
      osg::Vec3 currentPositionUnderMouse;

      typedef NodeMasks_t::const_iterator iter_t;
      for (iter_t p = pickingMasks_.begin(); p != pickingMasks_.end(); ++p)
      {
         osg::ref_ptr<osgUtil::LineSegmentIntersector> picker(
            new osgUtil::LineSegmentIntersector(
               osgUtil::Intersector::WINDOW, x, y));

         osgUtil::IntersectionVisitor iv(picker);
         iv.setTraversalMask(*p);

         view->getCamera()->accept(iv);

         const osgUtil::LineSegmentIntersector::Intersections hitList =
            picker->getIntersections();

         if (hitList.size() > 0)
         {
            typedef
               osgUtil::LineSegmentIntersector::Intersections::const_iterator
               iter_t;

            iter_t theHit = hitList.end();

            // This implementation needs two or more hits to correctly ignore
            // back faces. There is a little more detail about this in the
            // Doxygen comments for \c ignoreBackFaces().
            if (ignoreBackFaces_ && hitList.size() >= 2)
            {
               const osg::Vec3 begin =
                  hitList.begin()->getWorldIntersectPoint();
               const osg::Vec3 end =
                  (--hitList.end())->getWorldIntersectPoint();

               osg::Vec3 rayDir = end - begin;
               rayDir.normalize();

               for (iter_t hit = hitList.begin(); hit != hitList.end(); ++hit)
               {
                  const bool frontFacing =
                     rayDir * hit->getWorldIntersectNormal() < 0.0;

                  if (frontFacing)
                  {
                     theHit = hit;
                     break;
                  }
               }
            }
            else // !ignoreBackFaces_
            {
               theHit = hitList.begin();
            }

            if (theHit != hitList.end())
            {
               currentNodeUnderMouse = getObservedNode (theHit->nodePath);
               assert (signals_.find (currentNodeUnderMouse) != signals_.end()
                       && "'getObservedNode()' returned an invalid value!");

               currentPositionUnderMouse = theHit->getLocalIntersectPoint();

               hitUnderMouse_ = *theHit;

               break;
            }
         }  // if (hitList.size() > 0)
      } // for (...pickingMasks_...)

      prevNodeUnderMouse_ = nodeUnderMouse_;
      prevPositionUnderMouse_ = positionUnderMouse_;

      nodeUnderMouse_ = currentNodeUnderMouse;
      positionUnderMouse_ = currentPositionUnderMouse;
   }



   // - EventHandler::updatePickingDataPolytope --------------------------------
   void EventHandler::updatePickingDataPolytope(
      osg::View* view, const osgGA::GUIEventAdapter& ea)
   {
   }

} // namespace OSGUIsh
