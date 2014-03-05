#ifndef NIMBUS_BLANKBEHAVIOUR_H
#define NIMBUS_BLANKBEHAVIOUR_H

#include "Behaviour.h"

namespace Nimbus
{
	class BlankBehaviour:
		public Behaviour
	{
	public:
		BlankBehaviour(World* world);
		BlankBehaviour(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		BlankBehaviour(BlankBehaviour* other, World* world);
		virtual ~BlankBehaviour(void);

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		virtual Behaviour* clone();
	};
}

#endif