#ifndef NIMBUS_BEHAVIOUR_SOARING_H
#define NIMBUS_BEHAVIOUR_SOARING_H

#include "Positional.h"

namespace Nimbus
{
	/* Soaring is a behaviour based on the Positional behaviour.
		This behaviour will cause the entity to float over the wind
		currents currently calculated in the vector field. All the
		effected vectors will be averaged into one single vector used
		to guide the entity's motion.
	*/
	class Soaring:
		public Behaviour
	{
	private:

		/* Called by the constructors to initialize the behaviour. Anything that needs to
			be duplicated among constructors should be put in here.
		*/
		void init();

	public:
		/* Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
		*/
		Soaring(BehaviourType type, World* world);

		/* Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
		*/
		Soaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Constructor based on another Soaring behaviour.
			@param other A pointer to the other Soaring behaviour.
			@param world A pointer to the game world.
			@param id The id of the parent entity holding this behaviour.
		*/
		Soaring(Soaring* other, World* world, int id);

		/* Virtual destructor for destroying things. */
		virtual ~Soaring(void);

		// From: Behaviour
		/* !! STUBBED !! Starts up the initial behaviour.*/
		virtual void startup(void);

		/* !! STUBBED !! Updates the behaviour, influencing the entity. */
		virtual void update(void);

		/* !! STUBBED !! Shuts down the behaviour, taking care of any major cleanup. */
		virtual void shutdown(void);

		/* Duplicates the entity using the settings given.
			Currently merely returns a pointer given by the constructor.
		*/
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Duplicates the entity based on the current copy.
			This is used to enable use of the prototype pattern.
		*/
		virtual Behaviour* clone(int id);
	};
}

#endif