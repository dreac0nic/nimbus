#include "GameMode.h"
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include "NimbusApplication.h"

using namespace Nimbus;
using namespace Ogre;

GameMode::GameMode(void)
{
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
	delete this->mEntityMan;
	delete this->mEnvironmentMan;
	delete this->mWorld;
}

RunMode* GameMode::run(const FrameEvent& evt)
{
	// Updating all of the entities through the manager
	this->mEntityMan->update();

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
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Hardcoding some things for now
	// Position the camera
	mCamera->setPosition(Vector3(0, 50, 80));
	mCamera->lookAt(Vector3(0, 0, -100));
	mCamera->setNearClipDistance(5);
	// Add a viewport for the camera
	mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);
	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	
	// Construct the game world (truthfully this should be done by the menu probably...
	//     this would allow for game configuration and loading... of maybe we
	//     should have a loading game mode.
	this->mWorld = new World(this->mSceneMgr);

	// Construct the world managers
	this->mEntityMan = new EntityManager(this->mWorld);
	this->mEnvironmentMan = new EnvironmentManager(this->mSceneMgr);

	// Configure entity types
	this->mEntityMan->configureEntityTypes("../../assets/scripts/ConfigFiles.ini", this->mWorld);

	// Initializing the managers
	this->mEnvironmentMan->initialize();
	this->mEntityMan->initialize();

	std::map<std::string, void*> entityType;
	entityType["EntityType"] = new std::string("Dragon");
	EventSystem::getSingleton()->fireEvent(EventSystem::CREATE_ENTITY, entityType);

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
	mContainingMode->mCreatingWind = true;
}

void GameMode::MouseUpdateListener::handleEvent(payloadmap payload, EventListener* responder)
{
	if (mContainingMode->mCreatingWind)
	{
		Ogre::Vector2* position = static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);
		Ogre::Ray ray = mContainingMode->mCamera->getCameraToViewportRay(
			position->x / mContainingMode->mViewport->getActualWidth(),
			position->y / mContainingMode->mViewport->getActualHeight());

		payloadmap mousePosRay;
		mousePosRay["Context"] = new std::string("Wind");
		mousePosRay["ScreenPosition"] = position;
		mousePosRay["WorldRay"] = &ray;
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::MOUSE_POSITION, mousePosRay);
	}
}

void GameMode::MouseUpListener::handleEvent(payloadmap payload, EventListener* responder)
{
	mContainingMode->mCreatingWind = false;

	payloadmap createCloud;
	std::string type = "Cloud";
	Vector3 position = Vector3(50, 30, -130);
	createCloud["EntityType"] = &type;
	createCloud["PositionVector"] = &position;
	EventSystem::getSingleton()->fireEvent(EventSystem::EventType::CREATE_ENTITY, createCloud);
}
