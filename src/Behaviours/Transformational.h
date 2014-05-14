#ifndef NIMBUS_BEHAVIOUR_POSITIONAL_H
#define NIMBUS_BEHAVIOUR_POSITIONAL_H

#include <Ogre.h>

#include "Behaviour.h"

#include "../EventSystem/EventSystem.h"

namespace Nimbus
{
	/** Transformational is a class giving the entity the ability to have a position,
		rotation, scale, and velocity. While we're not so sure about that last one, that
		seems to make up a transformation... so that's why this is called Transformational.

		(More than meets the eye!!!)

		This tranformation is merely for virtual movement's sake, and does not directly
		effect the position that the entity is rendered at. This will be sent to the
		Renderable behaviour using an event-based messaging system.
	*/
	class Transformational:
		public Behaviour
	{
	private:
		/** Holds the position of the entity. */
		Ogre::Vector3 mPosition;

		/** Contains the rotations for the entity. */
		Ogre::Vector3 mRotationVector;

		/** Contains a vector which defines the scale of the entity. */
		Ogre::Vector3 mScale;

		/** Contains the facing vector for the entity. */
		Ogre::Vector3 mFacingVector;

		/** Contains the delta vector for the position vector. */
		Ogre::Vector3 mDeltaPosition;

		/** Contains the delta vector for the rotation vector. */
		Ogre::Vector3 mDeltaRotation;

		/** Contains the new vector for the facing vector.
			(Facing vectors are never relative to each other.
			That leads to scale issues and stuff.)
		*/
		Ogre::Vector3 mNewFacing;

		/** Determines if a force position update is necessary. */
		bool requireUpdate;

		/** Private constructor function referred to by all the actual constructors.
			Provides a standard method for creating an object given all the necessary parameters.
			@param initialPosition Pretty much like it sounds. It's a Vector3.
			@param facingDirection The direction the entity faces when it is created
			@param scale The initial scale vector
		*/
		void init(Ogre::Vector3 initialPosition, Ogre::Vector3 facingDirection, Ogre::Vector3 scale);

	protected:
		// Event Listeners

		/** Listener for translation events.
		*/
		class TranslationListener :
			public EventListener
		{
		private:
			Transformational* parent;
		public:
			TranslationListener(Transformational* parent) { this->parent = parent; }
			virtual ~TranslationListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
            virtual std::string str();
		}* mTranslationListener;

		class TranslationQueryListener :
			public EventListener
		{
		private:
			Transformational* mParent;
		public:
			TranslationQueryListener(Transformational* parent) : mParent(parent) {}
			virtual ~TranslationQueryListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
            virtual std::string str();
		}* mTranslationQueryListener;

	public:
		/** Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param eventSystem A pointer to the parent entity's event system.
		*/
		Transformational(BehaviourType type, World *world, EventSystem* eventSystem);

		/** Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
			@param eventSystem A pointer to the parent entity's event system.
		*/
		Transformational(BehaviourType type, World *world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem);

		/** Constructor taking a different behaviour and making a similar one.
			@param other The template Transformational behaviour.
			@param world A pointer to the game world.
			@param id The id for the parent entity.
			@param eventSystem A pointer to the parent entity's event system.
		*/
		Transformational(Transformational* other, World* world, int id, EventSystem* eventSystem);

		/** Virtual destructor for destroying things. */
		virtual ~Transformational(void);

		// From: Behaviour
		/** !! STUBBED !! Starts up the initial behaviour.*/
		virtual void startup(void);

		/** !! STUBBED !! Updates the behaviour, influencing the entity. */
		virtual void update(void);

		/** !! STUBBED !! Shuts down the behaviour, taking care of any major cleanup. */
		virtual void shutdown(void);

		/** Forces a position update. */
		virtual void forceUpdate(void);

		/** Duplicates the entity using the settings given.
			Currently merely returns a pointer given by the constructor.
		*/
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem);

		/** Duplicates the entity based on the current copy.
			This is used to enable use of the prototype pattern.
		*/
		virtual Behaviour* clone(int id, EventSystem* eventSystem);
	};
}

#endif
