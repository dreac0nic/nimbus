#include "World.h"
#include "GameEntity.h"
#include <OgreSceneManager.h>

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager)
{
	idCounter = 0;
	this->mSceneManager = sceneManager;
	this->mWorldNode = mSceneManager->createSceneNode();
}

World::~World(void)
{
}

int World::getCurrentId()
{
	return idCounter++;
}

void World::addEntity(GameEntity* entity)
{
	this->mEntities.push_back(entity);
	entity->startup();
}
