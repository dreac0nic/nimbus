#ifndef NIMBUS_BEHAVIOUR_RENDERABLE_H
#define NIMBUS_BEHAVIOUR_RENDERABLE_H

#include <OgreEntity.h>
#include "Behaviour.h"

namespace Nimbus
{
	class Renderable:
		public Behaviour
	{
	private:
		Ogre::Entity* mModel;
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mScale;
		Ogre::Vector3 mRotation;
		Ogre::SceneNode* mNode;

		/* Constructor function that initializes all the necessary properties of the
			Renderable behaviour. Called by all the constructors with the appropriate
			parameters.
		*/
		void init(Ogre::Entity* model, Ogre::Vector3 scale);

	protected:
		// Event Listeners

		/* An event listener for catching position updates from the positional behaviour.
		*/
		class PositionListener :
			public EventListener
		{
		private:
			Renderable* parent;
		public:
			PositionListener(Renderable* parent) { this->parent = parent; }
			~PositionListener() {}

			void handleEvent(payloadmap payload);
		}* mPositionListener;

	public:
		Renderable(BehaviourType type, World *world);
		Renderable(BehaviourType type, World *world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		Renderable(Renderable* other, World* world, int id);
		virtual ~Renderable(void);

		// Accessor methods
		virtual Ogre::Entity* getEntity();
		virtual Ogre::Vector3 getPosition();
		virtual Ogre::Vector3 getScale();
		virtual Ogre::Vector3 getRotation();

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings);
		virtual Behaviour* clone(int id);
	};
}

#endif