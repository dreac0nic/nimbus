#include "GameEntitySet.h"

using namespace Nimbus;

GameEntitySet::GameEntitySet(void)
{
}

GameEntitySet::~GameEntitySet(void)
{
}

void GameEntitySet::addGameEntity(GameEntity* entity)
{
}

GameEntitySet::entityiterator GameEntitySet::getIterator(GameEntityType type)
{
	return GeneralEntityIterator(NULL);
}

//////////
// Iterator Definitions

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(GameEntity* entity)
{
}

GameEntitySet::GeneralEntityIterator::GeneralEntityIterator(
	const GameEntitySet::GeneralEntityIterator& other)
{
}

GameEntitySet::GeneralEntityIterator& GameEntitySet::GeneralEntityIterator::operator++()
{
	return *(new GeneralEntityIterator(NULL));
}

GameEntitySet::GeneralEntityIterator& GameEntitySet::GeneralEntityIterator::operator++(int junk)
{
	return *(new GeneralEntityIterator(NULL));
}

GameEntity& GameEntitySet::GeneralEntityIterator::operator*()
{
	return *(new GameEntity(-1));
}

GameEntity* GameEntitySet::GeneralEntityIterator::operator->()
{
	return NULL;
}

bool GameEntitySet::GeneralEntityIterator::operator==(const GameEntitySet::GeneralEntityIterator& rhs)
{
	return false;
}

bool GameEntitySet::GeneralEntityIterator::operator!=(const GameEntitySet::GeneralEntityIterator& rhs)
{
	return false;
}
