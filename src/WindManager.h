#ifndef NIMBUS_WINDMANAGER_H
#define NIMBUS_WINDMANAGER_H

#include "EventSystem.h"
#include "Manager.h"
#include "WindMap.h"
#include "World.h"

namespace Nimbus
{

	/** Takes input and Updates WindMap
	*/
	class WindManager :
		public Manager
	{
	private:
		// Member variables

		/** A reference to the game world... it's useful sometimes. */
		World* mWorld;

		/** The Ogre scene manager. */
		Ogre::SceneManager* mSceneManager;

		/** The wind plane used for collision with the mouse rays. */
		Ogre::Plane mWindPlane;

		/** The last position stored for creating a wind current. */
		Ogre::Vector2* mCurrentPosition;

		/** The wind current being constructed. */
		WindCurrent* mWindCurrent;
		
		/// Setting up the plane that will register the clicks for the wind creation
		virtual void createClickPlane();

		/** Adds a new position to the wind current being built.
			@param newPosition The next point to add to our wind current
				(aka the location of the mouse)
		*/
		void addPoint(Ogre::Vector2& newPosition);

		/** Subdivides the given wind current vector into a list of alpha length (or less) separated
			vectors representing delta subarrays' positions. This is used so that we hit all wind
			vectors along the current path in the grid map.
			@param current The wind current vector to subdivide.
		*/
		std::list<Ogre::Vector2> subdivideCurrent(Ogre::Vector2 current);

		/** Generates a new random current with a random number of points 
			along a random path at a random starting position.
		*/
		void generateCurrent(void);

		/** Generates a new random current with a given number of points
			along a random path at a random starting position.
			@param numVectors The number of vectors to generate for this current
		*/
		void generateCurrent(int numVectors);

		/** Generates a new random current with a given number of points
			along a random path at a given starting position.
			@param numVectors The number of vectors to generate for this current
			@param startingPosition The origin point to start this current at
		*/
		void generateCurrent(int numVectors, Ogre::Vector2 startingPosition);

		/** Get the position of the collision of a mouse click ray
			across our wind plane
			@param collisionRay A pointer of the ray click to collide our plane with
		*/
		Ogre::Vector2 WindManager::getCollisionPoint(Ogre::Ray* collisionRay);

	protected:
		// Event Listeners

		// ********************************************************************
		/// Listens for the wind update events
		class MouseWindUpdateListener : 
			public EventListener
		{
		private:
			WindManager* mParent;

		public:
			MouseWindUpdateListener(WindManager* parent)
				{ this->mParent = parent; }
			virtual ~MouseWindUpdateListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseWindUpdateListener;

		// ********************************************************************
		/// Listens for the wind current creation events
		class MouseWindStartListener : 
			public EventListener
		{
		private:
			WindManager* mParent;

		public:
			MouseWindStartListener(WindManager* containingManager)
				{ this->mParent = containingManager; }
			virtual ~MouseWindStartListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseWindStartListener;

		// ********************************************************************
		/// Listens for the wind current completion events
		class MouseWindEndListener : 
			public EventListener
		{
		private:
			WindManager* mParent;

		public:
			MouseWindEndListener(WindManager* containingManager)
				{ this->mParent = containingManager; }
			virtual ~MouseWindEndListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mMouseWindEndListener;

		// ********************************************************************
		/** Used for things like periodic updates, namely super expensive ones like updating the arrows
		*/
		class TickListener :
			public EventListener
		{
		private:
			WindManager* mParent;

		public:
			TickListener(WindManager* parent) : mParent(parent) {}
			virtual ~TickListener() {}

			// From Nimbus::EventListener
			void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mTickListener;

	public:
		WindManager(Ogre::SceneManager* sceneManager, World* world);
		virtual ~WindManager(void);

		// From Nimbus::Manager
		virtual void initialize(void) {}
		virtual bool update(void);
	};
}

#endif
