#include "World.h"
#include "GameEntity.h"
#include "GameEntitySet.h"
#include "WindMap.h"
#include <OgreSceneManager.h>

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager):
	worldBounds(Ogre::Real(5000), Ogre::Real(5000))
{
	idCounter = 0;
	this->mSceneManager = sceneManager;
	this->mWorldNode = mSceneManager->createSceneNode();
	this->mEntities = new GameEntitySet();
	this->mWindMap = new WindMap((int)worldBounds.x, (int)worldBounds.y);
}

World::~World(void)
{
	delete this->mEntities;
	delete this->mWindMap;
}

void World::Initialize()
{
	mWindMap->Initialize();
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
