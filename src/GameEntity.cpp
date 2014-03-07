#include "GameEntity.h"
#include <OgreConfigFile.h>

using namespace Nimbus;
using namespace Ogre;

GameEntity::GameEntity(GameEntityId id, GameEntityType type) :
	mId(id),
	mEntityType(type)
{
}

GameEntity::GameEntity(GameEntityId id, GameEntity* other) :
	mId(id)
{
	// Copying all of the properties and behaviours in the given entity to this new entity
	this->mEntityType = other->getEntityType();

	behaviourmap::iterator behaviour = other->getBehaviours()->begin();
	while (behaviour != other->getBehaviours()->end())
	{
		this->mBehaviours[behaviour->first] = behaviour->second->clone();
		behaviour++;
	}
}

GameEntity::~GameEntity(void)
{
}

void GameEntity::configure(ConfigFile::SettingsMultiMap* initializingSettings)
{
	ConfigFile::SettingsMultiMap::iterator currentSetting;
	std::stringstream settingConverter;

	currentSetting = initializingSettings->find("name");
	if (currentSetting != initializingSettings->end())
	{
		this->mEntityType = (*currentSetting).second;
	}
}

void GameEntity::startup()
{
	behaviourmap::iterator behaviour = mBehaviours.begin();
	while (behaviour != mBehaviours.end())
	{
		behaviour->second->startup();
		behaviour++;
	}
}

bool GameEntity::addBehaviour(Behaviour* behaviour, std::string name)
{
	this->mBehaviours[name] = behaviour;
	return true;
}

void GameEntity::update(void)
{
	behaviourmap::iterator behaviour = mBehaviours.begin();
	while (behaviour != mBehaviours.end())
	{
		behaviour->second->update();
		behaviour++;
	}
}

void GameEntity::removeBehaviour(Behaviour* behaviour)
{
}