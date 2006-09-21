/******************************************************************************\
* ThreeObjects.cpp                                                             *
* Two objects sharing some event handlers, one that doesn't receive events.    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgProducer/Viewer>
#include <OSGUIsh/EventHandler.hpp>

//
// Some globals (globals are not a problem in simple examples ;-))
//

OSGUIsh::NodePtr NodeUnderMouse;
osg::ref_ptr<osg::Node> TreeNode;;
osg::ref_ptr<osg::Node> StrawberryNode;
osg::ref_ptr<osg::Node> FishNode;


//
// The event handlers
//

void HandleMouseEnter (const osgGA::GUIEventAdapter& ea, OSGUIsh::NodePtr node)
{
   std::cout << "Entered node '" << node->getName() << "'!\n";
}

void HandleMouseLeave (const osgGA::GUIEventAdapter& ea, OSGUIsh::NodePtr node)
{
   std::cout << "Left node '" << node->getName() << "'!\n";
}

void HandleDoubleClickTree (const osgGA::GUIEventAdapter& ea, OSGUIsh::NodePtr node)
{
   std::cout << "Double click on Tree!\n";
}

void HandleDoubleClickStrawberry (const osgGA::GUIEventAdapter& ea, OSGUIsh::NodePtr node)
{
   std::cout << "Double click on Strawberry!\n";
}



// - LoadModel -----------------------------------------------------------------
osg::ref_ptr<osg::Node> LoadModel (const std::string& fileName)
{
   osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile (fileName);

   if (!loadedModel)
   {
      std::cerr << "Problem opening '" << fileName << "'\n";
      exit (1);
   }

   return loadedModel;
}



// - LoadModels ----------------------------------------------------------------
osg::ref_ptr<osg::Group> LoadModels()
{
   osg::ref_ptr<osg::Group> group (new osg::Group);

   TreeNode = LoadModel ("Data/Tree_01.3ds");
   TreeNode->setName ("Tree");
   osg::ref_ptr<osg::PositionAttitudeTransform> treePAT(
      new osg::PositionAttitudeTransform());
   treePAT->addChild (TreeNode.get());
   treePAT->setPosition (osg::Vec3 (1.2, 0.0, 0.0));
   group->addChild (treePAT.get());

   StrawberryNode = LoadModel ("Data/Strawberry.3ds");
   StrawberryNode->setName ("Strawberry");
   osg::ref_ptr<osg::PositionAttitudeTransform> strawberryPAT(
      new osg::PositionAttitudeTransform());
   strawberryPAT->addChild (StrawberryNode.get());
   strawberryPAT->setPosition (osg::Vec3 (-1.0, 0.0, 0.0));
   group->addChild (strawberryPAT.get());

   FishNode = LoadModel ("Data/Fish.3ds");
   FishNode->setName ("Fish");

   group->addChild (FishNode.get());

   return group;
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

   // Load the models, set them as the data to be viewed
   viewer.setSceneData (LoadModels().get());

   // Adds the node to the event handler, so that it can get events
   guishEH->addNode (TreeNode);
   guishEH->addNode (StrawberryNode);

   // Register event handlers
   guishEH->getSignal (TreeNode, "MouseEnter")->connect (&HandleMouseEnter);
   guishEH->getSignal (StrawberryNode, "MouseEnter")->connect (&HandleMouseEnter);
   guishEH->getSignal (TreeNode, "MouseLeave")->connect (&HandleMouseLeave);
   guishEH->getSignal (StrawberryNode, "MouseLeave")->connect (&HandleMouseLeave);
   guishEH->getSignal (StrawberryNode, "DoubleClick")->connect (&HandleDoubleClickStrawberry);
   guishEH->getSignal (TreeNode, "DoubleClick")->connect (&HandleDoubleClickTree);

   // An OpenGL window, please
   viewer.realize();

   // Enter rendering loop
   while (!viewer.done())
   {
      viewer.sync();
      viewer.update();
      viewer.frame();
   }

   viewer.sync();
}
