/******************************************************************************\
* EventHandler.hpp                                                             *
* An event handler providing GUI-like events                                   *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _GUISH_EVENT_HANDLER_HPP_
#define _GUISH_EVENT_HANDLER_HPP_


#include <boost/signal.hpp>
#include <osgProducer/Viewer>


namespace OSGUIsh
{

   /** An event handler providing GUI-like events for nodes.
    */
   class EventHandler: public osgGA::GUIEventHandler
   {
      public:
         EventHandler (osgProducer::Viewer& viewer);

         bool handle (const osgGA::GUIEventAdapter& ea,
                      osgGA::GUIActionAdapter&);


         typedef boost::signal<void (osgGA::GUIEventAdapter*, std::string)> Signal_t;

         void addNode (osg::ref_ptr<osg::Node> node, const std::string& nodeName);

         Signal_t& getSignal (const std::string& nodeName,
                              const std::string& eventName);


      private:
         osgProducer::Viewer& viewer_;

         osgUtil::IntersectVisitor::HitList hitList_;

         /** @todo Make the return value of \c handle() configurable "per event
          *        type". For example: "please return \c false when a \c PUSH
          *        event happens (even if it is handled). So far, this is
          *        uninitialized, so \c false is always returned.
          */
         std::map <osgGA::GUIEventAdapter::EventType, bool> handleReturnValues_;

         // maps name to node
         std::map <std::string, osg::ref_ptr<osg::Node> > nodesByName_;

         // node -> event name -> signal
         std::map <osg::ref_ptr<osg::Node>, std::map <std::string, Signal_t> > events_;

         osg::ref_ptr<osg::Node> focusedNode_;
   };

} // namespace OSGUIsh


#endif // _GUISH_EVENT_HANDLER_HPP_
