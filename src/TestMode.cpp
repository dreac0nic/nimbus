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
	this->keyListener = new KeyListener();

	EventSystem::getSingleton()->registerListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
}

TestMode::~TestMode(void)
{
	EventSystem::getSingleton()->unregisterListener(this->keyListener, EventSystem::EventType::KEY_PRESS);

	delete this->keyListener;
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

/*
// Overlay stuffz
OverlaySystem* overlaySystem = new OverlaySystem;
mSceneMgr->addRenderQueueListener(overlaySystem);

OverlayManager* mOverlayMgr = OverlayManager::getSingletonPtr();

Overlay* theOverlay = mOverlayMgr->create("theOverlay");

theOverlay->setZOrder(500);

// Font
FontPtr mFont = FontManager::getSingleton().create("Font", "General");
mFont->setType(FT_TRUETYPE);
mFont->setSource("../../times.ttf");
mFont->setTrueTypeSize(32);
mFont->setTrueTypeResolution(96);
mFont->addCodePointRange(Ogre::Font::CodePointRange(33, 255));


// Overlay element
OverlayContainer* mPanel = static_cast<OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "PANEL"));
mPanel->setMetricsMode(GMM_RELATIVE);
mPanel->setPosition(0.35, 0.25);							// This sets the position of the menu on the screen
mPanel->setDimensions(0.3, 0.5);						// This sets the width and height of the menu on the screen
mPanel->setMaterialName( "BaseWhite" );					// This makes the menu color white

// Overlay buttons
OverlayContainer* resumeButton = static_cast<OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "RESUMEBUTTON"));
resumeButton->setMetricsMode(GMM_RELATIVE);
resumeButton->setPosition(0.47, 0.3);
resumeButton->setDimensions(0.0, 0.0);
//resumeButton->setMaterialName("BaseWhite");

OverlayContainer* quitButton = static_cast<OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "QUITBUTTON"));
quitButton->setMetricsMode(GMM_RELATIVE);
quitButton->setPosition(0.48, 0.5);
quitButton->setDimensions(0.0, 0.0);
//quitButton->setMaterialName("BaseWhite");


// Overlay element text
TextAreaOverlayElement* resume = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "RESUME"));
resume->setMetricsMode(GMM_PIXELS);
resume->setFontName("Font");
resume->setPosition(0.0, 0.0);
resume->setDimensions(0.1, 0.1);
resume->setCaption("Resume");
resume->setCharHeight(16);
//resume->setColourBottom(ColourValue(1.0, 0.0, 0.0));
//resume->setColourTop(ColourValue(0.0, 1.0, 0.0));
resume->setColour(ColourValue(0.0, 1.0, 0.0));
printf("resume name = %s\n",resume->getName());


TextAreaOverlayElement* quit = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "QUIT"));
quit->setMetricsMode(GMM_PIXELS);
quit->setFontName("Font");
quit->setPosition(0.0, 0.0);
quit->setDimensions(0.1, 0.1);
quit->setCaption("Quit");
quit->setCharHeight(16);
//quit->setColourBottom(ColourValue(1.0, 0.0, 0.0));
//quit->setColourTop(ColourValue(0.0, 1.0, 0.0));
quit->setColour(ColourValue(1.0, 0.0, 0.0));
printf("quit name = %s\n",quit->getName());

theOverlay->add2D(mPanel);
theOverlay->add2D(resumeButton);
theOverlay->add2D(quitButton);
resumeButton->addChild(resume);
quitButton->addChild(quit);

theOverlay->show();

// testing
printf("Overlay Enabled = %d\n", theOverlay->isInitialised());
printf("mPanel Enabled = %d\n", mPanel->isEnabled());
printf("resumeButton Enabled = %d\n", resumeButton->isEnabled());
printf("resume Enabled = %d\n", resume->isEnabled());
printf("quit Enabled = %d\n", quit->isEnabled());
printf("resume Visible = %d\n", resume->isVisible());
printf("quit Visible = %d\n", quit->isVisible());
*/

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

void TestMode::KeyListener::handleEvent(payloadmap payload)
{
	OIS::KeyCode keyCode = *static_cast<OIS::KeyCode*>(payload["KeyCode"]);
	bool keyReleased = *static_cast<bool*>(payload["KeyReleased"]);

	// Do stuff!
}
