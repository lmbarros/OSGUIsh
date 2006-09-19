/******************************************************************************\
* Types.hpp                                                                    *
* Types used in OSGUIsh.                                                       *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _OSGUISH_TYPES_HPP_
#define _OSGUISH_TYPES_HPP_

#include <osg/Node>


namespace OSGUIsh
{
   /// A (smart) pointer to a scene graph node.
   typedef osg::ref_ptr<osg::Node> NodePtr;

} // namespace OSGUIsh

#endif // _OSGUISH_TYPES_HPP_
