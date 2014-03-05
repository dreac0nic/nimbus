#include "GameEntity.h"
#include <OgreConfigFile.h>

using namespace Nimbus;
using namespace Ogre;

GameEntity::GameEntity(void)
{
}

GameEntity::GameEntity(GameEntity* other)
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

	currentSetting = initializingSettings->find("age");
	if (currentSetting != initializingSettings->end())
	{
		settingConverter = std::stringstream((*currentSetting).second);
		settingConverter >> this->mAge;
	}

	currentSetting = initializingSettings->find("name");
	if (currentSetting != initializingSettings->end())
	{
		this->mEntityType = (*currentSetting).second;
	}
}

bool GameEntity::add(Behaviour* behaviour)
{
	this->mBehaviours[behaviour->getBehaviourType()] = behaviour;
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

void remove(Behaviour* behaviour)
{
}