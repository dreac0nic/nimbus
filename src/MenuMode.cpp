#include "MenuMode.h"
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

MenuMode::MenuMode(void)
{
	this->keyListener = new KeyListener();
	this->mouseListener = new MouseListener(mViewport);

	EventSystem::getSingleton()->registerListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
	EventSystem::getSingleton()->registerListener(this->mouseListener, EventSystem::EventType::MOUSE_CLICKED);
}

MenuMode::~MenuMode(void)
{
	EventSystem::getSingleton()->unregisterListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
	EventSystem::getSingleton()->unregisterListener(this->mouseListener, EventSystem::EventType::MOUSE_CLICKED);

	delete this->keyListener;
	delete this->mouseListener;
}

RunMode* MenuMode::run(const FrameEvent& evt)
{
// Attempt to initialize the run mode
if(!this->initialized && !this->initialize())
{
LogManager::getSingletonPtr()->logMessage("(Nimbus) Failed to initialize MenuMode");

// Terminate the application if we fail to initialize
return 0;
}

// Continue to run this runmode
return this;
}

bool MenuMode::initialize()
{
Entity* dragon;
SceneNode* dragonNode;

Light* light;

// Create the scene manager
mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");

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
OverlayContainer* resumeButton = static_cast<OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "NEWBUTTON"));
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
TextAreaOverlayElement* resume = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "NEWGAME"));
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
quit->setCaption("Exit Game");
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

// testing
printf("\nValues of Viewport in initialize()\n");
printf("Left of Resume Button = %f\n", float(mViewport->getActualWidth()) * 0.35);
printf("Right of Resume Button = %f\n", float(mViewport->getActualWidth()) * 0.65);
printf("Top of Resume Button = %f\n", float(mViewport->getActualHeight()) * 0.25);
printf("Bottom of Resume Button = %f\n", float(mViewport->getActualHeight()) * 0.35);
printf("Left of Quit Button = %f\n", float(mViewport->getActualWidth()) * 0.35);
printf("Right of Quit Button = %f\n", float(mViewport->getActualWidth()) * 0.65);
printf("Top of Quit Button = %f\n", float(mViewport->getActualHeight()) * 0.45);
printf("Bottom of Quit Button = %f\n", float(mViewport->getActualHeight()) * 0.55);

//////////
// Set up the appropriate models

// Correct the aspect ratio of the camera
mCamera->setAspectRatio(
Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

//// Load the dragon
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

void MenuMode::KeyListener::handleEvent(payloadmap payload)
{
	printf("I'm in the KeyListener!\n");
	OIS::KeyCode keyCode = *static_cast<OIS::KeyCode*>(payload["KeyCode"]);
	bool keyReleased = *static_cast<bool*>(payload["KeyReleased"]);

	if(keyCode == OIS::KC_P) 
	{
		
	}
}

void MenuMode::MouseListener::handleEvent(payloadmap payload)
{
	printf("I'm in the MouseListener!\n");
	OIS::MouseEvent* evt = (static_cast<OIS::MouseEvent*>(payload["MouseClicked"]));

	// testing
	printf("\nValues of Viewport in MouseListener\n");
	printf("Left of Resume Button = %f\n", float(viewport->getActualWidth()) * 0.35);
	printf("Right of Resume Button = %f\n", float(viewport->getActualWidth()) * 0.65);
	printf("Top of Resume Button = %f\n", float(viewport->getActualHeight()) * 0.25);
	printf("Bottom of Resume Button = %f\n", float(viewport->getActualHeight()) * 0.35);
	printf("Left of Quit Button = %f\n", float(viewport->getActualWidth()) * 0.35);
	printf("Right of Quit Button = %f\n", float(viewport->getActualWidth()) * 0.65);
	printf("Top of Quit Button = %f\n", float(viewport->getActualHeight()) * 0.45);
	printf("Bottom of Quit Button = %f\n", float(viewport->getActualHeight()) * 0.55);

	if((evt->state.X.abs >= (float(viewport->getActualWidth()) * 0.35)) && (evt->state.X.abs <= (float(viewport->getActualWidth()) * 0.65))
		&& (evt->state.Y.abs >= (float(viewport->getActualHeight()) * 0.25)) && (evt->state.Y.abs <= (float(viewport->getActualHeight()) * 0.35)))
	{
		printf("Resume Button Pressed!\n");
	}

	if((evt->state.X.abs >= (float(viewport->getActualWidth()) * 0.35)) && (evt->state.X.abs <= (float(viewport->getActualWidth()) * 0.65))
		&& (evt->state.Y.abs >= (float(viewport->getActualHeight()) * 0.45)) && (evt->state.Y.abs <= (float(viewport->getActualHeight()) * 0.55)))
	{
		printf("Quit Button Pressed!\n");
	}
}