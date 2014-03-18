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

GameEntitySet::EntityIterator& GameEntitySet::begin(GameEntityType type)
{
	if(type == "")
	{
		return *(new GeneralEntityIterator(this->mEntitiesById.begin(), &this->mEntitiesById));
	}

	return *(new GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById));
}

GameEntitySet::EntityIterator& GameEntitySet::end(GameEntityType type)
{
	if(type == "")
	{
		return *(new GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById));
	}

	return *(new GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById));
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
	GameEntitySet::EntityIterator& other)
{
	this->currentEntity = this->mEntityMap->find(other->getEntityId());
}

GameEntitySet::EntityIterator& GameEntitySet::GeneralEntityIterator::operator++()
{
	this->currentEntity = ++this->currentEntity;
	return *this;
}

GameEntitySet::EntityIterator& GameEntitySet::GeneralEntityIterator::operator++(int junk)
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

bool GameEntitySet::GeneralEntityIterator::operator==(GameEntitySet::EntityIterator& rhs)
{
	if (this->currentEntity != mEntityMap->end())
	{
		return this->currentEntity->second == &(*(rhs));
	}
	return this == &rhs;
}

bool GameEntitySet::GeneralEntityIterator::operator!=(GameEntitySet::EntityIterator& rhs)
{
	if (this->currentEntity != mEntityMap->end())
	{
		return (this->currentEntity->second) != &(*(rhs));
	}
	return this != &rhs;
}
