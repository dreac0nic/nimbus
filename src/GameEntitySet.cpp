#include "GameEntitySet.h"

using namespace Nimbus;

GameEntitySet::GameEntitySet(void) :
	mEntitiesByBehaviour(), mEntitiesById(), mEntitiesByType()
{
}

GameEntitySet::~GameEntitySet(void)
{
}

void GameEntitySet::addGameEntity(GameEntity* entity)
{
	// Add the entity to the list of entities
	this->mEntitiesById[entity->getEntityId()] = entity;

	// Add the entity to the appropriate entity type list
	this->mEntitiesByType[entity->getEntityType()].push_back(entity);

	// Add the entity to the appropriate behaviour lists
	for(behaviourmap::iterator it = entity->getBehaviours()->begin(); it != entity->getBehaviours()->end(); ++it)
	{
		this->mEntitiesByBehaviour[(*it).second->getBehaviourType()].push_back(entity);
	}
}

GameEntitySet::GeneralEntityIterator GameEntitySet::beginGeneralIterator()
{
	return GeneralEntityIterator(this->mEntitiesById.begin(), &this->mEntitiesById);
}

GameEntitySet::GeneralEntityIterator GameEntitySet::endGeneralIterator()
{
	return GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById);
}

GameEntitySet::TypeEntityIterator GameEntitySet::beginTypeIterator(GameEntityType type)
{
	return TypeEntityIterator(this->mEntitiesByType[type].begin(), &this->mEntitiesByType[type]);
}

GameEntitySet::TypeEntityIterator GameEntitySet::endTypeIterator(GameEntityType type)
{
	return TypeEntityIterator(this->mEntitiesByType[type].end(), &this->mEntitiesByType[type]);
}
