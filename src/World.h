#ifndef NIMBUS_WORLD_H
#define NIMBUS_WORLD_H

#include <vector>
#include <OgreSceneNode.h>
#include "WindMap.h"

namespace Nimbus
{
	class GameEntity;
	class GameEntitySet;

	/** This class aggregates the entire world of Nimbus.
	 
	 All the elements that make up the world should be placed in this class.
	 Classes in this structure are less concerned with making things happen
	 so much as keeping game relevant statistics available.

	 Concerning the design of this class, we have yet to decide how specific
	 modules will reference the data contained in the world. If we decide to
	 give general access to the world, then we will create accessors for different
	 parts of the game world in this class.

	 Additionally, the world might be a convenient, uniform way for managers to
	 access the game world. This is important for cleanliness and efficiency.

	 Otherwise, this class is mostly just a convenient way to aggregate all the
	 data about the game world so that we can easily save and load it. (Concerning
	 serialization, we still need to implement that.)

	 So basically, this class has potential, but we need to figure out exactly how
	 it will work with respect to various modules (particularly entities). For the
	 moment, don't be afraid to add to it. That will help us know what you need to
	 be here as we design this. At least, for whatever module your developing, make
	 member variable for the root of the system so that the entire game world is
	 aggregated by this class.
	 */

	class World
	{
	private:
		// The counter for each thing that we add to the world
		int idCounter;

		// Bounds of World
		Ogre::Vector2 worldBounds;

		//Temporary worldBounds constant
		int size;

		// The list that keeps track of all entities currently in the world
		GameEntitySet* mEntities;

		// The scene node that contains everything
		Ogre::SceneNode* mWorldNode;
		Ogre::SceneManager* mSceneManager;

	public:
		World(Ogre::SceneManager* sceneManager);
		virtual ~World(void);

		WindMap windMap;

		// Accessor methods
		int getCurrentId();
		GameEntitySet* getEntities() { return this->mEntities; }
		void addEntity(GameEntity* entity);
		Ogre::SceneNode* getWorldNode() { return this->mWorldNode; }
		Ogre::SceneManager* getSceneManager() { return this->mSceneManager; }
	};

}

#endif
