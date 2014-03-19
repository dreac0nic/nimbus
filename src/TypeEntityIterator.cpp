#include "GameEntitySet.h"

using namespace std;
using namespace Nimbus;

GameEntitySet::TypeEntityIterator::TypeEntityIterator(typeiterator& entity, list<GameEntity* >* entityList)
{
	this->mCurrentEntity = entity;
	this->mEntities = entityList;
}

GameEntitySet::TypeEntityIterator::TypeEntityIterator(const TypeEntityIterator& other)
{
	this->mCurrentEntity = other.mCurrentEntity;
	this->mEntities = other.mEntities;
}

const GameEntitySet::TypeEntityIterator& GameEntitySet::TypeEntityIterator::operator++()
{
	TypeEntityIterator temp = *this;
	++this->mCurrentEntity;
	return temp;
}

GameEntitySet::TypeEntityIterator& GameEntitySet::TypeEntityIterator::operator++(int junk)
{
	++this->mCurrentEntity;
	return *this;
}

GameEntity& GameEntitySet::TypeEntityIterator::operator*()
{
	return *(*this->mCurrentEntity);
}

GameEntity* GameEntitySet::TypeEntityIterator::operator->()
{
	return *this->mCurrentEntity;
}

bool GameEntitySet::TypeEntityIterator::operator==(const TypeEntityIterator& rhs)
{
	return this->mCurrentEntity == rhs.mCurrentEntity;
}

bool GameEntitySet::TypeEntityIterator::operator!=(const TypeEntityIterator& rhs)
{
	return this->mCurrentEntity != rhs.mCurrentEntity;
}