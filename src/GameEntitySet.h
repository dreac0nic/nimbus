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

			// The current entity
			generaliterator currentEntity;

		public:
			// Umm... not sure why we need this
			GeneralEntityIterator(generaliterator& entity);
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

	private:
		// Member variables

		std::map<GameEntityType, std::list<GameEntity*> > mEntitiesByType;
		std::map<BehaviourType, std::list<GameEntity*> > mEntitiesByBehaviour;
		std::map<GameEntityId, GameEntity*> mEntitiesById;

	public:
		GameEntitySet(void);
		~GameEntitySet(void);

		void addGameEntity(GameEntity* entity);

		GeneralEntityIterator begin(GameEntityType type = "");
		GeneralEntityIterator end(GameEntityType type = "");
	};
}

#endif