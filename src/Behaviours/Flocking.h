#ifndef NIMBUS_BEHAVIOUR_FLOCKING_H
#define NIMBUS_BEHAVIOUR_FLOCKING_H

#include "Behaviour.h"
#include "../EventSystem.h"
#include "../GameEntity.h"

namespace Nimbus
{
	/** Flocking is a behaviour that implements 
		grouped Flocking behaviour. The Flocking behaviour gives the entity
		the ablity to congregate with other entities and float in groups
		before being tore apart. Q.Q
	*/
	class Flocking:
		public Behaviour
	{
	private:
		// Arbitrary constants (which aren't technically constants)

		/** The influence factor of each component cloud (divided by the number of
			components on use)
		*/
		double mComponentInfluenceFactor;

		/** The factor deciding how much the individual movement of the cloud should
			override the group direction.
		*/
		double mComponentOverrideFactor;

		// Member variables

		// Delta vector aggregated from component entities
		Ogre::Vector3 mPositionDelta;

		// The list of component entities
		std::map<GameEntityId, Ogre::Vector3> mEntities;

		/** Called by the constructors to initialize the behaviour. Anything that needs to
			be duplicated among constructors should be put in here.
		*/
		void init(double influenceFactor, double overrideFactor);

	protected:
		/** Updates the direction of the flocking group based on the wind current from a single
			cloud.
		*/
		class SoarListener :
			public EventListener
		{
		private:
			Flocking* mParent;

		public:
			SoarListener(Flocking* parent) : mParent(parent) {}
			~SoarListener() {}

			// From Nimbus::EventListener
			void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mSoarListener;

	public:
		/** Default constructor, taking a world pointer.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
		*/
		Flocking(BehaviourType type, World* world);

		/** Constructor based of a set of initial settings.
			@param type The type of Behaviour being constructed.
			@param world A pointer to the game world.
			@param intitializingSettings A map of settings used to construct the intial entity.
		*/
		Flocking(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/** Constructor taking a different behaviour and making a similar one.
			@param other The template Flocking behaviour.
			@param world A pointer to the game world.
			@param id The id for the parent entity.
		*/
		Flocking(Flocking* other, World* world, int id);

		/** Virtual destructor for destroying things. */
		virtual ~Flocking(void);

		// From: Behaviour
		/** !! STUBBED !! Starts up the initial behaviour.*/
		virtual void startup(void);

		/** !! STUBBED !! Updates the behaviour, influencing the entity. */
		virtual void update(void);

		/** !! STUBBED !! Shuts down the behaviour, taking care of any major cleanup. */
		virtual void shutdown(void);

		/** Duplicates the entity using the settings given.
			Currently merely returns a pointer given by the constructor.
		*/
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);

		/** Duplicates the entity based on the current copy.
			This is used to enable use of the prototype pattern.
		*/
		virtual Behaviour* clone(int id);
	};
}

#endif