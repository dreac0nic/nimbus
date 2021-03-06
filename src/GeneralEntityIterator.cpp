#include "GameEntitySet.h"

using namespace std;
using namespace Nimbus;

// General Entity Iterator
// Designed to iterate over all entities one at a time

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(generaliterator initial,
															std::map<GameEntityId, GameEntity*>* entityMap)
{
	this->currentEntity = initial;
	this->mEntityMap = entityMap;
}

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(
	const GameEntitySet::GeneralEntityIterator& other)
{
	this->currentEntity = other.currentEntity;
	this->mEntityMap = other.mEntityMap;
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
