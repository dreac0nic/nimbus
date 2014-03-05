#include "World.h"

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager)
{
	this->mSceneManager = sceneManager;
}

World::~World(void)
{
}

void World::addEntity(GameEntity* entity)
{
	this->mEntities.push_back(entity);
}
