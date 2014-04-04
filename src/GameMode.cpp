#include "GameMode.h"
#include "Camera.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"

using namespace Nimbus;
using namespace Ogre;

GameMode::GameMode(void)
{
}

GameMode::~GameMode(void)
{
	// Delete dynamic member variables... I think this is the right place for this.
	delete this->mEntityMan;
	delete this->mEnvironmentMan;
	delete this->mWorld;
	delete this->mCamera;
}

RunMode* GameMode::run(const FrameEvent& evt)
{
	// Update the camera... without this line, the camera, oddly, wasn't updating... wonder why?...
	this->mCamera->update();

	// Updating all of the entities through the manager
	this->mEntityMan->update();
	mEnvironmentMan->update();

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
	this->mWorld->Initialize();

	std::map<std::string, void*> entityType;
	entityType["EntityType"] = new std::string("Dragon");
	EventSystem::getSingleton()->fireEvent(EventSystem::CREATE_ENTITY, entityType);

	// Adding the world root node to the actual scene
	this->mSceneMgr->getRootSceneNode()->addChild(this->mWorld->getWorldNode());

	// Listening to events for mouse
	EventSystem::getSingleton()->registerListener(new MouseDownListener(this), EventSystem::EventType::MOUSE_DOWN);
	EventSystem::getSingleton()->registerListener(new MouseUpdateListener(this), EventSystem::EventType::MOUSE_UPDATE);
	EventSystem::getSingleton()->registerListener(new MouseUpListener(this), EventSystem::EventType::MOUSE_UP);

	// Setting the wind creation to false
	mCreatingWind = false;
}

void GameMode::pause(void)
{
}

void GameMode::stop(void)
{
}

void GameMode::MouseDownListener::handleEvent(payloadmap payload, EventListener* responder)
{
	payloadmap termPayload;

	mContainingMode->mCreatingWind = true;

	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION_START, termPayload);
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

		// If within a screen threshold
		if(position->x < Camera::getScreenThreshold()*2 ||
			position->x > (mContainingMode->mCamera->getViewport()->getActualWidth() - Camera::getScreenThreshold())*2 ||
			position->y < Camera::getScreenThreshold()*2 ||
			position->y > (mContainingMode->mCamera->getViewport()->getActualHeight() - Camera::getScreenThreshold()*2))
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
	payloadmap termPayload;

	mContainingMode->mCreatingWind = false;

	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION_END, termPayload);
}
