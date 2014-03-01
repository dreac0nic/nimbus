#include "EntityFactory.h"

using namespace Nimbus;

EntityFactory::EntityFactory(std::string entityDefinitionFile)
{
	// LOAD ENTITY DEFINITIONS

}

EntityFactory::~EntityFactory(void)
{
	// EMPTY DESTRUCTOR
}

GameEntity* EntityFactory::createEntity(std::string entityType)
{
	GameEntity* factorizedEntity = new GameEntity;

	return factorizedEntity;
}