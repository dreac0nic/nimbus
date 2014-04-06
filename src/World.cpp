#include "World.h"
#include "GameEntity.h"
#include "GameEntitySet.h"
#include "WindMap.h"
#include <OgreSceneManager.h>

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager):
	worldBounds(Ogre::Real(250), Ogre::Real(250))
{
	idCounter = 0;
	this->mSceneManager = sceneManager;
	this->mWorldNode = mSceneManager->createSceneNode();
	this->mEntities = new GameEntitySet();
	this->mWindMap = new WindMap(worldBounds.x, 50.0F /* 50 is an arbitrary debug value. CHANGE THIS. */, worldBounds/2.0F);
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
