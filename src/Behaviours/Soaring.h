#ifndef NIMBUS_BEHAVIOUR_SOARING_H
#define NIMBUS_BEHAVIOUR_SOARING_H

#include "Behaviour.h"

namespace Nimbus
{
	/** Soaring allows the entity to be sensitive to wind data. It uses the SOAR_ENTITY event
		to alert the entity which direction the wind is pushing the entity.
	*/
	class Soaring:
		public Behaviour
	{
	private:

		/** Called by the constructors to initialize the behaviour. Anything that needs to
			be duplicated among constructors should be put in here.
		*/
		void init();

	public:
		/** Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param eventSystem The event system that localized events will be passed to.
		*/
		Soaring(BehaviourType type, World* world, EventSystem* eventSystem);

		/** Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
			@param eventSystem The event system that localized events will be passed to.
		*/
		Soaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem);

		/** Constructor based on another Soaring behaviour.
			@param other A pointer to the other Soaring behaviour.
			@param world A pointer to the game world.
			@param id The id of the parent entity holding this behaviour.
			@param eventSystem The event system that localized events will be passed to.
		*/
		Soaring(Soaring* other, World* world, int id, EventSystem* eventSystem);

		/** Virtual destructor for destroying things. */
		virtual ~Soaring(void);

		// From: Behaviour
		/** !! STUBBED !! Starts up the initial behaviour.*/
		virtual void startup(void);

		/** Sends off a SOAR_ENTITY event to the entity's flocking group. */
		virtual void update(void);

		/** !! STUBBED !! Shuts down the behaviour, taking care of any major cleanup. */
		virtual void shutdown(void);

		/** Duplicates the entity using the settings given.
			@return A pointer given by the constructor.
		*/
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem);

		/** Duplicates the entity based on the current copy.
			This is used to enable use of the prototype pattern.
		*/
		virtual Behaviour* clone(int id, EventSystem* eventSystem);
	};
}

#endif
