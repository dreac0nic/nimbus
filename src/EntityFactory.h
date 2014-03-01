#ifndef NIMBUS_ENTITYFACTORY_H
#define NIMBUS_ENTITYFACTORY_H

#include <iostream>
#include <fstream>

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
		std::map< std::string, GameEntity* > entityInstances;

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