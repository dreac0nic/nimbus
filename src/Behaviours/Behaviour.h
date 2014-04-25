#ifndef NIMBUS_BEHAVIOUR_H
#define NIMBUS_BEHAVIOUR_H

#include <string>
#include <OgreConfigFile.h>

#include "../World.h"

namespace Nimbus
{
	typedef int GameEntityId;
	typedef std::string BehaviourType;

	class EventSystem;

	/** THE ULTIMATE STUBBINESS */
	class Behaviour
	{
	protected:
		/** Handle to the world for Behavioural use. */
		World* mWorld;

		/** The type of Behaviour */
		BehaviourType mBehaviourType;

		/** The entity Id that holds this behaviour. */
		GameEntityId mParentId;

		/** The event system for the entity. */
		EventSystem* mEntityEventSystem;

	public:
		Behaviour(BehaviourType type, World* world, EventSystem* eventSystem) :
			mWorld(world), mBehaviourType(type), mEntityEventSystem(eventSystem) {}
		Behaviour(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem) :
			mWorld(world), mBehaviourType(type), mEntityEventSystem(eventSystem) {}
		Behaviour(Behaviour* other, World* world, int id, EventSystem* eventSystem) :
			mWorld(world), mBehaviourType(other->mBehaviourType), mParentId(id), mEntityEventSystem(eventSystem) {}
		virtual ~Behaviour(void) {}

		virtual void startup(void) = 0;
		virtual void update(void) = 0;
		virtual void shutdown(void) = 0;

		/** In form with the prototype pattern, this method creates a new instance of the behaviour. (Deep copy)
		Note: You are responsible for garbage collecting this behavior. */
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem) = 0;
		virtual Behaviour* clone(int id, EventSystem* eventSystem) = 0;

		// Public accessors
		BehaviourType getBehaviourType() { return this->mBehaviourType; }
	};
}

#endif
