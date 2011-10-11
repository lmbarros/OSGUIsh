/******************************************************************************\
* Types.hpp                                                                    *
* Types used in OSGUIsh.                                                       *
*                                                                              *
* Copyright (C) 2006-2011 by Leandro Motta Barros.                             *
*                                                                              *
* This program is distributed under the OpenSceneGraph Public License. You     *
* should have received a copy of it with the source distribution, in a file    *
* named 'COPYING.txt'.                                                         *
\******************************************************************************/

#ifndef _OSGUISH_TYPES_HPP_
#define _OSGUISH_TYPES_HPP_

#include <osg/Node>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/PolytopeIntersector>


namespace OSGUIsh
{
   /// A (smart) pointer to a scene graph node.
   typedef osg::ref_ptr<osg::Node> NodePtr;

   /**
    * An intersection (a hit when picking). OSG has types similar to this one,
    * but, unfortunately, they are directly coupled with the different
    * intersector types (thus, OSG has \c
    * osgUtil::LineSegmentIntersector::Intersection and \c
    * osgUtil::PolytopeIntersector::Intersection, for instance). This one is
    * home-made "generic" version, providing the information likely to be useful
    * to OSGUIsh users.
    */
   struct Intersection_t
   {
      public:
         /**
          * Default constructor. Does not initialize the members with anything
          * special.
          */
         Intersection_t() { };

         /**
          * Constructs the \c Intersection_t from an \c
          * osgUtil::LineSegmentIntersector::Intersection.
          */
         Intersection_t(
            const osgUtil::LineSegmentIntersector::Intersection& hit);

         /**
          * Constructs the \c Intersection_t from an \c
          * osgUtil::PolytopeIntersector::Intersection.
          */
         Intersection_t(
            const osgUtil::PolytopeIntersector::Intersection& hit);

         /**
          * The node path of the intersected low-level node. By "low-level
          * node", I mean "the node of the scene graph that actually contains
          * the geometry". In particular, this is \e not necessarily the node
          * registered with OSGUIsh; the node registered with OSGUIsh may
          * contain subnodes, and one of these subnodes would be the one at \c
          * nodePath.back().
          */
         osg::NodePath nodePath;

         /// The intersection point, in the world coordinate system.
         osg::Vec3d worldIntersectionPoint;

         /**
          * The object normal vector at the intersection point, in the world
          * coordinate system.
          */
         osg::Vec3d worldIntersectionNormal;

         /// The intersection point, in the local (object) coordinate system.
         osg::Vec3d localIntersectionPoint;

         /**
          * The object normal vector at the intersection point, in the local
          * (object) coordinate system.
          */
         osg::Vec3d localIntersectionNormal;
   };

} // namespace OSGUIsh

#endif // _OSGUISH_TYPES_HPP_
