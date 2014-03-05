#include "World.h"

using namespace Nimbus;

World::World(void)
{
}

World::~World(void)
{
}

void World::addEntity(GameEntity* entity)
{
	this->mEntities.push_back(entity);
}
