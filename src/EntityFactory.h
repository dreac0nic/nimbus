#ifndef NIMBUS_ENTITYFACTORY_H
#define NIMBUS_ENTITYFACTORY_H

#include <string>
#include <vector>
#include <map>

#include "GameEntity.h"
#include "Behaviour.h"

namespace Nimbus
{
	class EntityFactory
	{
	private:
		/* Simple map that maps the entity type name read in from the
		 definition file to the list of behaviours specified in definition
		 file.
		*/
		std::map< std::string, GameEntity* > mEntityInstances;

		/* Simple map that maps the behaviour type name defined by
		 programmers... Attempts to kind of offset hard-coding...
		 but at some level, I don't really see a way around the hard coding.
		 Maybe in some universe, there is a perfect factory system that
		 doesn't require hard coding. Maybe it's written in LISP...
		*/
		std::map< std::string, Behaviour* > mBehaviourInstances;

	public:
		EntityFactory(std::string entityDefinitionFile);
		~EntityFactory(void);

		// Object Methods --
		/* CreateEntity() creates a given type of entity and returns a reference
		 to the created entity.
		*/
		GameEntity* createEntity(std::string entityType);
	};
}

#endif