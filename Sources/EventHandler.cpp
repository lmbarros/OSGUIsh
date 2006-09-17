/******************************************************************************\
* EventHandler.cpp                                                             *
* An event handler providing GUI-like events                                   *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include "OSGUIsh/EventHandler.hpp"
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
   void EventHandler::addNode (osg::ref_ptr<osg::Node> node,
                               const std::string& nodeName)
   {
      nodesByName_[nodeName] = node;

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
      const std::string& nodeName, const std::string& eventName)
   {
      NodesByNameMap_t::const_iterator node = nodesByName_.find (nodeName);

      if (node == nodesByName_.end())
      {
         throw std::runtime_error(
            ("Trying to get a signal of an unknown node: '" + nodeName
             + "'.").c_str());
      }

      NameToSignalMap_t::iterator signal =
         signals_[node->second].find (eventName);

      if (signal == signals_[node->second].end())
      {
         throw std::runtime_error (("Trying to get an unknown signal: '"
                                    + eventName + "'.").c_str());
      }

      return signal->second;
   }


} // namespace OSGUIsh
