#ifndef NIMBUS_ENVIRONMENTMANAGER_H
#define NIMBUS_ENVIRONMENTMANAGER_H

#include <OgreSceneManager.h>

#include "Manager.h"
#include "World.h"
#include "WindManager.h"

namespace Nimbus
{
	/** Manages all elements of the game world environment.

	 The environment consists of all properties of the game world which affect
	 the entities which exist dependent on the conditions of world.
	 */
	class EnvironmentManager :
		public Manager
	{
	private:
		// The Wind manager
		WindManager* mWindManager;
		World* mWorld;

	public:
		EnvironmentManager(Ogre::SceneManager* sceneManager);
		virtual ~EnvironmentManager(void);

		// From Nimbus::Manager
		virtual bool update(void);
	};

}

#endif
