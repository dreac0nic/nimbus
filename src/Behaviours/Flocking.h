#ifndef NIMBUS_BEHAVIOUR_FLOCKING_H
#define NIMBUS_BEHAVIOUR_FLOCKING_H

#include "Flocking.h"

namespace Nimbus
{
	/* Flocking is a behaviour that implements a wind-based
		grouped Flocking behaviour. Inheriting much of its functionality from
		Flocking, the Flocking behaviour gives the entity the ablity to congragate
		with other entities and float in groups before being tore apart. Q.Q
	*/
	class Flocking:
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
		Flocking(BehaviourType type, World* world);

		/* Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
		*/
		Flocking(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Constructor taking a different behaviour and making a similar one.
			@param other The template Flocking behaviour.
			@param world A pointer to the game world.
			@param id The id for the parent entity.
		*/
		Flocking(Flocking* other, World* world, int id);

		/* Virtual destructor for destroying things. */
		virtual ~Flocking(void);

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