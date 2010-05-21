/******************************************************************************\
* HUD.cpp                                                                      *
* Using OSGUIsh with a HUD.                                                    *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <iostream>
#include <osg/Camera>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <OSGUIsh/EventHandler.hpp>

//
// Some globals (globals are not a problem in simple examples ;-))
//

osg::ref_ptr<osg::Node> TreeNode;
osg::ref_ptr<osg::Node> StrawberryNode;
osg::ref_ptr<osg::Node> FishNode;

osg::ref_ptr<osg::Node> HUDTreeNode;
osg::ref_ptr<osg::Node> HUDStrawberryNode;
osg::ref_ptr<osg::Node> HUDFishNode;

osg::ref_ptr<osgText::Text> TextMouseOver;

const osg::Node::NodeMask HUD_MASK = 0x00000001;
const osg::Node::NodeMask SCENE_MASK = 0x00000002;


//
// The event handlers
//

void HandleMouseEnter(OSGUIsh::HandlerParams& params)
{
   TextMouseOver->setText("Mouse over " + params.node->getName());
}

void HandleMouseLeave(OSGUIsh::HandlerParams& params)
{
   TextMouseOver->setText("Mouse over nothing");
}



// - LoadModel -----------------------------------------------------------------
osg::ref_ptr<osg::Node> LoadModel(const std::string& fileName)
{
   osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(fileName);

   if (!loadedModel)
   {
      std::cerr << "Problem opening '" << fileName << "'\n";
      exit(1);
   }

   return loadedModel;
}



// - LoadModels ----------------------------------------------------------------
osg::ref_ptr<osg::Group> LoadModels()
{
   osg::ref_ptr<osg::Group> group(new osg::Group);
   group->setNodeMask(SCENE_MASK);

   TreeNode = LoadModel("Data/Tree_01.3ds");
   TreeNode->setName("Tree (scene)");
   osg::ref_ptr<osg::PositionAttitudeTransform> treePAT(
      new osg::PositionAttitudeTransform());
   treePAT->addChild(TreeNode);
   treePAT->setPosition(osg::Vec3(1.2, 0.0, 0.0));
   group->addChild(treePAT);

   StrawberryNode = LoadModel("Data/Strawberry.3ds");
   StrawberryNode->setName("Strawberry (scene)");
   osg::ref_ptr<osg::PositionAttitudeTransform> strawberryPAT(
      new osg::PositionAttitudeTransform());
   strawberryPAT->addChild(StrawberryNode);
   strawberryPAT->setPosition(osg::Vec3(-1.0, 0.0, 0.0));
   group->addChild(strawberryPAT);

   FishNode = LoadModel("Data/Fish.3ds");
   FishNode->setName("Fish (scene)");

   group->addChild(FishNode);

   return group;
}



// - LoadModelsHUD -------------------------------------------------------------
osg::ref_ptr<osg::Group> LoadModelsHUD()
{
   osg::ref_ptr<osg::Group> group(new osg::Group());

   HUDTreeNode = LoadModel("Data/Tree_01.3ds");
   HUDTreeNode->setName("Tree (HUD)");
   osg::ref_ptr<osg::PositionAttitudeTransform> treePAT(
      new osg::PositionAttitudeTransform());
   treePAT->addChild(HUDTreeNode);
   treePAT->setPosition(osg::Vec3(100.0, 550.0, 0.0));
   treePAT->setScale(osg::Vec3(25.0, 25.0, 25.0));
   treePAT->setAttitude(osg::Quat(osg::DegreesToRadians(270.0),
                                  osg::Vec3(1.0, 0.0, 0.0)));
   group->addChild(treePAT);

   HUDStrawberryNode = LoadModel("Data/Strawberry.3ds");
   HUDStrawberryNode->setName("Strawberry (HUD)");
   osg::ref_ptr<osg::PositionAttitudeTransform> strawberryPAT(
      new osg::PositionAttitudeTransform());
   strawberryPAT->addChild(HUDStrawberryNode);
   strawberryPAT->setPosition(osg::Vec3(300.0, 575.0, 0.0));
   strawberryPAT->setAttitude(osg::Quat(osg::DegreesToRadians(270.0),
                                        osg::Vec3(1.0, 0.0, 0.0)));
   strawberryPAT->setScale(osg::Vec3(150.0, 150.0, 150.0));
   group->addChild(strawberryPAT);

   HUDFishNode = LoadModel("Data/Fish.3ds");
   HUDFishNode->setName("Fish (HUD)");
   osg::ref_ptr<osg::PositionAttitudeTransform> fishPAT(
      new osg::PositionAttitudeTransform());
   fishPAT->addChild(HUDFishNode);
   fishPAT->setPosition(osg::Vec3(500.0, 625.0, 0.0));
   fishPAT->setAttitude(osg::Quat(osg::DegreesToRadians(200.0),
                                  osg::Vec3(0.0, 0.0, 1.0)));
   fishPAT->setScale(osg::Vec3(130.0, 130.0, 130.0));
   group->addChild(fishPAT);

   return group;
}



// - CreateHUD -----------------------------------------------------------------
osg::ref_ptr<osg::Transform> CreateHUD(int width, int height)
{
   // Create the HUD per se
   osg::ref_ptr<osg::Camera> camera(new osg::Camera());
   camera->setNodeMask(HUD_MASK);
   camera->setProjectionMatrixAsOrtho(
      0.0, width, 0.0, height, -1000.0, +1000.0);
   camera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
   camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
   camera->setViewMatrix(osg::Matrix::identity());
   camera->setClearMask(GL_DEPTH_BUFFER_BIT);
   camera->setRenderOrder(osg::Camera::POST_RENDER);
   camera->setAllowEventFocus(false);
   osg::ref_ptr<osg::StateSet> stateSet = camera->getOrCreateStateSet();
   stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
   stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

   // Create the text node to be displayed on the HUD
   osg::ref_ptr<osg::Geode> hudTextGeode(new osg::Geode());

   TextMouseOver = new osgText::Text();
   TextMouseOver->setDataVariance(osg::Object::DYNAMIC);
   TextMouseOver->setText("Mouse over nothing");
   TextMouseOver->setFont("Data/bluehigl.ttf");
   TextMouseOver->setPosition(osg::Vec3(10.0f, 10.0f, 0.0f));
   TextMouseOver->setCharacterSize(25.0);
   hudTextGeode->addDrawable(TextMouseOver);

   camera->addChild(hudTextGeode);

   // Create the image nodes to be displayed on the HUD
   camera->addChild(LoadModelsHUD());

   // Voilà
   return camera;
}



// - main ----------------------------------------------------------------------
int main(int argc, char* argv[])
{
   // Create a viewer
   osgViewer::Viewer viewer;
   viewer.setUpViewInWindow(0, 0, 1024, 768);

   // Construct the scene graph, set it as the data to be viewed
   osg::ref_ptr<osg::Group> sgRoot(new osg::Group());

   sgRoot->addChild(LoadModels());
   osg::ref_ptr<osg::Transform> hudRoot(CreateHUD(1024, 768));
   sgRoot->addChild(hudRoot);

   viewer.setSceneData(sgRoot);

   // Create the OSGUIsh event handler
   osg::ref_ptr<OSGUIsh::EventHandler> guishEH(new OSGUIsh::EventHandler());

   viewer.addEventHandler(guishEH);

   // Add the nodes to the event handler, so that they can get events
   guishEH->addNode(TreeNode);
   guishEH->addNode(StrawberryNode);
   guishEH->addNode(FishNode);

   guishEH->addNode(HUDTreeNode);
   guishEH->addNode(HUDStrawberryNode);
   guishEH->addNode(HUDFishNode);

   // Register event handlers
   guishEH->getSignal(TreeNode, "MouseEnter")->connect(&HandleMouseEnter);
   guishEH->getSignal(StrawberryNode, "MouseEnter")->connect(&HandleMouseEnter);
   guishEH->getSignal(FishNode, "MouseEnter")->connect(&HandleMouseEnter);

   guishEH->getSignal(TreeNode, "MouseLeave")->connect(&HandleMouseLeave);
   guishEH->getSignal(StrawberryNode, "MouseLeave")->connect(&HandleMouseLeave);
   guishEH->getSignal(FishNode, "MouseLeave")->connect(&HandleMouseLeave);

   guishEH->getSignal(HUDTreeNode, "MouseEnter")->connect(&HandleMouseEnter);
   guishEH->getSignal(HUDStrawberryNode, "MouseEnter")->connect(&HandleMouseEnter);
   guishEH->getSignal(HUDFishNode, "MouseEnter")->connect(&HandleMouseEnter);

   guishEH->getSignal(HUDTreeNode, "MouseLeave")->connect(&HandleMouseLeave);
   guishEH->getSignal(HUDStrawberryNode, "MouseLeave")->connect(&HandleMouseLeave);
   guishEH->getSignal(HUDFishNode, "MouseLeave")->connect(&HandleMouseLeave);

   // Pick first the HUD, then the scene.
   guishEH->setPickingMasks(HUD_MASK, SCENE_MASK);

   // Enter rendering loop
   viewer.run();
}
