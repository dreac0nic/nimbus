#ifndef NIMBUS_BEHAVIOUR_FLOCKSOARING_H
#define NIMBUS_BEHAVIOUR_FLOCKSOARING_H

#include "Soaring.h"

namespace Nimbus
{
	/* FlockSoaring is a behaviour that implements a wind-based
		grouped soaring behaviour. Inheriting much of its functionality from
		Soaring, the FlockSoaring behaviour gives the entity the ablity to congragate
		with other entities and float in groups before being tore apart. Q.Q
	*/
	class FlockSoaring:
		public Soaring
	{
	public:
		/* Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
		*/
		FlockSoaring(BehaviourType type, World* world);

		/* Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
		*/
		FlockSoaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Virtual destructor for destroying things. */
		virtual ~FlockSoaring(void);

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