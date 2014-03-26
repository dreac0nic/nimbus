#include "TestMode.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreFont.h>
#include <OgreFontManager.h>


using namespace Nimbus;
using namespace Ogre;

TestMode::TestMode(void)
{
}

TestMode::~TestMode(void)
{
}

RunMode* TestMode::run(const FrameEvent& evt)
{
// Attempt to initialize the run mode
if(!this->initialized && !this->initialize())
{
LogManager::getSingletonPtr()->logMessage("(Nimbus) Failed to initialize TestMode");

// Terminate the application if we fail to initialize
return 0;
}

// Continue to run this runmode
return this;
}

bool TestMode::initialize()
{
Entity* dragon;
SceneNode* dragonNode;

Light* light;

// Create the scene manager
mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");

// Overlay stuffz
Ogre::OverlaySystem* overlaySystem = new Ogre::OverlaySystem;
mSceneMgr->addRenderQueueListener(overlaySystem);

Ogre::OverlayManager* mOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

Ogre::Overlay* theOverlay = mOverlayMgr->create("theOverlay");

theOverlay->setZOrder(500);

// Font
Ogre::FontPtr mFont = Ogre::FontManager::getSingleton().create("Font", "General");
mFont->setType(Ogre::FT_TRUETYPE);
mFont->setSource("../../times.ttf");
mFont->setTrueTypeSize(32);
mFont->setTrueTypeResolution(96);
mFont->addCodePointRange(Ogre::Font::CodePointRange(33, 255));


// Overlay element
Ogre::OverlayContainer* mPanel = static_cast<Ogre::OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "PANEL"));
mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
mPanel->setPosition(0.25, 0.25);						// This sets the position of the menu on the screen
mPanel->setDimensions(0.5, 0.5);						// This sets the width and height of the menu on the screen
mPanel->setMaterialName( "BaseWhite" );					// This makes the menu color white

// Overlay element text
Ogre::TextAreaOverlayElement* mTextArea = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "TEXT"));
mTextArea->setMetricsMode(Ogre::GMM_PIXELS);
mTextArea->setFontName("Font");
mTextArea->setPosition(0.25, 0.25);
mTextArea->setDimensions(0.5, 0.5);
mTextArea->setCaption("Grignr began to reel and stagger under the blob, his chalk \n\
white face and faltering muscles attesting to the gigantic loss \n\
of blood.  Carthena slipped from Grignr in a death-like faint, a \n\
morrow chilling scream upon her red rubish lips.  In final \n\
desperation Grignr grasped the smoldering torch upon the ground \n\
and plunged it into the reeking maw of the travestry.  A shudder \n\
passed through the thing.  Grignr felt the blackness closing upon \n\
his eyes, but held on with the last ebb of his rapidly waning \n\
vitality.  He could feel its grip lessoning as a hideous gurgling \n\
sound erupted from the writhing maw.  The jelly like mass began \n\
to bubble like a vat of boiling tar as quavers passed up and down \n\
its entire form.");
mTextArea->setCharHeight(16);
mTextArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
mTextArea->setColourTop(ColourValue(0.5, 0.7, 0.5));
//mTextArea->setColour(Ogre::ColourValue(0.1, 0.1, 0.1));
printf("%s ",mTextArea->getName());



theOverlay->add2D(mPanel);
mPanel->addChild(mTextArea);

//mTextArea->show();
//mPanel->show();
theOverlay->show();


printf("Overlay Enabled?? = %d", theOverlay->isInitialised()); 
// Create the camera
mCamera = mSceneMgr->createCamera("PlayerCam");


//////////
// Set up the camera

// Position the camera
mCamera->setPosition(Vector3(0, 50, 80));
mCamera->lookAt(Vector3(0, 0, -100));
mCamera->setNearClipDistance(5);

// Add a viewport for the camera
mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);

//////////
// Set up the appropriate models

// Correct the aspect ratio of the camera
mCamera->setAspectRatio(
Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

// Load the dragon
dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
dragonNode->attachObject(dragon);

dragonNode->setPosition(0, 0, -100);
dragonNode->setScale(40.0, 40.0, 40.0);
dragonNode->pitch(Degree(90));

//////////
// Set up light sources

// Set the ambient light
mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

// Create a point light for main light
light = mSceneMgr->createLight("MainLight");
light->setPosition(20, 80, 50);

// Note that the RunMode has been initialized
this->initialized = true;
return true;
}
