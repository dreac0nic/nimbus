#ifndef NIMBUS_ENTITYMANAGER_H
#define NIMBUS_ENTITYMANAGER_H

#include <string>

#include "Manager.h"
#include "EntityFactory.h"

namespace Nimbus
{
	/** Manages all the entities in the game world.
	 
	 Entities are all the game world elements. Particularly elements that can
	 be seen as elements that exist in the environment. We often stretch this
	 definition for convenience sake, particularly because entity provides a
	 generic framework from which we can build any game object we'd like.

	 This manager merely updates various entities as appropriate. Though it may
	 handle some aggregate updates that would be impossible for individual
	 entities to execute.
	 */
	class EntityManager :
		public Manager
	{
	private:
		// Member variables

		/* The entity factory for the world.

		This is a bit nested, but I think it makes the most sense for the factory
		to be inside the entity manager class. If this is contrary to a previous
		decision, just move it back.
		*/
		EntityFactory* mEntityFactory;

		// The world object to access the entities from
		World* mWorld;

	public:
		EntityManager(World* world);
		virtual ~EntityManager(void);

		// From Nimbus::Manager
		virtual bool update(void);

		/* Configures the entity factory.

		I'm trying something a bit different in order to take weight off the
		constructor. Theoretically this could lead to switching/updating
		the entity types midgame... might be cool.

		@param entityTypesFile The file location of the entity types definition
		file. This file contains a list of entity type names mapped to entity
		type definition files. These files are loaded by the EntityFactory to
		construct the prototypal entity types.
		@param world A reference to the world because sometimes, just sometimes,
		you need a reference to the world.
		*/
		void configureEntityTypes(std::string entityTypesFile, World* world);
	};

}

#endif
