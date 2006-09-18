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

   /// A (smart) pointer to a scene graph node.
   typedef osg::ref_ptr<osg::Node> NodePtr;


   /// An event handler providing GUI-like events for nodes.
   class EventHandler: public osgGA::GUIEventHandler
   {
      public:
         /** Constructs an \c EventHandler.
          *  @param viewer The viewer used to view the scene graph. This used
          *         for calling its \c computeIntersections() method.
          */
         EventHandler (osgProducer::Viewer& viewer);

         /// Handle upcoming events (overloads virtual method).
         bool
         handle (const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter&);

         /** A type representing a signal used in OSGUIsh. This signal returns
          *  nothing and takes two parameters: the \c osgGA::GUIEventAdapter
          *  that generated this event and a pointer to the node receiving the
          *  event.
          */
         typedef boost::signal<void (const osgGA::GUIEventAdapter&,
                                     NodePtr)> Signal_t;

         /// A (smart) pointer to a \c Signal_t;
         typedef boost::shared_ptr<Signal_t> SignalPtr;

         /** Put a node under vigilance of this \c EventHandler, that is, make
          *  it capable of triggering OSGUIsh signals.
          *  @param node The node that will be added to this \c EventHandler.
          */
         void addNode (const NodePtr node);

         /** Returns a signal associated with a given node. This is typically
          *  used to call \c connect() on the returned signal.
          *  @param node The desired node.
          *  @param signal The desired signal. Valid signals are
          *         <tt>"MouseEnter"</tt>, <tt>"MouseLeave"</tt>,
          *         <tt>"MouseMove"</tt>, <tt>"MouseDown"</tt>,
          *         <tt>"MouseUp"</tt>, <tt>"Click"</tt>,
          *         <tt>"DoubleClick"</tt>, <tt>"MouseWheelUp"</tt>,
          *         <tt>"MouseWheelDown"</tt>, <tt>"KeyUp"</tt> and
          *         <tt>"KeyDown"</tt>.
          */
         SignalPtr getSignal (const NodePtr node, const std::string& signal);

      private:

         // Goes up the nodepath until finding a registered node. Returns null
         // if couldn't find any. BAD FUNCTION NAME!
         NodePtr getAddedNode (const osg::NodePath& nodePath);

         osgProducer::Viewer& viewer_;

         osgUtil::IntersectVisitor::HitList hitList_;

         /** @todo Make the return value of \c handle() configurable "per event
          *        type". For example: "please return \c false when a \c PUSH
          *        event happens (even if it is handled). So far, this is
          *        uninitialized, so \c false is always returned.
          */
         std::map <osgGA::GUIEventAdapter::EventType, bool> handleReturnValues_;

         // maps name to node
         typedef std::map <std::string, NodePtr> NodesByNameMap_t;

         /// Type mapping a signal name to the signal object.
         typedef std::map <std::string, SignalPtr> SignalCollection_t;

         /// Type mapping nodes to the collection of signals associated to it.
         typedef std::map <NodePtr, SignalCollection_t > SignalsMap_t;

         /** Structure containing all the signals used by this \c EventHandler.
          *  It can be used with a syntax like this:
          *  <tt>signals_[node]["MouseMove"]->connect(&MyHandler);</tt>
          */
         SignalsMap_t signals_;

         //
         // For "MouseEnter", "MouseLeave", "MouseMove"
         //

         /// The node currently under the mouse pointer.
         NodePtr nodeUnderMouse_;

         /** The position (in the object coordinate system) of
          *  \c nodeUnderMouse_ currently under the mouse pointer.
          */
         osg::Vec3 positionUnderMouse_;
   };

} // namespace OSGUIsh


#endif // _GUISH_EVENT_HANDLER_HPP_
