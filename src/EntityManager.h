#ifndef NIMBUS_ENTITYMANAGER_H
#define NIMBUS_ENTITYMANAGER_H

#include "manager.h"

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
	public:
		EntityManager(void);
		virtual ~EntityManager(void);

		// From Nimbus::Manager
		virtual bool update(void);
	};

}

#endif
