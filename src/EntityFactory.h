#ifndef NIMBUS_ENTITYFACTORY_H
#define NIMBUS_ENTITYFACTORY_H

#include <string>
#include <vector>
#include <map>

#include "GameEntity.h"
#include "./Behaviours/Behaviour.h"
#include "EventSystem/EventSystem.h"

namespace Nimbus
{
	class EntityFactory
	{
	private:
		/** Simple map that maps the entity type name read in from the
		 definition file to the list of behaviours specified in definition
		 file.
		*/
		std::map< GameEntityType, GameEntity* > mEntityInstances;

		/** Simple map that maps the behaviour type name defined by
		 programmers... Attempts to kind of offset hard-coding...
		 but at some level, I don't really see a way around the hard coding.
		 Maybe in some universe, there is a perfect factory system that
		 doesn't require hard coding. Maybe it's written in LISP...
		*/
		std::map< std::string, Behaviour* > mBehaviourInstances;

		/** The world that needs to be passed to behaviours. */
		World* mWorld;

	public:
		EntityFactory(World* world, std::string entityDefinitionFile);
		~EntityFactory(void);

		// Object Methods --
		/** CreateEntity() creates a given type of entity and returns a reference
		 to the created entity.

		 Note: You are responsible for garbage collecting this new entity.
		*/
		GameEntity* createEntity(std::string entityType);
	};
}

#endif