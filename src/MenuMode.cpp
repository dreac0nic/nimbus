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

//Flags for determining actions
bool menuEndFlag = FALSE; 
bool overlayFlag = TRUE;

MenuMode::MenuMode(void)
{
	this->keyListener = new KeyListener();
	EventSystem::getSingleton()->registerListener(this->keyListener, EventSystem::EventType::KEY_PRESS);

	//Set up the pointer for the game to go to Menu Mode
	gameModePointer = NULL;
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
	if(gameModePointer == NULL)
	{
		gameModePointer = new GameMode();
	}
	// Removes the current Viewport and move on to the next RunMode
	NimbusApplication::getRenderWindow()->removeViewport(this->mViewport->getZOrder());
	EventSystem::getSingleton()->unregisterListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
	delete this->keyListener;
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
OverlayContainer* newGameButton = static_cast<OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "NEWBUTTON"));
newGameButton->setMetricsMode(GMM_RELATIVE);
newGameButton->setPosition(0.41, 0.3);
newGameButton->setDimensions(0.0, 0.0);
//newGameButton->setMaterialName("BaseWhite");

OverlayContainer* quitButton = static_cast<OverlayContainer*>(mOverlayMgr->createOverlayElement("Panel", "QUITBUTTON"));
quitButton->setMetricsMode(GMM_RELATIVE);
quitButton->setPosition(0.415, 0.5);
quitButton->setDimensions(0.0, 0.0);
//quitButton->setMaterialName("BaseWhite");


// Overlay element text
TextAreaOverlayElement* newGame = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "NEWGAME"));
newGame->setMetricsMode(GMM_PIXELS);
newGame->setFontName("Font");
newGame->setPosition(0.0, 0.0);
newGame->setDimensions(0.1, 0.1);
newGame->setCaption("Home Key to start a New Game");
newGame->setCharHeight(16);
//newGame->setColourBottom(ColourValue(1.0, 0.0, 0.0));
//newGame->setColourTop(ColourValue(0.0, 1.0, 0.0));
newGame->setColour(ColourValue(0.0, 1.0, 0.0));
printf("newGame name = %s\n",newGame->getName());


TextAreaOverlayElement* quit = static_cast<TextAreaOverlayElement*>(mOverlayMgr->createOverlayElement("TextArea", "QUIT"));
quit->setMetricsMode(GMM_PIXELS);
quit->setFontName("Font");
quit->setPosition(0.0, 0.0);
quit->setDimensions(0.1, 0.1);
quit->setCaption("Escape Key to Exit the Game");
quit->setCharHeight(16);
//quit->setColourBottom(ColourValue(1.0, 0.0, 0.0));
//quit->setColourTop(ColourValue(0.0, 1.0, 0.0));
quit->setColour(ColourValue(1.0, 0.0, 0.0));
printf("quit name = %s\n",quit->getName());

theOverlay->add2D(mPanel);
theOverlay->add2D(newGameButton);
theOverlay->add2D(quitButton);
newGameButton->addChild(newGame);
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

	//This checks if the correct key was pressed upon the release of it. (Kinda hacky, could be improved.)
	if(keyReleased && keyCode == OIS::KC_P) 
	{
		overlayFlag = !overlayFlag;
	}

	//Pressing the home key should change this flag. (For testing purposes for now)
	if(keyCode == OIS::KC_HOME) 
	{
		menuEndFlag = TRUE;
	}
}
