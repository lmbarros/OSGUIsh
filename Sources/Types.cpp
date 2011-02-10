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


#include "OSGUIsh/Types.hpp"

namespace OSGUIsh
{
   // - Intersection_t::Intersection_t -----------------------------------------
   Intersection_t::Intersection_t(
      const osgUtil::LineSegmentIntersector::Intersection& hit)
      : nodePath(hit.nodePath),
        worldIntersectionPoint(hit.getWorldIntersectPoint()),
        worldIntersectionNormal(hit.getWorldIntersectNormal()),
        localIntersectionPoint(hit.getLocalIntersectPoint()),
        localIntersectionNormal(hit.getLocalIntersectNormal())
   { }


   Intersection_t::Intersection_t(
      const osgUtil::PolytopeIntersector::Intersection& hit)
      : nodePath(hit.nodePath),
        worldIntersectionPoint(hit.matrix.valid()
                               ? hit.localIntersectionPoint * (*hit.matrix)
                               : hit.localIntersectionPoint),
        worldIntersectionNormal(osg::Vec3(0.0, 0.0, 0.0)),
        localIntersectionPoint(hit.localIntersectionPoint),
        localIntersectionNormal(osg::Vec3(0.0, 0.0, 0.0))
   { }


} // namespace OSGUIsh
