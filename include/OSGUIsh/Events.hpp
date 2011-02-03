/******************************************************************************\
* Events.hpp                                                                   *
* Types events supported by OSGUIsh.                                           *
*                                                                              *
* Copyright (C) 2011 by Leandro Motta Barros.                                  *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _OSGUISH_EVENTS_HPP_
#define _OSGUISH_EVENTS_HPP_

#include <osg/Node>
#include <osgUtil/LineSegmentIntersector>


namespace OSGUIsh
{
   /// The events supported by OSGUIsh.
   enum Event
   {
      /**
       * A "mouse move" event; generated when the mouse pointer moves on the
       * surface of a registered node. Movement is relative: if the mouse
       * pointer is standing still, but the object under it is moving, mouse
       * move events will be generated.
       */
      EVENT_MOUSE_MOVE,

      /**
       * A "mouse enter" event; generated when the mouse pointer goes over a
       * registered node.
       */
      EVENT_MOUSE_ENTER,

      /**
       * A "mouse leave" event; generated when the mouse pointer goes out of a
       * registered node.
       */
      EVENT_MOUSE_LEAVE,

      /**
       * A "mouse down" event; generated when a mouse button is pressed when the
       * mouse cursor is over a registered node.
       */
      EVENT_MOUSE_DOWN,

      /**
       * A "mouse up" event; generated when a mouse button is released when the
       * mouse cursor is over a registered node.
       */
      EVENT_MOUSE_UP,

      /**
       * A "click" event; generated when a mouse button is pressed and then
       * released when the mouse cursor is over a registered node.
       */
      EVENT_CLICK,

      /**
       * A "double click" event; generated when two \c EVENT_CLICK events are
       * generated in rapid succession on the same registered node.
       */
      EVENT_DOUBLE_CLICK,

      /**
       * A "key down" event; generated when a keyboard key is pressed while a
       * registered node has the focus. Different <tt>FocusPolicy</tt>s can be
       * used to determine which node has the focus.
       */
      EVENT_KEY_DOWN,

      /**
       * A "key up" event; generated when a keyboard key is released while a
       * registered node has the focus. Different <tt>FocusPolicy</tt>s can be
       * used to determine which node has the focus.
       */
      EVENT_KEY_UP,

      /**
       * A "mouse wheel up" event; generated when the mouse wheel is rolled up
       * (forward) while a registered node has the focus. Different
       * <tt>FocusPolicy</tt>s can be used to determine which node has the
       * focus.
       */
      EVENT_MOUSE_WHEEL_UP,

      /**
       * A "mouse wheel down" event; generated when the mouse wheel is rolled
       * down (backward) while a registered node has the focus. Different
       * <tt>FocusPolicy</tt>s can be used to determine which node has the
       * focus.
       */
      EVENT_MOUSE_WHEEL_DOWN,
   };

} // namespace OSGUIsh

#endif // _OSGUISH_EVENTS_HPP_
