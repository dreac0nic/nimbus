#include "GameEntity.h"
#include <OgreConfigFile.h>

using namespace Nimbus;
using namespace Ogre;

GameEntity::GameEntity(void)
{
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
}

void remove(Behaviour* behaviour)
{
}