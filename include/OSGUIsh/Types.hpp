/******************************************************************************\
* Types.hpp                                                                    *
* Types used in OSGUIsh.                                                       *
*                                                                              *
* Copyright (C) 2006-2010 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _OSGUISH_TYPES_HPP_
#define _OSGUISH_TYPES_HPP_

#include <osg/Node>
#include <osgUtil/LineSegmentIntersector>


namespace OSGUIsh
{
   /// A (smart) pointer to a scene graph node.
   typedef osg::ref_ptr<osg::Node> NodePtr;

   /// The type of an intersection (a hit when picking).
   typedef osgUtil::LineSegmentIntersector::LineSegmentIntersector::Intersection
   Intersection_t;

} // namespace OSGUIsh

#endif // _OSGUISH_TYPES_HPP_
