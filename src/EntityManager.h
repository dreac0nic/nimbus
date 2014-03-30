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

		/** The entity factory for the world.

		This is a bit nested, but I think it makes the most sense for the factory
		to be inside the entity manager class. If this is contrary to a previous
		decision, just move it back.
		*/
		EntityFactory* mEntityFactory;

		// The world object to access the entities from
		World* mWorld;

		// The player entity
		GameEntity* player;

	protected:
		// EventListeners

		// Creates entities on demand
		class CreateEntityListener : 
			public EventListener
		{
		private:
			// Reference to the factory that contains this listener
			EntityFactory* mFactory;
			World* mWorld;

		public:
			CreateEntityListener(EntityFactory* factory, World* world) : mFactory(factory), mWorld(world) {}
			virtual ~CreateEntityListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mCreateEntityListener;

		// Destroys entities on demand
		class DestroyEntityListener :
			public EventListener
		{
		private:
			World* mWorld;

		public:
			DestroyEntityListener(World* world) : mWorld(world) {}
			virtual ~DestroyEntityListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mDestroyEventListener;


		class PositionResponseListener :
			public EventListener
		{
		private:
			Ogre::Vector3 mPosition;
		public:
			PositionResponseListener() {}
			virtual ~PositionResponseListener() {}

			/** Gets the position from the query response. */
			Ogre::Vector3 getPosition() { return this->mPosition; }

			// From Nimbus::EventListener
			void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mPositionResponseListener;

	public:
		EntityManager(World* world);
		virtual ~EntityManager(void);

		// From Nimbus::Manager
		virtual void initialize(void);
		virtual bool update(void);
		virtual void pause(void);
		virtual void stop(void);

		/** Configures the entity factory.

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
