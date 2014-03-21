#ifndef NIMBUS_BEHAVIOUR_POSITIONAL_H
#define NIMBUS_BEHAVIOUR_POSITIONAL_H

#include <Ogre.h>

#include "Behaviour.h"

namespace Nimbus
{
	/* Positional is a class giving the entity the ability to have a position.
		This position is merely for virtual movement's sake, and does not directly
		effect the position that the entity is rendered at. This will be sent to the
		Renderable behaviour using an event-based messaging system.
	*/
	class Positional:
		public Behaviour
	{
	private:
		/* Holds the position of the entity. */
		Ogre::Vector3 mPosition;

		/* Contains the displaced vector for the last frame. */
		Ogre::Vector3 mDisplacementVector;

	public:
		/* Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
		*/
		Positional(BehaviourType type, World *world);

		/* Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
		*/
		Positional(BehaviourType type, World *world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/* Constructor taking a different behaviour and making a similar one.
			@param other The template Positional behaviour.
			@param world A pointer to the game world.
			@param id The id for the parent entity.
		*/
		Positional(Positional* other, World* world, int id);

		/* Virtual destructor for destroying things. */
		virtual ~Positional(void);

		// From: Behaviour
		/* !! STUBBED !! Starts up the initial behaviour.*/
		virtual void startup(void);

		/* !! STUBBED !! Updates the behaviour, influencing the entity. */
		virtual void update(void);

		/* !! STUBBED !! Shuts down the behaviour, taking care of any major cleanup. */
		virtual void shutdown(void);

		/* Forces a position update. */
		virtual void forceUpdate(void);

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