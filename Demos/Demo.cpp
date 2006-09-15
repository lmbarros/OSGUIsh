/******************************************************************************\
* Demo.cpp                                                                     *
* OSGUIsh demo.                                                                *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <osgDB/ReadFile>
#include <osgProducer/Viewer>
#include <OSGUIsh/EventHandler.hpp>


class AnEventHandler
{
   public:
      void operator()(osgGA::GUIEventAdapter* ea, std::string nodeName)
      {
         std::cout << "Node '" << nodeName << "' just got an event!\n";
      }
};

void HandleAnEvent (osgGA::GUIEventAdapter* ea, std::string nodeName)
{
   std::cout << "Node '" << nodeName << "' just got an event!\n";
}



// - main ----------------------------------------------------------------------
int main (int argc, char* argv[])
{
   // Create a Producer-based viewer
   osgProducer::Viewer viewer;
   viewer.setUpViewer(osgProducer::Viewer::STANDARD_SETTINGS);

   osg::ref_ptr<OSGUIsh::EventHandler> guishEH(
      new OSGUIsh::EventHandler (viewer));

   viewer.getEventHandlerList().push_front (guishEH.get());

   // Load the model
   osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile ("Data/Tree_01.3ds");

   if (!loadedModel)
   {
      std::cerr << "Problem opening 'Data/Tree_01.3ds'\n";
      exit (1);
   }

   viewer.setSceneData (loadedModel.get());

   // Register event handlers
   guishEH->addNode (loadedModel, "Tree");
   guishEH->getSignal ("Tree", "MouseDown").connect (&HandleAnEvent);


   // Enter rendering loop
   viewer.realize();

   while (!viewer.done())
   {
      viewer.sync();
      viewer.update();
      viewer.frame();
   }

   viewer.sync();
}
