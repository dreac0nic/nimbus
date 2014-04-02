#include "EnvironmentManager.h"

using namespace Nimbus;

EnvironmentManager::EnvironmentManager(Ogre::SceneManager* sceneManager)
{
	mWindManager = new WindManager(sceneManager);
}

EnvironmentManager::~EnvironmentManager(void)
{
}

void EnvironmentManager::initialize(void)
{
}

bool EnvironmentManager::update(void)
{
	return true;
}
