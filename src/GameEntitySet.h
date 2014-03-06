#ifndef NIMBUS_GAMEENTITYSET_H
#define NIMBUS_GAMEENTITYSET_H

#include <iterator>
#include <map>

#include "GameEntity.h"

namespace Nimbus
{
	
	/* THE ULTIMATE STUBBINESS */
	class GameEntitySet
	{
	public:
		// Public iterator generic type
		typedef std::iterator<std::input_iterator_tag, GameEntity> entityiterator;
	
	private:
		// Private iterator implementations
		
		/* An implementation of the entity iterator that iterates over all the entities
		in the entity set.
		*/
		class GeneralEntityIterator :
			public entityiterator
		{
		private:
			// The current entity
			std::map<GameEntityId, GameEntity*>::iterator currentEntity;

		public:
			// Umm... not sure why we need this
			GeneralEntityIterator(GameEntity* entity);
			// Copy constructor
			GeneralEntityIterator(const GeneralEntityIterator& other);
			
			// Post increment
			GeneralEntityIterator& operator++();
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

		std::map<GameEntityType, std::vector<GameEntity* > > mEntitiesByType;
		std::map<GameEntityId, GameEntity*> mEntitiesById;

	public:
		GameEntitySet(void);
		~GameEntitySet(void);

		void addGameEntity(GameEntity* entity);

		entityiterator getIterator(GameEntityType type);
	};
}

#endif