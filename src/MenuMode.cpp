#include "MenuMode.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"
#include "GameMode.h"
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreFont.h>
#include <OgreFontManager.h>

using namespace Nimbus; 
using namespace Ogre;

bool menuEndFlag = FALSE; 
bool overlayFlag = FALSE;
MenuMode::MenuMode(void)
{
	this->keyListener = new KeyListener();
	GameMode* gameAtHand = new GameMode();
	gameModePointer = gameAtHand;
	EventSystem::getSingleton()->registerListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
}

MenuMode::~MenuMode(void)
{
	EventSystem::getSingleton()->unregisterListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
	delete this->keyListener;

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

//Check for if the overlay needs to be enabled.
if(overlayFlag)
{
	this->mViewport->setOverlaysEnabled(TRUE);
} 
else if(!overlayFlag)
{
	this->mViewport->setOverlaysEnabled(FALSE);
}
//Checks to see if it needs to change into GameMode
if(menuEndFlag)
{
	std::cout << "menuEndFlag is now true.";
	return gameModePointer;
}

// Continue to run this runmode
return this;
}

bool MenuMode::initialize()
{

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
resume->setCaption("New Game");
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

mViewport->setOverlaysEnabled(FALSE);
//////////
// Set up the appropriate models

// Correct the aspect ratio of the camera
mCamera->setAspectRatio(
Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

//// Load the dragon
//dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
//dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
//dragonNode->attachObject(dragon);
//
//dragonNode->setPosition(0, 0, -100);
//dragonNode->setScale(40.0, 40.0, 40.0);
//dragonNode->pitch(Degree(90));

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
	OIS::KeyCode keyCode = *static_cast<OIS::KeyCode*>(payload["KeyCode"]);
	bool keyReleased = *static_cast<bool*>(payload["KeyReleased"]);

	if(keyCode == OIS::KC_P) 
	{
		overlayFlag = !overlayFlag;
		std::cout << overlayFlag << "\n";
	}

	//Pressing the home key should change this flag.
	if(keyCode == OIS::KC_HOME) 
	{
		menuEndFlag = TRUE;
		std::cout << menuEndFlag << "\n";
	}
}
