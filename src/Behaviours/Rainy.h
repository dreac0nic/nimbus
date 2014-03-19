#ifndef NIMBUS_BEHAVIOUR_RAINY_H
#define NIMBUS_BEHAVIOUR_RAINY_H

#include "Behaviour.h"

namespace Nimbus
{
	/* Rainy is a behaviour that will cause the applied
		to an entity that will cause the entity to rain down
		on the tiles below it.
	*/
	class Rainy:
		public Behaviour
	{
	public:
		/* Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
		*/
		Rainy(BehaviourType type, World* world);

		/* Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
		*/
		Rainy(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Virtual destructor for destroying things. */
		virtual ~Rainy(void);

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
	};
}

#endif