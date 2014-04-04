#ifndef VECTOR_MAP_H
#define VECTOR_MAP_H

#include <OgreRoot.h>
#include <vector>
#include "EventSystem.h"
#include "Grid.h"
#include "GameEntity.h"

namespace Nimbus
{
	//int VECTORMAPTOWORLDRATIO = 250;

	/** Stores an array of 2D-Vectors
	 Used by WindMap
	 */
	class VectorMap
	{
	private:
		// Stores Vector2s in vector
		std::vector<Ogre::Vector2> map;

		Grid<GameEntityId>* arrows;

		// Stores size of one dimension of map
		int length;

		// Stores ambient wind strength for initial creation
		//const double averageStr = 1;

	protected:
		// Event Listeners

		/** Responder for catching created entities.

			Designed solely as a responder, not a listener.
		*/
		class CatchEntityListener :
			public EventListener
		{
		private:
			GameEntityId mEntityId;

		public:
			CatchEntityListener() {}
			virtual ~CatchEntityListener() {}

			/** Gets the associated entity id
				@return The contained game entity id
			*/
			GameEntityId getEntityId() { return this->mEntityId; }

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mCatchEntityListener;

	public:
		VectorMap(int xLength, int yLength);
		virtual ~VectorMap(void);

		// Gets Vector2 from location (x, y)
		Ogre::Vector2 getVector(int posx, int posy);

		// Sets Vector2 to strength (x, y) at position (x, y)
		void setVector(int posx, int posy, double strx, double stry);

		void updateArrows();

		GameEntityId getArrowId(int posx, int posy);
	};
}
#endif
