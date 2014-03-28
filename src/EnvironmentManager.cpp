#include "EnvironmentManager.h"

using namespace Nimbus;

EnvironmentManager::EnvironmentManager(Ogre::SceneManager* sceneManager)
{
	mWindManager = new WindManager(sceneManager, 100, 100);
}

EnvironmentManager::~EnvironmentManager(void)
{
}

bool EnvironmentManager::update(void)
{
	return true;
}
