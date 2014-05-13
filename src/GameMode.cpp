#include "GameMode.h"
#include "MenuMode.h"
#include "Camera.h"
#include "Map.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"
#include <OgreOverlay.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreFont.h>

using namespace Nimbus;
using namespace Ogre;

//Flag for determining Actions
bool pauseModeFlag = false;

GameMode::GameMode(void)
{
	this->keyListener = new KeyListener();
	EventSystem::getSingleton()->registerListener(this->keyListener, EventSystem::EventType::KEY_PRESS);

	menuModePointer = NULL;
	// Initialize the listeners
	this->mMouseDownListener = new MouseDownListener(this);
	this->mMouseUpdateListener = new MouseUpdateListener(this);
	this->mMouseUpListener = new MouseUpListener(this);

}

GameMode::~GameMode(void)
{
	// Delete the listener objects
	delete this->mMouseDownListener;
	delete this->mMouseUpdateListener;
	delete this->mMouseUpListener;

	// Delete dynamic member variables... I think this is the right place for this.

	if (mEntityMan) delete this->mEntityMan;
	if (mEnvironmentMan) delete this->mEnvironmentMan;
	if (mWorld) delete this->mWorld;

	
	delete this->mCamera;
}

RunMode* GameMode::run(const FrameEvent& evt)
{
	if(pauseModeFlag)
	{
		std:: cout << "OK, going to the pause Menu\n";
		if(menuModePointer == NULL)
		{
			menuModePointer = new MenuMode();
			std::cout << "Created a new MenuMode";
		}

		// Removes the current Viewport and move on to the next RunMode
		//NimbusApplication::getRenderWindow()->removeViewport(this->mViewport->getZOrder());
		//EventSystem::getSingleton()->unregisterListener(this->keyListener, EventSystem::EventType::KEY_PRESS);
		//delete this->keyListener;
		return menuModePointer;

	}
	// Update the camera... without this line, the camera, oddly, wasn't updating... wonder why?...
	this->mCamera->update();


	// Updating all of the entities through the manager
	this->mEntityMan->update();
	mEnvironmentMan->update();

	// Update elapsed time
	elapsedTime += evt.timeSinceLastEvent;

	// Fire off a tick event if appropriate
	if(elapsedTime >= timePerTick)
	{
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::TICK);
		elapsedTime = 0;
	}

	// Continue to run this runmode
	return this;
}

void GameMode::initialize()
{
	// Create the scene manager
	mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");
	
	// Initialize the camera
	mCamera = new Camera();
	mCamera->initialize(mSceneMgr);

	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	// Construct the game world (truthfully this should be done by the menu probably...
	//     this would allow for game configuration and loading... of maybe we
	//     should have a loading game mode.
	this->mWorld = new World(this->mSceneMgr);

	// Construct the world managers
	this->mEntityMan = new EntityManager(this->mWorld);
	this->mEnvironmentMan = new EnvironmentManager(this->mSceneMgr, this->mWorld);

	// Configure entity types
	this->mEntityMan->configureEntityTypes("../../assets/scripts/ConfigFiles.ini", this->mWorld);

	// Initializing the managers
	this->mEnvironmentMan->initialize();
	this->mEntityMan->initialize();

	std::map<std::string, void*> entityType;
	entityType["EntityType"] = new std::string("Dragon");
	//EventSystem::getSingleton()->fireEvent(EventSystem::CREATE_ENTITY, entityType);

	// Adding the world root node to the actual scene
	this->mSceneMgr->getRootSceneNode()->addChild(this->mWorld->getWorldNode());

	// Listening to events for mouse
	EventSystem::getSingleton()->registerListener(this->mMouseDownListener, EventSystem::EventType::MOUSE_DOWN);
	EventSystem::getSingleton()->registerListener(this->mMouseUpdateListener, EventSystem::EventType::MOUSE_UPDATE);
	EventSystem::getSingleton()->registerListener(this->mMouseUpListener, EventSystem::EventType::MOUSE_UP);

	// Setting the wind creation to false
	mCreatingWind = false;

	// Initialize time constants
	timePerTick = 1; // 1.0 = one tick per second
	elapsedTime = 0;
}

void GameMode::pause(void)
{
	// Unregister the mouse listeners
	EventSystem::getSingleton()->unregisterListener(this->mMouseDownListener, EventSystem::EventType::MOUSE_DOWN);
	EventSystem::getSingleton()->unregisterListener(this->mMouseUpdateListener, EventSystem::EventType::MOUSE_UPDATE);
	EventSystem::getSingleton()->unregisterListener(this->mMouseUpListener, EventSystem::EventType::MOUSE_UP);
}

void GameMode::stop(void)
{
	// Unregister the mouse listeners
	EventSystem::getSingleton()->unregisterListener(this->mMouseDownListener, EventSystem::EventType::MOUSE_DOWN);
	EventSystem::getSingleton()->unregisterListener(this->mMouseUpdateListener, EventSystem::EventType::MOUSE_UPDATE);
	EventSystem::getSingleton()->unregisterListener(this->mMouseUpListener, EventSystem::EventType::MOUSE_UP);
}

void GameMode::MouseDownListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Let's not send a payload for now. This is dependent on the
	// input development and user interface development to continue.
	// payloadmap termPayload;
	// We need to know what was clicked and who should know about it.
	// That means input delegation to send to overlays/user interface,
	// wind, or whatnot.
	OIS::MouseButtonID* id = static_cast<OIS::MouseButtonID*>(payload["ButtonPressed"]);
	Ogre::Vector2* position = static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);
	Ogre::Ray ray = mContainingMode->mCamera->getCamera()->getCameraToViewportRay(
		position->x / mContainingMode->mCamera->getViewport()->getActualWidth(),
		position->y / mContainingMode->mCamera->getViewport()->getActualHeight());

	payloadmap mousePosRay;
	mousePosRay["Context"] = new std::string("Wind");
	mousePosRay["ButtonPressed"] = id;
	mousePosRay["ScreenPosition"] = position;
	mousePosRay["WorldRay"] = &ray;

	mContainingMode->mCreatingWind = true;

	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION_START, mousePosRay);
}

void GameMode::MouseUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if (mContainingMode->mCreatingWind)
	{
		Ogre::Vector2* position = static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);
		Ogre::Ray ray = mContainingMode->mCamera->getCamera()->getCameraToViewportRay(
			position->x / mContainingMode->mCamera->getViewport()->getActualWidth(),
			position->y / mContainingMode->mCamera->getViewport()->getActualHeight());

		payloadmap mousePosRay;
		mousePosRay["Context"] = new std::string("Wind");
		mousePosRay["ScreenPosition"] = position;
		mousePosRay["WorldRay"] = &ray;
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION_UPDATE, mousePosRay);
	}

	// Look for edge events
	if(payload.find("ScreenPosition") != payload.end())
	{
		Ogre::Vector2* position = static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);

		// Not using this for now, because if our mouse skips outside of the threshold
		// into the middle of the screen, the camera continues to move. To combat this,
		// we are going to pass all mouse events to our camera.
		// If within a screen threshold
		/*if(position->x < Camera::getScreenThreshold()*2 ||
			position->x > (mContainingMode->mCamera->getViewport()->getActualWidth() - Camera::getScreenThreshold()*2) ||
			position->y < Camera::getScreenThreshold()*2 ||
			position->y > (mContainingMode->mCamera->getViewport()->getActualHeight() - Camera::getScreenThreshold()*2))*/
		{
			// Initialize payload values
			payloadmap mousePosPayload;
			std::string context = "Screen";

			// Store values in the payload
			mousePosPayload["Context"] = &context;
			mousePosPayload["ScreenPosition"] = position;

			// Fire the event
			EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION_UPDATE, mousePosPayload);
		}
	}
}

void GameMode::MouseUpListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Let's not send a payload for now. This is dependent on the
	// input development and user interface development to continue.
	// payloadmap termPayload;
	// We need to know what was clicked and who should know about it.
	// That means input delegation to send to overlays/user interface,
	// wind, or whatnot.
	OIS::MouseButtonID* id = static_cast<OIS::MouseButtonID*>(payload["ButtonPressed"]);
	Ogre::Vector2* position = static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);
	Ogre::Ray ray = mContainingMode->mCamera->getCamera()->getCameraToViewportRay(
		position->x / mContainingMode->mCamera->getViewport()->getActualWidth(),
		position->y / mContainingMode->mCamera->getViewport()->getActualHeight());

	payloadmap mousePosRay;
	mousePosRay["Context"] = new std::string("Wind");
	mousePosRay["ButtonPressed"] = id;
	mousePosRay["ScreenPosition"] = position;
	mousePosRay["WorldRay"] = &ray;

	if(mContainingMode->mCreatingWind)
	{
		mContainingMode->mCreatingWind = false;

		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION_END, mousePosRay);
	}
}

void GameMode::KeyListener::handleEvent(payloadmap payload, EventListener* responder)
{
	OIS::KeyCode keyCode = *static_cast<OIS::KeyCode*>(payload["KeyCode"]);
	bool keyReleased = *static_cast<bool*>(payload["KeyReleased"]);

	//This checks if the correct key was pressed upon the release of it. (Kinda hacky, could be improved.)
	if(keyReleased && keyCode == OIS::KC_P) 
	{
		pauseModeFlag = !pauseModeFlag;
		std::cout <<  pauseModeFlag << "\nThe game should go into pause mode.";
	}

}

