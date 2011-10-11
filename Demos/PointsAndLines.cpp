/******************************************************************************\
* PointsAndLines.cpp                                                           *
* Using the "picker radius" to pick points and lines.                          *
* Leandro Motta Barros                                                         *
\******************************************************************************/

#include <boost/lexical_cast.hpp>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <OSGUIsh/EventHandler.hpp>

//
// Some globals (globals are not a problem in simple examples ;-))
//

osg::ref_ptr<osg::Geode> LinesPointsNode;
osg::ref_ptr<osgText::Text> TextMouseOver;


//
// The event handlers
//

void HandleMouseEnterOrMove(OSGUIsh::HandlerParams& params)
{
   osg::Vec3 wip = params.hit.worldIntersectionPoint;

   TextMouseOver->setText("Mouse over point( "
                          + boost::lexical_cast<std::string>(wip.x())
                          + ", " + boost::lexical_cast<std::string>(wip.y())
                          + ", " + boost::lexical_cast<std::string>(wip.z())
                          + ").");
}

void HandleMouseLeave(OSGUIsh::HandlerParams& params)
{
   TextMouseOver->setText("Mouse over nothing.");
}


// - CreateHUD -----------------------------------------------------------------
osg::ref_ptr<osg::Projection> CreateHUD(int width, int height)
{
   // Create the text nodes to be displayed on the HUD
   osg::ref_ptr<osg::Geode> hudGeometry (new osg::Geode());

   TextMouseOver = new osgText::Text();
   TextMouseOver->setDataVariance(osg::Object::DYNAMIC);
   TextMouseOver->setText("Nothing interesting under the mouse.");
   TextMouseOver->setFont("Data/bluehigl.ttf");
   TextMouseOver->setPosition(osg::Vec3 (10.0f, 10.0f, 0.0f));
   TextMouseOver->setCharacterSize(25.0);
   hudGeometry->addDrawable(TextMouseOver);

   // Create the HUD per se
   osg::ref_ptr<osg::StateSet> stateSet = hudGeometry->getOrCreateStateSet();
   stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
   stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
   stateSet->setRenderBinDetails(11, "RenderBin");

   osg::ref_ptr<osg::MatrixTransform> modelviewAbs(new osg::MatrixTransform);
   modelviewAbs->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
   modelviewAbs->setMatrix(osg::Matrix::identity());

   modelviewAbs->addChild(hudGeometry);

   osg::ref_ptr<osg::Projection> projection(new osg::Projection());
   projection->setMatrix(osg::Matrix::ortho2D(0, width, 0, height));
   projection->addChild(modelviewAbs);

   return projection;
}



// - CreatePointsAndLines ------------------------------------------------------
void CreatePointsAndLines()
{
   osg::ref_ptr<osg::Geometry> geometry(new osg::Geometry());

   osg::ref_ptr<osg::Vec3Array> positions(new osg::Vec3Array());
   positions->push_back(osg::Vec3(-1.0, 0.0, -1.0));
   positions->push_back(osg::Vec3(-1.0, 0.0, 0.0));
   positions->push_back(osg::Vec3(0.0, 0.0, 0.0));
   positions->push_back(osg::Vec3(0.0, 0.0, 2.0));

   positions->push_back(osg::Vec3(2.0, 0.0, 2.0));
   positions->push_back(osg::Vec3(3.0, 0.0, 1.5));

   osg::ref_ptr<osg::Vec4ubArray> colors(new osg::Vec4ubArray());
   colors->push_back(osg::Vec4ub(255, 255, 255, 255));

   geometry->setVertexArray(positions);
   geometry->setColorArray(colors);
   geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

   geometry->addPrimitiveSet(new osg::DrawArrays(
                                osg::PrimitiveSet::LINE_STRIP, 0, 4));

   geometry->addPrimitiveSet(new osg::DrawArrays(
                                osg::PrimitiveSet::POINTS, 4, 2));

   LinesPointsNode = osg::ref_ptr<osg::Geode>(new osg::Geode());
   LinesPointsNode->addDrawable(geometry);

   LinesPointsNode->getOrCreateStateSet()->setMode(
      GL_LIGHTING, osg::StateAttribute::OFF);
}



// - main ----------------------------------------------------------------------
int main(int argc, char* argv[])
{
   // Create a viewer
   osgViewer::Viewer viewer;
   viewer.setUpViewInWindow(0, 0, 1024, 768);

   // Construct the scene graph, set it as the data to be viewed
   CreatePointsAndLines();
   osg::ref_ptr<osg::Group> sgRoot(new osg::Group());
   sgRoot->addChild(LinesPointsNode);
   sgRoot->addChild(CreateHUD(1024, 768));
   viewer.setSceneData(sgRoot);

   // Create the OSGUIsh event handler
   osg::ref_ptr<OSGUIsh::EventHandler> guishEH(new OSGUIsh::EventHandler(0.01));

   viewer.addEventHandler(guishEH);

   // Adds the node to the event handler, so that it can get events
   guishEH->addNode(LinesPointsNode);

   // Register event handlers. Notice that "mouse move" events are not generated
   // for points (which makes sense!), so we need to handle "mouse enter"
   // events, too.
   guishEH->getSignal(LinesPointsNode, OSGUIsh::EVENT_MOUSE_ENTER)
      ->connect(&HandleMouseEnterOrMove);
   guishEH->getSignal(LinesPointsNode, OSGUIsh::EVENT_MOUSE_MOVE)
      ->connect(&HandleMouseEnterOrMove);
   guishEH->getSignal(LinesPointsNode, OSGUIsh::EVENT_MOUSE_LEAVE)
      ->connect(&HandleMouseLeave);

   // Enter rendering loop
   viewer.run();
}
