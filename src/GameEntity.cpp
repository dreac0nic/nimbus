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

void GameEntity::configure(ConfigFile::SettingsMultiMap* configurationSettings)
{
	ConfigFile::SettingsMultiMap::iterator currentSetting;
	std::stringstream settingConverter;

	currentSetting = configurationSettings->find("age");
	if (currentSetting != configurationSettings->end())
	{
		settingConverter = std::stringstream((*currentSetting).second);
		settingConverter >> this->mAge;
	}

	currentSetting = configurationSettings->find("name");
	if (currentSetting != configurationSettings->end())
	{
		this->mEntityType = (*currentSetting).second;
	}
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