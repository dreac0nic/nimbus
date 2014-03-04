#include "GameEntity.h"
#include <OgreConfigFile.h>

using namespace Nimbus;
using namespace Ogre;

GameEntity::GameEntity(void)
{
}

GameEntity::GameEntity(ConfigFile::SettingsMultiMap* initializingSettings)
{
	ConfigFile::SettingsMultiMap::iterator currentSetting;
	std::stringstream settingConverter;

	currentSetting = initializingSettings->find("age");
	settingConverter = std::stringstream((*currentSetting).second);
	settingConverter >> this->mAge;

	currentSetting = initializingSettings->find("name");
	this->mEntityType = (*currentSetting).second;
}

GameEntity::~GameEntity(void)
{
}

bool GameEntity::add(Behaviour* behaviour)
{
	return false;
}

void GameEntity::update(void)
{
}

void remove(Behaviour* behaviour)
{
}