#ifndef NIMBUS_BEHAVIOUR_H
#define NIMBUS_BEHAVIOUR_H

#include <string>
#include <OgreConfigFile.h>

namespace Nimbus
{
	typedef std::string BehaviourType;
	/* THE ULTIMATE STUBBINESS */
	class Behaviour
	{
	protected:
		/* Behaviour type */
		BehaviourType mBehaviourType;

	public:
		Behaviour(void) {}
		~Behaviour(void) {}

		virtual void startup(void) = 0;
		virtual void update(void) = 0;
		virtual void shutdown(void) = 0;

		/* In form with the prototype pattern, this method creates a new instance of the behaviour. (Deep copy)

		Note: You are responsible for garbage collecting this behavior.
		*/
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings) = 0;
	};
}

#endif