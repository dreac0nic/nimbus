#include "GameEntitySet.h"

using namespace Nimbus;

GameEntitySet::GameEntitySet(void)
{
	this->mEntitiesByBehaviour.clear();
	this->mEntitiesById.clear();
	this->mEntitiesByType.clear();
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
		return GeneralEntityIterator(this->mEntitiesById.begin());
	}

	return GeneralEntityIterator(this->mEntitiesById.end());
}

GameEntitySet::GeneralEntityIterator GameEntitySet::end(GameEntityType type)
{
	if(type == "")
	{
		return GeneralEntityIterator(this->mEntitiesById.end());
	}

	return GeneralEntityIterator(this->mEntitiesById.end());
}

//////////
// Iterator Definitions

// General Entity Iterator
// Designed to iterate over all entities one at a time

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(generaliterator& initial)
{
	this->currentEntity = initial;
}

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(
	const GameEntitySet::GeneralEntityIterator& other)
{
	this->currentEntity = other.currentEntity;
}

GameEntitySet::GeneralEntityIterator& GameEntitySet::GeneralEntityIterator::operator++()
{
	GeneralEntityIterator temp(*this);
	++this->currentEntity;
	return temp;
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
	return (*this->currentEntity).second == (*rhs.currentEntity).second;
}

bool GameEntitySet::GeneralEntityIterator::operator!=(const GameEntitySet::GeneralEntityIterator& rhs)
{
	return (*this->currentEntity).second != (*rhs.currentEntity).second;
}
