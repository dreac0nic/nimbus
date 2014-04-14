#include "World.h"
#include "GameEntity.h"
#include "GameEntitySet.h"
#include "Terrain.h"
#include <OgreSceneManager.h>

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager):
	worldBounds(Ogre::Real(250), Ogre::Real(250))
{
	idCounter = 0;
	this->mSceneManager = sceneManager;
	this->mWorldNode = mSceneManager->createSceneNode();
	this->mEntities = new GameEntitySet();

	this->mTerrain = new Terrain(this);

	//mTerrain->printDiagram();
}

World::~World(void)
{
	delete this->mEntities;
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
