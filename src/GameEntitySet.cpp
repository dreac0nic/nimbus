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

GameEntitySet::GeneralEntityIterator GameEntitySet::begin(GameEntityType type)
{
	if(type == "")
	{
		return GeneralEntityIterator(this->mEntitiesById.begin(), &this->mEntitiesById);
	}

	return GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById);
}

GameEntitySet::GeneralEntityIterator GameEntitySet::end(GameEntityType type)
{
	if(type == "")
	{
		return GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById);
	}

	return GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById);
}

//////////
// Iterator Definitions

// General Entity Iterator
// Designed to iterate over all entities one at a time

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(generaliterator& initial,
															std::map<GameEntityId, GameEntity*>* entityMap)
{
	this->currentEntity = initial;
	this->mEntityMap = entityMap;
}

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(
	const GameEntitySet::GeneralEntityIterator& other)
{
	this->currentEntity = other.currentEntity;
}

const GameEntitySet::GeneralEntityIterator& GameEntitySet::GeneralEntityIterator::operator++()
{
	this->currentEntity = ++this->currentEntity;
	return *this;
}

GameEntitySet::GeneralEntityIterator& GameEntitySet::GeneralEntityIterator::operator++(int junk)
{
	++this->currentEntity;
	return *this;
}

GameEntity& GameEntitySet::GeneralEntityIterator::operator*()
{
	return *(*this->currentEntity).second;
}

GameEntity* GameEntitySet::GeneralEntityIterator::operator->()
{
	return (*this->currentEntity).second;
}

bool GameEntitySet::GeneralEntityIterator::operator==(const GameEntitySet::GeneralEntityIterator& rhs)
{
	if (this->currentEntity == mEntityMap->end() || rhs.currentEntity == mEntityMap->end())
	{
		return this->currentEntity == rhs.currentEntity;
	}
	return (*this->currentEntity).second == (*rhs.currentEntity).second;
}

bool GameEntitySet::GeneralEntityIterator::operator!=(const GameEntitySet::GeneralEntityIterator& rhs)
{
	if (this->currentEntity == mEntityMap->end() || rhs.currentEntity == mEntityMap->end())
	{
		return this->currentEntity != rhs.currentEntity;
	}
	return this->currentEntity->second != rhs.currentEntity->second;
}
