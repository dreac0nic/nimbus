#include "World.h"
#include "GameEntity.h"
#include "GameEntitySet.h"
#include "Map.h"
#include "WindMap.h"

#include <OgreEntity.h>
#include <OgreSceneManager.h>

using namespace Nimbus;

World::World(Ogre::SceneManager* sceneManager):
	worldBounds(Ogre::Real(1000), Ogre::Real(1000))
{
	idCounter = 0;
	this->mSceneManager = sceneManager;
	this->mWorldNode = mSceneManager->createSceneNode();
	this->mEntities = new GameEntitySet();
	this->mMap = new Map();
	this->mWindMap = new WindMap(worldBounds.x, 50.0F /* 50 is an arbitrary debug value. CHANGE THIS. */, worldBounds/2.0F, 5.0f);

	// We're creating some static map generation here

	// Create material for tiles: NOTE SHOULD BE PROPERLY DONE SOON
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create("Tiles/Default",
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::Entity* terrainEntity = mSceneManager->createEntity("testTerrain", this->mMap->getMesh("terrainMesh"));
	terrainEntity->setMaterialName("Tiles/Default");
	Ogre::SceneNode* terrainNode = this->mWorldNode->createChildSceneNode();
	terrainNode->attachObject(terrainEntity);
	terrainNode->setPosition(0, -120, 0);
	terrainNode->setScale(40.0f, 40.0f, 40.0f);
}

World::~World(void)
{
	delete this->mEntities;
	delete this->mMap;
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
