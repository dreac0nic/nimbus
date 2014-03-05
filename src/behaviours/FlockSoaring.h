#ifndef NIMBUS_BEHAVIOUR_FLOCKSOARING_H
#define NIMBUS_BEHAVIOUR_FLOCKSOARING_H

#include "Soaring.h"

namespace Nimbus
{
	class FlockSoaring:
		public Soaring
	{
	public:
		FlockSoaring(World* world);
		FlockSoaring(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		virtual ~FlockSoaring(void);

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
	};
}

#endif