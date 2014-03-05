#ifndef NIMBUS_BEHAVIOUR_H
#define NIMBUS_BEHAVIOUR_H

#include <string>
#include <OgreConfigFile.h>

#include "World.h"

namespace Nimbus
{
	typedef std::string BehaviourType;

	/* THE ULTIMATE STUBBINESS */
	class Behaviour
	{
	protected:
		/* Handle to the world for Behavioural use. */
		World* mWorld;

		/* Behaviour type */
		BehaviourType mBehaviourType;

	public:
		Behaviour(World* world) { this->mWorld = world;}
		Behaviour(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings) { this->mWorld = world; } // Fix with delegating constructor, later
		Behaviour(Behaviour* other, World* world) {};
		virtual ~Behaviour(void) {}

		virtual void startup(void) = 0;
		virtual void update(void) = 0;
		virtual void shutdown(void) = 0;

		/* In form with the prototype pattern, this method creates a new instance of the behaviour. (Deep copy)
		Note: You are responsible for garbage collecting this behavior. */
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings) = 0;
		virtual Behaviour* clone() = 0;

		/* Return the entity type name. */
		BehaviourType getBehaviourType() { return this->mBehaviourType; }
	};
}

#endif