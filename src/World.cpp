#include "World.h"
#include "GameEntity.h"
#include "GameEntitySet.h"
#include "WindMap.h"
#include <OgreSceneManager.h>

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager):
	size(5000), worldBounds((Ogre::Real)size, (Ogre::Real)size), windMap((int)(worldBounds.x), (int)(worldBounds.y))
{
	idCounter = 0;
	this->mSceneManager = sceneManager;
	this->mWorldNode = mSceneManager->createSceneNode();
	this->mEntities = new GameEntitySet();
	this->mWindMap = new WindMap(0,0);
}

World::~World(void)
{
	delete this->mEntities;
	delete this->mWindMap;
}

int World::getCurrentId()
{
	return idCounter++;
}

void World::addEntity(GameEntity* entity)
{
	this->mEntities->addGameEntity(entity);
	entity->startup();
}
