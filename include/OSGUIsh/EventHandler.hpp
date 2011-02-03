/******************************************************************************\
* EventHandler.hpp                                                             *
* An event handler providing GUI-like events                                   *
*                                                                              *
* Copyright (C) 2006-2011 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _GUISH_EVENT_HANDLER_HPP_
#define _GUISH_EVENT_HANDLER_HPP_

#include <boost/signal.hpp>
#include <osgGA/GUIEventHandler>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/View>
#include <OSGUIsh/Events.hpp>
#include <OSGUIsh/FocusPolicy.hpp>
#include <OSGUIsh/ManualFocusPolicy.hpp>


namespace OSGUIsh
{
   /**
    * A \c struct grouping parameters passed to event handlers. Future versions
    * of OSGUish may add more members here if necessary without breaking
    * existing user code.
    * @note Be aware that, in the general case, <tt>hit.nodePath.back() !=
    *       node</tt>. \c hit contains the actual, "low level" hit, while \c
    *       node contains the node registered with OSGUish. For instance,
    *       suppose you register a car node, that has a car body node and four
    *       wheel nodes as subnodes. \c node will always be the whole car, while
    *       \c hit will be the "car body" or one of the "wheels".
    */
   struct HandlerParams
   {
      public:
         /// Convenience constructor.
         HandlerParams(NodePtr nodeParam,
                       const osgGA::GUIEventAdapter& eventParam,
                       const Intersection_t& hitParam)
            : node(nodeParam), event(eventParam), hit(hitParam)
         { }

         /// The node generating the event.
         NodePtr node;

         /**
          * The event data, as passed by OSG. Here you can find many useful
          * information, like "which mouse button was pressed".
          */
         const osgGA::GUIEventAdapter& event;

         /**
          * The \c Intersection_t for the node that was under the mouse pointer
          * when the event was generated.
          * @note Notice that, in some cases, the \c Intersection_t for the node
          *       under the mouse pointer doesn't really have something to do
          *       with the event being handled. For example, if the event is a
          *       \c KeyUp and the focus policy is not the "node under mouse has
          *       focus" policy, than the hit may not be related to the node
          *       generating the event.
          *       <p>Also, in cases like that, the hit may even be invalid,
          *       since, perhaps, there will not be any registered node under
          *       the mouse pointer. Figuring out if \c hit contains valid and
          *       meaningful information is up to the user.
          */
         const Intersection_t& hit;
   };



   /**
    * An event handler providing GUI-like events for nodes. The \c EventHandler
    * has an internal list of nodes being "observed". Every observed node has a
    * collection of signals associated to it. These signals represent the events
    * that can be generated for the node.
    */
   class EventHandler: public osgGA::GUIEventHandler
   {
      public:
         /**
          * Constructs an \c EventHandler.
          * @param kbdPolicyFactory The factory that will be used to create the
          *        \c FocusPolicy used to automatically set the focus for
          *        keyboard events.
          * @param wheelPolicyFactory The factory that will be used to create
          *        the \c FocusPolicy used to automatically set the focus for
          *        mouse wheel events.
          */
         EventHandler(const FocusPolicyFactory& kbdPolicyFactory =
                      FocusPolicyFactoryMason<ManualFocusPolicy>(),
                      const FocusPolicyFactory& wheelPolicyFactory =
                      FocusPolicyFactoryMason<ManualFocusPolicy>());

         /**
          * Handles upcoming events (overloads virtual method).
          * @return See \c handleReturnValues_, please.
          */
         bool handle(const osgGA::GUIEventAdapter& ea,
                     osgGA::GUIActionAdapter&);

         /// A type representing a sequence of node masks.
         typedef std::vector<osg::Node::NodeMask> NodeMasks_t;

         /**
          * Sets node mask that will be used when picking. The default node mask
          * is 0xFFFFFFFF, meaning that every node with at least one bit set on
          * their node masks will be taken into account for picking. (All this
          * just means that you don't change any setting related to node masks,
          * OSGUIsh will not ignore any node when picking.)
          *
          * So, when should one explicitly set the picking mask? The easy answer
          * is "whenever one wants to ignore some nodes when picking". (When
          * picking, OSGUIsh (OSG, in fact) will perform a bitwise AND between
          * the picking root set here and each node's node mask; if the result
          * is zero, the node will be ignored.)
          *
          * The interesting part is that OSGUIsh supports multiple picking
          * masks. First, it will try to pick nodes using the first node mask;
          * if no node is picked, it will try picking using the second picking
          * mask and so on.
          *
          * Creative people may find various uses for this, but the feature was
          * implemented so that picking could work with HUDs. When using
          * multiple <tt>osg::Camera</tt>s (as is the case when using HUDs),
          * picking in OSG can be problematic. The details of this problem are
          * not important here (anyway, basically, we don't have control on the
          * relative "z-order" of objects viewed by different
          * <tt>Camera</tt>s). But it is important to know that, if we want to
          * pick objects in different camera nodes, we may have some
          * unpredictable results.
          *
          * So, to enable proper picking in HUDs, OSGUIsh can use this scheme of
          * multiple picking roots.
          *
          * @see setPickingMasks() which are the methods capable of setting
          *      multiple picking roots.
          *
          * @param newMask The new node mask to be used when picking.
          */
         void setPickingMask(osg::Node::NodeMask newMask);

         /**
          * Sets the node masks used when picking.
          * @param newMask1 The first node mask to be tried when picking.
          * @param newMask2 The second node mask to be tried when picking.
          * @see setPickingMask for a description of why using multiple picking
          *      roots can be useful (tip: HUD).
          */
         void setPickingMasks(osg::Node::NodeMask newMask1,
                              osg::Node::NodeMask newMask2);

         /**
          * Sets the node masks used when picking.
          * @param newMask1 The first node mask to be tried when picking.
          * @param newMask2 The second node mask to be tried when picking.
          * @param newMask3 The third node mask to be tried when picking.
          * @see setPickingMask for a description of why using multiple picking
          *      roots can be useful (tip: HUD).
          */
         void setPickingMasks(osg::Node::NodeMask newMask1,
                              osg::Node::NodeMask newMask2,
                              osg::Node::NodeMask newMask3);

         /**
          * Sets the node masks used when picking.
          * @param newMasks A vector with the node masks to be tried when
          *        picking (they'll be tried in sequence).
          * @see setPickingMask for a description of why using multiple picking
          *      roots can be useful (tip: HUD).
          */
         void setPickingMasks(const NodeMasks_t& newMasks);

         /**
          * A type representing a signal used in OSGUIsh. This signal returns
          * nothing and takes a \c HandlerParams, which packs all relevant data
          * for an event handler function.
          */
         typedef boost::signal<void (HandlerParams&)> Signal_t;

         /// A (smart) pointer to a \c Signal_t;
         typedef boost::shared_ptr<Signal_t> SignalPtr;

         /**
          * Adds a given node to the list of nodes being "observed" by this \c
          * EventHandler. In other words, after this call, signals for this node
          * will be triggered.
          * @param node The node that will be added to this \c EventHandler.
          */
         void addNode(const NodePtr node);

         /**
          * Returns a signal associated with a given node. This is typically
          * used to call \c connect() on the returned signal.
          * @param node The desired node.
          * @param signal The desired signal.
          * @note The "mouse move" event is "relative": if the mouse is not
          *       moving, but a node is moving "below" it, mouse move events
          *       will be generated.
          */

         SignalPtr getSignal(const NodePtr node, Event signal);

         /**
          * Ignores or stops to ignore faces that are back-facing the viewer
          * when picking. It may be useful to ignore back faces when backface
          * culling is enabled.
          * @param ignore If \c true, back faces will be ignored when picking.
          *        If \c false, back faces will be considered when piking.
          * @bug "Ignore back faces" will not work always. More specifically, it
          *      will work only when picking gets at least two hits. It doesn't
          *      matter if these two hits are of front or back facing faces (or
          *      a combination of them). If there is only a single hit, it will
          *      not be ignored, even the face hit was a back facing one.
          */
         void ignoreBackFaces(bool ignore = true)
         { ignoreBackFaces_ = ignore; }

         /**
          * Manually sets the node that will receive keyboard events. Notice
          * that focus policies allow to set this automatically.
          */
         void setKeyboardFocus(const NodePtr node);

         /**
          * Sets the node that will receive mouse wheel events. Notice that
          * focus policies allow to set this automatically.
          */
         void setMouseWheelFocus(const NodePtr node);

         /**
          * Sets the focus policy for keyboard events to a given one.
          * @param policyFactory The factory that will be used to create the
          *        actual policy.
          */
         void setKeyboardFocusPolicy(const FocusPolicyFactory& policyFactory);

         /**
          * Sets the focus policy for mouse wheel events to a given one.
          * @param policyFactory The factory that will be used to create the
          *        actual policy.
          */
         void setMouseWheelFocusPolicy(const FocusPolicyFactory& policyFactory);

      private:
         /**
          * Returns the first node in an \c osg::NodePath that is present in the
          * list of nodes being "observed" by this \c EventHandler. This is
          * necessary in the cases in which the user is picking a node that is
          * child of an added node. That's the case, for instance, of when the
          * user adds a node read from a 3D model file returned by \c
          * osgDB::readNodeFile().
          * @param nodePath The node path leading to the node being queried.
          * @returns The first node in \c nodePath that was added to the list of
          *          nodes being observed.
          */
         NodePtr getObservedNode(const osg::NodePath& nodePath);

         /**
          * Handles a \c FRAME event triggered by OSG. Signals triggered here
          * are <tt>"MouseEnter"</tt>, <tt>"MouseLeave"</tt> and
          * <tt>"MouseMove"</tt>.
          * @param view The view displaying the scene.
          * @param ea The event generated by OSG.
          */
         void handleFrameEvent(osgViewer::View* view,
                               const osgGA::GUIEventAdapter& ea);

         /**
          * Handles a \c PUSH event triggered by OSG. The only signal triggered
          * here is <tt>"MouseDown"</tt>, but this function also does
          * bookkeeping related to other mouse signals.
          * @param ea The event generated by OSG.
          */
         void handlePushEvent(const osgGA::GUIEventAdapter& ea);

         /**
          * Handles a \c RELEASE event triggered by OSG. Signals triggered here
          * are <tt>"MouseUp"</tt>, <tt>"Click"</tt> and <tt>"DoubleClick"</tt>.
          * @param ea The event generated by OSG.
          */
         void handleReleaseEvent(const osgGA::GUIEventAdapter& ea);

         /**
          * Handles a \c KEYDOWN event triggered by OSG. The only signal
          * triggered here is <tt>"KeyDown"</tt>.
          * @param ea The event generated by OSG.
          */
         void handleKeyDownEvent(const osgGA::GUIEventAdapter& ea);

         /**
          * Handles a \c KEYUP event triggered by OSG. The only signal triggered
          * here is <tt>"KeyUp"</tt>.
          * @param ea The event generated by OSG.
          */
         void handleKeyUpEvent(const osgGA::GUIEventAdapter& ea);

         /**
          * Handles a \c SCROLL event triggered by OSG. The signals triggered
          * here are <tt>"ScrollUp"</tt> and <tt>"ScrollDown"</tt>.
          * @param ea The event generated by OSG.
          */
         void handleScrollEvent(const osgGA::GUIEventAdapter& ea);

         /**
          * If this is \c true, faces back-facing the viewer will be ignored
          * while picking.
          */
         bool ignoreBackFaces_;

         /**
          * The sequence of node masks used when picking.
          * @see setPickingRoot for a discussion on how this is used and why
          *      this is necessary.
          */
         NodeMasks_t pickingMasks_;

         /**
          * The values to be returned by the \c handle() method, depending on
          * the event type it is handling. Currently, this is not initialized,
          * meaning that \c handle(), by default, returns \c false for all event
          * types.
          * @todo Implement a method like \c setHandleReturnValue(), so that the
          *        user can configure this.
          */
         std::map <osgGA::GUIEventAdapter::EventType, bool> handleReturnValues_;

         /// Type mapping an event type to the signal object.
         typedef std::map <Event, SignalPtr> SignalCollection_t;

         /// Type mapping nodes to the collection of signals associated to it.
         typedef std::map <NodePtr, SignalCollection_t > SignalsMap_t;

         /**
          * Structure containing all the signals used by this \c EventHandler.
          * It can be used with a syntax like this:
          * <tt>signals_[node]["MouseMove"]->connect(&MyHandler);</tt>
          */
         SignalsMap_t signals_;

         /**
          * The \c Intersection_t structure for the node currently under the
          * mouse pointer. (Respecting the \c ignoreBackFaces_ flag.)
          */
         Intersection_t hitUnderMouse_;

         //
         // For "MouseEnter", "MouseLeave", "MouseMove"
         //

         /// The node currently under the mouse pointer.
         NodePtr nodeUnderMouse_;

         /**
          * The position (in the object coordinate system) of \c nodeUnderMouse_
          * currently under the mouse pointer.
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

         /**
          * Returns the mouse button used in the \c osgGA::GUIEventAdapter
          * passed as parameter.
          */
         MouseButton getMouseButton(const osgGA::GUIEventAdapter& ea);

         /**
          * An array indicating (for every mouse button) which was the node that
          * received the last mouse down event. This is used to identify clicks.
          */
         NodePtr nodeThatGotMouseDown_[MOUSE_BUTTON_COUNT];

         /**
          * An array indicating (for every mouse button) which was the node that
          * received the last click event. This is used to identify double
          * clicks.
          */
         NodePtr nodeThatGotClick_[MOUSE_BUTTON_COUNT];

         /**
          * An array indicating (for every mouse button) the time at which the
          * last click event has happened. This is used to identify double
          * clicks.
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
