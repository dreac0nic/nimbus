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

	// Continue to run this runmode
	return this;
}

bool GameMode::initialize()
{
	this->mEntityFactory = new EntityFactory(this->mWorld, "../../assets/scripts/ConfigFiles.ini");

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
