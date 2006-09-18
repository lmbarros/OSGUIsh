/******************************************************************************\
* EventHandler.cpp                                                             *
* An event handler providing GUI-like events                                   *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include "OSGUIsh/EventHandler.hpp"
#include <boost/lexical_cast.hpp>
#include <osg/io_utils>


namespace OSGUIsh
{

   // - EventHandler::EventHandler ---------------------------------------------
   EventHandler::EventHandler (osgProducer::Viewer& viewer)
      : viewer_(viewer)
   { }



   // - EventHandler::handle ---------------------------------------------------
   bool EventHandler::handle (const osgGA::GUIEventAdapter& ea,
                              osgGA::GUIActionAdapter&)
   {
      switch (ea.getEventType())
      {
         case osgGA::GUIEventAdapter::FRAME:
         {
            hitList_.clear();
            viewer_.computeIntersections (ea.getX(), ea.getY(), hitList_);

            osg::ref_ptr<osg::Node> currentNodeUnderMouse;
            if (hitList_.size() > 0)
            {
                currentNodeUnderMouse = getAddedNode (hitList_[0].getNodePath());

               assert (signals_.find(currentNodeUnderMouse) != signals_.end()
                       && "'getAddedNode()' returned an invalid value!");
            }

            osg::ref_ptr<osg::Node> prevNodeUnderMouse = nodeUnderMouse_;

            nodeUnderMouse_ = currentNodeUnderMouse;

            if (currentNodeUnderMouse == prevNodeUnderMouse)
            {
               // <--- TODO: Don't know the node name...
               // <--- TODO: Must check if the position changed, too.
               if (prevNodeUnderMouse.valid())
                  signals_[currentNodeUnderMouse]["MouseMove"]->operator()(ea, currentNodeUnderMouse);
            }
            else // currentNodeUnderMouse != prevNodeUnderMouse
            {
               if (!currentNodeUnderMouse.valid())
               {
                  signals_[prevNodeUnderMouse]["MouseLeave"]->operator()(ea, prevNodeUnderMouse);
               }
               else if (!prevNodeUnderMouse.valid())
               {
                  signals_[currentNodeUnderMouse]["MouseEnter"]->operator()(ea, currentNodeUnderMouse);
               }
               else // different and non-null
               {
                  signals_[prevNodeUnderMouse]["MouseLeave"]->operator()(ea, prevNodeUnderMouse);
                  signals_[currentNodeUnderMouse]["MouseEnter"]->operator()(ea, currentNodeUnderMouse);
               }
            }

            return handleReturnValues_[osgGA::GUIEventAdapter::FRAME];
         }

         case osgGA::GUIEventAdapter::PUSH:
         {
            std::cerr << "PUSH\n";


            typedef osgUtil::IntersectVisitor::HitList::iterator iter_t;
            for (iter_t p = hitList_.begin(); p != hitList_.end(); ++p)
            {
               if (p->_geode.valid() && !p->_geode->getName().empty())
               {
                  std::cerr << "Geode name: \"" << p->_geode->getName() << "\""
                            << std::endl;
               }
               if (p->_drawable.valid())
               {
                  std::cerr << "Drawable class name: \""
                            << p->_drawable->className() << "\"" << std::endl;
               }

               std::cerr << "        local coords vertex(" << p->getLocalIntersectPoint()
                         << ")" << "  normal(" << p->getLocalIntersectNormal() << ")"
                         << std::endl;

               std::cerr << "        world coords vertex(" << p->getWorldIntersectPoint()
                         << ")" << "  normal(" << p->getWorldIntersectNormal() << ")"
                         << std::endl;

//                osgUtil::Hit::VecIndexList& vil = p->_vecIndexList;
//                for (unsigned int i = 0; i < vil.size(); ++i)
//                {
//                   std::cerr << "        vertex indices [" << i << "] = " << vil[i] << std::endl;
//                }
            }
            return handleReturnValues_[osgGA::GUIEventAdapter::PUSH];
         }

         default:
            return false;
      }

      return false;
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



   // - EventHandler::getAddedNode ---------------------------------------------
   osg::ref_ptr<osg::Node>
   EventHandler::getAddedNode (const osg::NodePath& nodePath)
   {
      typedef osg::NodePath::const_iterator iter_t;
      typedef osg::ref_ptr<osg::Node> nodePtr;
      for (iter_t p = nodePath.begin(); p != nodePath.end(); ++p)
      {
         if (signals_.find (nodePtr(*p)) != signals_.end())
            return nodePtr(*p);
      }

      return nodePtr();
   }


} // namespace OSGUIsh
