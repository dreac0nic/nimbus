#include "EnvironmentManager.h"
#include "World.h"
#include "WindManager.h"

using namespace Nimbus;

EnvironmentManager::EnvironmentManager(Ogre::SceneManager* sceneManager, World* world)
{
	mWorld = world;

	mWindManager = new WindManager(sceneManager, mWorld, mWorld->getWindMap()->getAlphaVector().length() / 7.0F);
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
