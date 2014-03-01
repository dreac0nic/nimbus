#include "GameEntity.h"
#include <sstream>

using namespace Nimbus;
using namespace std;

GameEntity::GameEntity(void)
{
}

GameEntity::GameEntity(map<string, string> cowmap)
{
	if(cowmap.find("age") != cowmap.end())
	{
		stringstream convert (cowmap["age"]);
		// convert >> this->mAge;
	}
}

GameEntity::~GameEntity(void)
{
}

bool GameEntity::add(Behaviour* behaviour)
{
}

void GameEntity::update(void)
{
}

void GameEntity::remove(Behaviour* behaviour)
{
}