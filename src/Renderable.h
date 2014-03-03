#ifndef NIMBUS_BEHAVIOUR_RENDERABLE_H
#define NIMBUS_BEHAVIOUR_RENDERABLE_H

#include "Behaviour.h"

namespace Nimbus
{
	class Renderable:
		public Behaviour
	{
	public:
		Renderable(World *world);
		Renderable(World *world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		virtual ~Renderable(void);

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
	};
}

#endif