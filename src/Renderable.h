#ifndef NIMBUS_BEHAVIOUR_RENDERABLE_H
#define NIMBUS_BEHAVIOUR_RENDERABLE_H

#include <OgreEntity.h>
#include "Behaviour.h"

namespace Nimbus
{
	class Renderable:
		public Behaviour
	{
	private:
		std::string mModelPath;
		Ogre::Entity* mModel;

	public:
		Renderable(World *world);
		Renderable(World *world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		Renderable(Renderable* other, World* world);
		virtual ~Renderable(void);

		// Accessor methods
		virtual Ogre::Entity* getEntity();

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		virtual Behaviour* clone();
	};
}

#endif