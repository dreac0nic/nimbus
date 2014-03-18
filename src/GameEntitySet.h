#ifndef NIMBUS_GAMEENTITYSET_H
#define NIMBUS_GAMEENTITYSET_H

#include <iterator>
#include <map>

#include "GameEntity.h"

namespace Nimbus
{
	
	/* The GameEntitySet is a class for holding all the entities in the game
	world. It's main benefit and purpose is to provide custom iterators over
	entities so that users can select specific relevant sets of entities
	based on their component behaviours. */
	class GameEntitySet
	{
	public:
		class EntityIterator :
			public std::iterator<std::input_iterator_tag, GameEntity>
		{
		public:
			// Post increment
			virtual EntityIterator& operator++() { return *this; }
			// Pre increment
			virtual EntityIterator& operator++(int junk) { return *this; }

			// Dereference
			virtual GameEntity& operator*() { return *(new GameEntity(0, "null")); }
			// Functional dereference
			virtual GameEntity* operator->() { return NULL; }

			// Equality
			virtual bool operator==(EntityIterator& rhs) { return false; }
			// Inequality
			virtual bool operator!=(EntityIterator& rhs) { return true; }
		};
	
	//private:
		// Private iterator implementations... umm I'd like to make this private
		
		/* An implementation of the entity iterator that iterates over all the entities
		in the entity set.
		*/
		class GeneralEntityIterator :
			public EntityIterator
		{
		private:
			typedef std::map<GameEntityId, GameEntity*>::iterator generaliterator;
			std::map<GameEntityId, GameEntity*>* mEntityMap;

			// The current entity
			generaliterator currentEntity;

		public:
			// Umm... not sure why we need this
			GeneralEntityIterator(generaliterator& entity, std::map<GameEntityId, GameEntity*>* entityMap);
			// Copy constructor
			GeneralEntityIterator(EntityIterator& other);
			
			// Post increment
			EntityIterator& operator++();
			// Pre increment
			EntityIterator& operator++(int junk);

			// Dereference
			GameEntity& operator*();
			// Functional dereference
			GameEntity* operator->();

			// Equality
			bool operator==(EntityIterator& rhs);
			// Inequality
			bool operator!=(EntityIterator& rhs);
		};

	private:
		// Member variables

		std::map<GameEntityType, std::list<GameEntity*> > mEntitiesByType;
		std::map<BehaviourType, std::list<GameEntity*> > mEntitiesByBehaviour;
		std::map<GameEntityId, GameEntity*> mEntitiesById;

	public:
		GameEntitySet(void);
		~GameEntitySet(void);

		void addGameEntity(GameEntity* entity);

		EntityIterator& begin(GameEntityType type = "");
		EntityIterator& end(GameEntityType type = "");
	};
}

#endif