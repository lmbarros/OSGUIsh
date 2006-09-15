/******************************************************************************\
* GUIshEventHandler.hpp                                                        *
* An event handler providing GUI-like events                                   *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#ifndef _GUISH_EVENT_HANDLER_HPP_
#define _GUISH_EVENT_HANDLER_HPP_


#include <osgProducer/Viewer>


/** An event handler providing GUI-like events for nodes.
 *  @todo Make the return value of \c handle() configurable "per event type".
 *        For example: please return \c false when a \c PUSH event happens (even
 *        if it is handled).
 */
class GUIshEventHandler: public osgGA::GUIEventHandler
{
   public:
      GUIshEventHandler (const osgProducer::Viewer& viewer);

   private:
      const osgProducer::Viewer& viewer_;
};


#endif // _GUISH_EVENT_HANDLER_HPP_
