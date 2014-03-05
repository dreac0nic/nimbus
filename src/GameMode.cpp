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
}

GameMode::~GameMode(void)
{
	// Delete dynamic member variables... I think this is the right place for this.
	delete this->mEntityMan;
	delete this->mEnvironmentMan;
	delete this->mWorld;
}

RunMode* GameMode::run(const FrameEvent& evt)
{
	// Attempt to initialize the run mode
	if(!this->initialized && !this->initialize())
	{
		LogManager::getSingletonPtr()->logMessage("(Nimbus) Failed to initialize RunMode");

		// Terminate the application if we fail to initialize
		return 0;
	}

	// Updating all of the entities through the manager
	this->mEntityMan->update();

	// Continue to run this runmode
	return this;
}

bool GameMode::initialize()
{
	//this->mEntityFactory = new EntityFactory(this->mWorld, "../../assets/scripts/ConfigFiles.ini");
	
	// Create the scene manager
	mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	
	// Construct the game world (truthfully this should be done by the menu probably...
	//     this would allow for game configuration and loading... of maybe we
	//     should have a loading game mode.
	this->mWorld = new World(this->mSceneMgr);

	// Construct the world managers
	this->mEntityMan = new EntityManager(this->mWorld);
	this->mEnvironmentMan = new EnvironmentManager();

	// Configure entity types
	this->mEntityMan->configureEntityTypes("../../assets/scripts/ConfigFiles.ini", this->mWorld);

	std::map<std::string, void*> entityType;
	entityType["EntityType"] = new std::string("Dragon");
	EventSystem::getSingleton()->fireEvent(EventSystem::CREATE_ENTITY, entityType);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
