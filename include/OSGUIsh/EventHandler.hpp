/******************************************************************************\
* EventHandler.hpp                                                             *
* An event handler providing GUI-like events                                   *
*                                                                              *
*                                                                              *
* Copyright (C) 2006 by Leandro Motta Barros.                                  *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _GUISH_EVENT_HANDLER_HPP_
#define _GUISH_EVENT_HANDLER_HPP_

#include <boost/signal.hpp>
#include <osgProducer/Viewer>
#include <OSGUIsh/FocusPolicy.hpp>
#include <OSGUIsh/ManualFocusPolicy.hpp>


namespace OSGUIsh
{
   /** An event handler providing GUI-like events for nodes. The \c EventHandler
    *  has an internal list of nodes being "observed". Every observed node has a
    *  collection of signals associated to it. These signals represent the
    *  events that can be generated for the node.
    */
   class EventHandler: public osgGA::GUIEventHandler
   {
      public:
         /** Constructs an \c EventHandler.
          *  @param viewer The viewer used to view the scene graph. This used
          *         for calling its \c computeIntersections() method.
          *  @param kbdPolicyFactory The factory that will be used to create the
          *         \c FocusPolicy used to automatically set the focus for
          *         keyboard events.
          *  @param wheelPolicyFactory The factory that will be used to create
          *         the \c FocusPolicy used to automatically set the focus for
          *         mouse wheel events.
          */
         EventHandler (osgProducer::Viewer& viewer,
                       const FocusPolicyFactory& kbdPolicyFactory =
                       FocusPolicyFactoryMason<ManualFocusPolicy>(),
                       const FocusPolicyFactory& wheelPolicyFactory =
                       FocusPolicyFactoryMason<ManualFocusPolicy>());

         /** Handles upcoming events (overloads virtual method).
          *  @return See \c handleReturnValues_ please.
          */
         bool handle (const osgGA::GUIEventAdapter& ea,
                      osgGA::GUIActionAdapter&);

         /** A type representing a signal used in OSGUIsh. This signal returns
          *  nothing and takes three parameters: a pointer to the node receiving
          *  the event, the \c osgGA::GUIEventAdapter that generated this event,
          *  and the \c osgUtil::Hit for the node under the mouse pointer.
          *  @note Be aware that, in the general case,
          *        <tt>hit.getNodePath().back() != node</tt>. The hit will
          *        contain the actual, "low level" hit, while \c node will
          *        contain the node registered with OSGUish. For instance, if
          *        you register "car" node, that has a "car body" and four
          *        "wheels" as subnodes, the \c node will always be the "car",
          *        while the hit will be the "car body" or one of the "wheels".
          *  @note Notice that, in some cases, the \c osgUtil::Hit for the node
          *        under the mouse pointer doesn't really have something to do
          *        with the event being handled. For example, if the event is a
          *        <tt>KeyUp</tt> and the focus policy is not the "node under
          *        mouse has focus" policy, than the hit will not be related to
          *        the node generating the event.
          *        <p>Also, in cases like the above, the hit may even be
          *        invalid, since, perhaps, there will not be any registered
          *        node under the mouse pointer.
          */
         typedef boost::signal<void (NodePtr,
                                     const osgGA::GUIEventAdapter&,
                                     const osgUtil::Hit& hit
                                     )> Signal_t;

         /// A (smart) pointer to a \c Signal_t;
         typedef boost::shared_ptr<Signal_t> SignalPtr;

         /** Adds a given node to the list of nodes being "observed" by this
          *  \c EventHandler. In other words, signals for this node will be
          *  triggered from this call on.
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

         /** Ignores or stops to ignore faces that are back-facing the viewer
          *  when picking. It may be useful to ignore back faces when backface
          *  culling is enabled.
          *  @param ignore If \c true, back faces will be ignored when picking.
          *         If \c false, back faces will be considered when piking.
          */
         void ignoreBackFaces (bool ignore = true)
         { ignoreBackFaces_ = ignore; }

         /// Sets the node that will receive keyboard events.
         void setKeyboardFocus (const NodePtr node);

         /// Sets the node that will receive mouse wheel events.
         void setMouseWheelFocus (const NodePtr node);

         /** Sets the focus policy for keyboard events to a given one.
          *  @param policyFactory The factory that will be used to create the
          *         actual policy.
          */
         void setKeyboardFocusPolicy (const FocusPolicyFactory& policyFactory);

         /** Sets the focus policy for mouse wheel events to a given one.
          *  @param policyFactory The factory that will be used to create the
          *         actual policy.
          */
         void setMouseWheelFocusPolicy (const FocusPolicyFactory& policyFactory);

      private:
         /** Returns the first node in an \c osg::NodePath that is present in the
          *  list of nodes being "observed" by this \c EventHandler. This is
          *  necessary in the cases in which the user is picking a node that is
          *  child of an added node. That's the case, for instance, of when the
          *  user adds a node read from a 3D model file returned by
          *  \c osgDB::readNodeFile().
          *  @param nodePath The node path leading to the node being queried.
          *  @returns The first in \c nodePath that as added to the list of
          *           nodes being observed.
          */
         NodePtr getObservedNode (const osg::NodePath& nodePath);

         /** Handles a \c FRAME event triggered by Producer. Signals triggered
          *  here are <tt>"MouseEnter"</tt>, <tt>"MouseLeave"</tt> and
          *  <tt>"MouseMove"</tt>.
          *  @param ea The event generated by Producer.
          */
         void handleFrameEvent (const osgGA::GUIEventAdapter& ea);

         /** Handles a \c PUSH event triggered by Producer. The only signal
          *  triggered here is <tt>"MouseDown"</tt>, but this function also does
          *  bookkeeping related to other mouse signals.
          *  @param ea The event generated by Producer.
          */
         void handlePushEvent (const osgGA::GUIEventAdapter& ea);

         /** Handles a \c RELEASE event triggered by Producer. Signals triggered
          *  here are <tt>"MouseUp"</tt>, <tt>"Click"</tt> and
          *  <tt>"DoubleClick"</tt>.
          *  @param ea The event generated by Producer.
          */
         void handleReleaseEvent (const osgGA::GUIEventAdapter& ea);

         /** Handles a \c KEYDOWN event triggered by Producer. The only signal
          *  triggered here is <tt>"KeyDown"</tt>.
          *  @param ea The event generated by Producer.
          */
         void handleKeyDownEvent (const osgGA::GUIEventAdapter& ea);

         /** Handles a \c KEYUP event triggered by Producer. The only signal
          *  triggered here is <tt>"KeyUp"</tt>.
          *  @param ea The event generated by Producer.
          */
         void handleKeyUpEvent (const osgGA::GUIEventAdapter& ea);

         /** Handles a \c SCROLL event triggered by Producer. The signal
          *  triggered here are <tt>"ScrollUp"</tt> and <tt>"ScrollDown"</tt>.
          *  @param ea The event generated by Producer.
          */
         void handleScrollEvent (const osgGA::GUIEventAdapter& ea);

         /// The viewer viewing the nodes.
         osgProducer::Viewer& viewer_;

         /** If this is \c true, faces back-facing the viewer will be ignored
          *  while picking.
          */
         bool ignoreBackFaces_;

         /** The values to be returned by the \c handle() method, depending on
          *  the event type it is handling. Values are not initialized, meaning
          *  that \c handle(), by default, returns \c false for all event types.
          *  @todo Implement a method like \c setHandleReturnValue(), so that
          *        the user can configure this.
          */
         std::map <osgGA::GUIEventAdapter::EventType, bool> handleReturnValues_;

         /// Type mapping a signal name to the signal object.
         typedef std::map <std::string, SignalPtr> SignalCollection_t;

         /// Type mapping nodes to the collection of signals associated to it.
         typedef std::map <NodePtr, SignalCollection_t > SignalsMap_t;

         /** Structure containing all the signals used by this \c EventHandler.
          *  It can be used with a syntax like this:
          *  <tt>signals_[node]["MouseMove"]->connect(&MyHandler);</tt>
          */
         SignalsMap_t signals_;

         /** The \c osgUtil::Hit structure for the node currently under the
          *  mouse pointer. (Respecting the \c ignoreBackFaces_ flag.)
          */
         osgUtil::Hit hitUnderMouse_;

         //
         // For "MouseEnter", "MouseLeave", "MouseMove"
         //

         /// The node currently under the mouse pointer.
         NodePtr nodeUnderMouse_;

         /** The position (in the object coordinate system) of
          *  \c nodeUnderMouse_ currently under the mouse pointer.
          */
         osg::Vec3 positionUnderMouse_;

         //
         // For "Click" and "DoubleClick"
         //

         /// The buttons of a mouse.
         enum MouseButton
         {
            LEFT_MOUSE_BUTTON,   ///< The left button.
            MIDDLE_MOUSE_BUTTON, ///< The middle button.
            RIGHT_MOUSE_BUTTON,  ///< The right button.
            MOUSE_BUTTON_COUNT   ///< The number of buttons in a mouse.
         };

         /** Returns the mouse button used in the \c osgGA::GUIEventAdapter
          *  passed as parameter.
          */
         MouseButton getMouseButton (const osgGA::GUIEventAdapter& ea);

         /** An array indicating (for every mouse button) which was the node
          *  that received the last mouse down event. This is used to identify
          *  clicks.
          */
         NodePtr nodeThatGotMouseDown_[MOUSE_BUTTON_COUNT];

         /** An array indicating (for every mouse button) which was the node
          *  that received the last click event. This is used to identify double
          *  clicks.
          */
         NodePtr nodeThatGotClick_[MOUSE_BUTTON_COUNT];

         /** An array indicating (for every mouse button) the time at which the
          *  last click event has happened. This is used to identify double
          *  clicks.
          */
         double timeOfLastClick_[MOUSE_BUTTON_COUNT];

         //
         // For "KeyUp" and "KeyDown"
         //

         /// The node receiving keyboard events.
         NodePtr kbdFocus_;

         /// The focus policy for keyboard-related events.
         FocusPolicyPtr kbdFocusPolicy_;

         //
         // For "MouseWheelUp" and "MouseWheelDown"
         //

         /// The node receiving mouse wheel events.
         NodePtr wheelFocus_;

         /// The focus policy for mouse wheel-related events.
         FocusPolicyPtr wheelFocusPolicy_;
   };

} // namespace OSGUIsh


#endif // _GUISH_EVENT_HANDLER_HPP_
