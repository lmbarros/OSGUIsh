/******************************************************************************\
* Simplest.cpp                                                                 *
* The simplest OSGUIsh demo.                                                   *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <OSGUIsh/EventHandler.hpp>

//
// The event handlers
//

void HandleMouseMove(OSGUIsh::HandlerParams& params)
{
   std::cout << "Mouse moved on node '" << params.node << "'!\n";
}

void HandleMouseEnter(OSGUIsh::HandlerParams& params)
{
   std::cout << "Entered node '" << params.node << "'!\n";
}

void HandleMouseLeave(OSGUIsh::HandlerParams& params)
{
   std::cout << "Left node '" << params.node << "'!\n";
}

void HandleMouseDown(OSGUIsh::HandlerParams& params)
{
   std::cout << "Mouse down on node '" << params.node << "'!\n";
}

void HandleMouseUp(OSGUIsh::HandlerParams& params)
{
   std::cout << "Mouse up on node '" << params.node << "'!\n";
}

void HandleClick(OSGUIsh::HandlerParams& params)
{
   std::cout << "Click on node '" << params.node << "'!\n";
}

void HandleDoubleClick(OSGUIsh::HandlerParams& params)
{
   std::cout << "Double click on node '" << params.node << "'!\n";
}

void HandleKeyDown(OSGUIsh::HandlerParams& params)
{
   std::cout << "Key down on node '" << params.node << "'!\n";
}

void HandleKeyUp(OSGUIsh::HandlerParams& params)
{
   std::cout << "Key up on node '" << params.node << "'!\n";
}

void HandleMouseWheelUp(OSGUIsh::HandlerParams& params)
{
   std::cout << "Mouse wheel up on node '" << params.node << "'!\n";
}

void HandleMouseWheelDown(OSGUIsh::HandlerParams& params)
{
   std::cout << "Mouse wheel down on node '" << params.node << "'!\n";
}



// - main ----------------------------------------------------------------------
int main(int argc, char* argv[])
{
   // Create a viewer
   osgViewer::Viewer viewer;
   viewer.setUpViewInWindow(0, 0, 1024, 768);

   // Load the model, set it as the scene data
   osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("Data/Tree_01.3ds");

   if (!loadedModel)
   {
      std::cerr << "Problem opening 'Data/Tree_01.3ds'\n";
      exit(1);
   }

   viewer.setSceneData(loadedModel);

   // Create the OSGUIsh event handler
   osg::ref_ptr<OSGUIsh::EventHandler> guishEH(new OSGUIsh::EventHandler());
   viewer.addEventHandler(guishEH);

   // Adds the node to the event handler, so that it can get events
   guishEH->addNode(loadedModel);

   // Register event handlers
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_MOVE)
      ->connect(&HandleMouseMove);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_ENTER)
      ->connect(&HandleMouseEnter);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_LEAVE)
      ->connect(&HandleMouseLeave);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_DOWN)
      ->connect(&HandleMouseDown);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_UP)
      ->connect(&HandleMouseUp);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_CLICK)
      ->connect(&HandleClick);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_DOUBLE_CLICK)
      ->connect(&HandleDoubleClick);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_KEY_DOWN)
      ->connect(&HandleKeyDown);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_KEY_UP)
      ->connect(&HandleKeyUp);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_WHEEL_UP)
      ->connect(&HandleMouseWheelUp);
   guishEH->getSignal(loadedModel, OSGUIsh::EVENT_MOUSE_WHEEL_DOWN)
      ->connect(&HandleMouseWheelDown);

   // Set the receivers of keyboard and mouse wheel events
   guishEH->setKeyboardFocus(loadedModel);
   guishEH->setMouseWheelFocus(loadedModel);

   // Enter rendering loop
   viewer.run();
}
