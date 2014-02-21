#ifndef NIMBUS_GAMEMODE_H
#define NIMBUS_GAMEMODE_H

#include "RunMode.h"

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
	public:
		GameMode(void);
		virtual ~GameMode(void);

		// From Nimbus::RunMode
		virtual RunMode* run(const Ogre::FrameEvent& evt);
	};

}

#endif
