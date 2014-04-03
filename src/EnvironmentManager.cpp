#include "EnvironmentManager.h"
#include "World.h"
#include "WindManager.h"

using namespace Nimbus;

EnvironmentManager::EnvironmentManager(Ogre::SceneManager* sceneManager)
{
	// Temporary solution
	mWorld = new World(sceneManager);


	mWindManager = new WindManager(sceneManager, mWorld->windMap);
}

EnvironmentManager::~EnvironmentManager(void)
{
}

void EnvironmentManager::initialize(void)
{
}

bool EnvironmentManager::update(void)
{
	mWindManager->update();
	return true;
}
