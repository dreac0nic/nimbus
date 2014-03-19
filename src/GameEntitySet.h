#ifndef NIMBUS_GAMEENTITYSET_H
#define NIMBUS_GAMEENTITYSET_H

#include <iterator>
#include <list>
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
		// Public iterator generic type
		typedef std::iterator<std::input_iterator_tag, GameEntity> entityiterator;
	
	//private:
		// Private iterator implementations... umm I'd like to make this private
		
		/* An implementation of the entity iterator that iterates over all the entities
		in the entity set.
		*/
		class GeneralEntityIterator :
			public entityiterator
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
			GeneralEntityIterator(const GeneralEntityIterator& other);
			
			// Post increment
			const GeneralEntityIterator& operator++();
			// Pre increment
			GeneralEntityIterator& operator++(int junk);

			// Dereference
			GameEntity& operator*();
			// Functional dereference
			GameEntity* operator->();

			// Equality
			bool operator==(const GeneralEntityIterator& rhs);
			// Inequality
			bool operator!=(const GeneralEntityIterator& rhs);
		};

		/** An implementation of the entity iterator which iterates over a
		specific type of entity (entityType)
		*/
		class TypeEntityIterator :
			public entityiterator
		{
		private:
			typedef std::list<GameEntity* >::iterator typeiterator;
			std::list<GameEntity* >* mEntities;

			// The current entity
			typeiterator mCurrentEntity;

		public:
			// Initial constructor to initialize the iterator to an arbitrary position
			TypeEntityIterator(typeiterator& entity, std::list<GameEntity* >* entityList);
			// Copy constructor
			TypeEntityIterator(const TypeEntityIterator& other);
			
			// Post increment
			const TypeEntityIterator& operator++();
			// Pre increment
			TypeEntityIterator& operator++(int junk);

			// Dereference
			GameEntity& operator*();
			// Functional dereference
			GameEntity* operator->();

			// Equality
			bool operator==(const TypeEntityIterator& rhs);
			// Inequality
			bool operator!=(const TypeEntityIterator& rhs);
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

		GeneralEntityIterator beginGeneralIterator();
		GeneralEntityIterator endGeneralIterator();

		TypeEntityIterator beginTypeIterator(GameEntityType type);
		TypeEntityIterator endTypeIterator(GameEntityType type);
	};
}

#endif