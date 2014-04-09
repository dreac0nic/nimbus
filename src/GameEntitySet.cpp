#include "GameEntitySet.h"
#include <algorithm>

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

void GameEntitySet::removeGameEntity(GameEntityId id)
{
	GameEntity* entity = mEntitiesById[id];
	std::list<GameEntity* >* tempList;
	std::list<GameEntity* >::iterator eraseElement;
	std::map<GameEntityId, GameEntity* >::iterator eraseMapElement;

	// Delete the entity from the type list
	tempList = &mEntitiesByType[entity->getEntityType()];
	eraseElement = std::find(tempList->begin(), tempList->end(), entity);
	tempList->erase(eraseElement);

	// Delete the entity from each behaviour list as necessary
	for(behaviourmap::iterator it = entity->getBehaviours()->begin(); it != entity->getBehaviours()->end(); ++it)
	{
		// Shut down the behaviour
		it->second->shutdown();

		tempList = &mEntitiesByBehaviour[it->second->getBehaviourType()];
		eraseElement = std::find(tempList->begin(), tempList->end(), entity);
		tempList->erase(eraseElement);
	}

	// Delete the entity from the entity map
	eraseMapElement = mEntitiesById.find(entity->getEntityId());
	mEntitiesById.erase(eraseMapElement);

	// Delete the actual entity
	delete entity;
}

GameEntitySet::GeneralEntityIterator GameEntitySet::beginGeneralIterator()
{
	return GeneralEntityIterator(this->mEntitiesById.begin(), &this->mEntitiesById);
}

GameEntitySet::GeneralEntityIterator GameEntitySet::endGeneralIterator()
{
	return GeneralEntityIterator(this->mEntitiesById.end(), &this->mEntitiesById);
}

std::list<GameEntity*> GameEntitySet::getEntitiesOfType(std::string type)
{
	return this->mEntitiesByType[type];
}

std::list<GameEntity*> GameEntitySet::getEntitiesWithBehaviour(std::string behaviour)
{
	return this->mEntitiesByBehaviour[behaviour];
}

GameEntity* GameEntitySet::getEntity(int id)
{
	return this->mEntitiesById[id];
}