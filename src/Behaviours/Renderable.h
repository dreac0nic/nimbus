#ifndef NIMBUS_BEHAVIOUR_RENDERABLE_H
#define NIMBUS_BEHAVIOUR_RENDERABLE_H

#include <OgreEntity.h>
#include "Behaviour.h"
#include "../EventSystem.h"

namespace Nimbus
{
	class Renderable:
		public Behaviour
	{
	private:
		std::string mBaseEntityName;
		Ogre::Entity* mOgreEntity;
		//Ogre::Vector3 mPosition;
		Ogre::Vector3 mScale;
		//Ogre::Vector3 mRotation;
		Ogre::SceneNode* mNode;

		/** Constructor function that initializes all the necessary properties of the
			Renderable behaviour. Called by all the constructors with the appropriate
			parameters.
		*/
		void init(Ogre::Entity* ogreEntity, Ogre::Vector3 scale, std::string entityBaseName);

	protected:
		// Event Listeners

		/** An event listener for catching position updates from the positional behaviour.
		*/
		class PositionListener :
			public EventListener
		{
		private:
			Renderable* parent;
		public:
			PositionListener(Renderable* parent) { this->parent = parent; }
			~PositionListener() {}

			void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mPositionListener;

	public:
		Renderable(BehaviourType type, World *world, EventSystem* eventSystem);
		Renderable(BehaviourType type, World *world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem);
		Renderable(Renderable* other, World* world, int id, EventSystem* eventSystem);
		virtual ~Renderable(void);

		// Accessor methods
		virtual Ogre::Entity* getModel();
		virtual Ogre::Vector3 getScale();

		// Setter methods
		virtual void setModel(Ogre::Entity* model);

		// From: Behaviour
		virtual void startup(void);
		virtual void update(void);
		virtual void shutdown(void);
		virtual Behaviour* clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem);
		virtual Behaviour* clone(int id, EventSystem* eventSystem);
	};
}

#endif