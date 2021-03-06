#ifndef NIMBUS_GAMEENTITYSET_H
#define NIMBUS_GAMEENTITYSET_H

#include <iterator>
#include <list>
#include <map>

#include "GameEntity.h"

namespace Nimbus
{
	
	/** The GameEntitySet is a class for holding all the entities in the game
	world. Its main benefit and purpose is to provide custom iterators over
	entities so that users can select specific relevant sets of entities
	based on their component behaviours. */
	class GameEntitySet
	{
	public:
		// Public iterator generic type
		typedef std::iterator<std::input_iterator_tag, GameEntity> entityiterator;
	
	//private:
		// Private iterator implementations... umm I'd like to make this private
		
		/** An implementation of the entity iterator that iterates over all the entities
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
            GeneralEntityIterator(generaliterator entity, std::map<GameEntityId, GameEntity*>* entityMap);
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
		void removeGameEntity(GameEntityId id);

		GeneralEntityIterator beginGeneralIterator();
		GeneralEntityIterator endGeneralIterator();

		std::list<GameEntity*> getEntitiesOfType(std::string type);
		std::list<GameEntity*> getEntitiesWithBehaviour(std::string behaviour);
		GameEntity* getEntity(int id);
	};
}

#endif
