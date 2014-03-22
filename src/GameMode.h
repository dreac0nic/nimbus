#ifndef NIMBUS_GAMEMODE_H
#define NIMBUS_GAMEMODE_H

#include "RunMode.h"
#include "EnvironmentManager.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "World.h"

namespace Nimbus
{
	/** RunMode that drives the game.

	 This RunMode contains the world object which contains all information about
	 the game world (the environment and the entities). This RunMode is also
	 responsible for the managers which run the game.
	 */
	class GameMode :
		public RunMode
	{
	private:
		// Member variables

		// The Environment Manager
		EnvironmentManager* mEnvironmentMan;

		// The Entity Manager
		EntityManager* mEntityMan;

		// The game World
		World* mWorld;

		// Ogre variables
		Ogre::SceneManager* mSceneMgr;
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;

	protected:
		// From Nimbus::RunMode
		virtual bool initialize();

	public:
		GameMode(void);
		virtual ~GameMode(void);

		// From Nimbus::RunMode
		virtual RunMode* run(const Ogre::FrameEvent& evt);
	};

}

#endif
