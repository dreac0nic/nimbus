#ifndef WIND_MAP_H
#define WIND_MAP_H

#include <list>
#include <OgreRoot.h>
#include "Grid.h"
#include "WindCurrent.h"
#include "EventSystem.h"
#include "GameEntity.h"

namespace Nimbus
{
	/** Stores all information relating to wind movement
	 Contains an VectorRow
	 Contains an list of WindCurrents
	 */
	class WindMap
	{
	private:
		/** The resolution of the vector map grid. */
		Ogre::Real mResolution;

		/** Arbitrary vector used to define the dimensions of a wind grid tile. */
		Ogre::Vector2 mAlphaVector;

		/** Factor determining how persistent our vector map should be between updates. */
		Ogre::Real mPersistenceFactor;

		/** The vector grid to represent wind strength and direction. */
		Grid<Ogre::Vector2>* mVectorMap;

		/** The list of wind currents which currently affect the wind map. */
		std::list<WindCurrent*> mCurrents;

		/** The minimum length this wind map accepts as valid for new wind currents. */
		Ogre::Real mMinimumCurrentLength;

		/** Offset to world center based on world size. */
		Ogre::Vector2 mOffset;

		/** A grid for storing the entity ids of arrows for showing the wind map. */
		Grid<GameEntityId>* mArrowGrid;

		/** Creates an arrow grid for the wind map.
		*/
		void createArrowGrid();

		/** Updates the arrow grid.
		*/
		void updateArrowGrid();

	protected:
		// Event listener

		/** Catches the id of a created arrow. */
		class ArrowCatcher :
			public EventListener
		{
		private:
			GameEntityId entityId;

		public:
			ArrowCatcher() : entityId(0) {}
			virtual ~ArrowCatcher() {}

			/** Gets the entity id returned from the event. */
			GameEntityId getEntityId() { return entityId; }

			// Form Nimbus::EventListener
			void handleEvent(payloadmap payload, EventListener* responder = NULL);
		}* mArrowCatcher;
		
	public:
		/** Creates a basic wind map.
			@param worldSize The size of the world in world distance.
				Right now assumes everything to be square... could change
			@param resolution The resolution of the vector map in terms of world coordinates.
				This value is the world distance between each wind vector in the vector map.
			@param offset The offset to the center of the map.
			@param minimumCurrentLength The minimum length of a current accepted by the map.
		*/
		WindMap(Ogre::Real worldSize, Ogre::Real resolution, Ogre::Vector2 offset, Ogre::Real minimumCurrentLength);
		virtual ~WindMap(void);

		/** Recalculates the vector map based on the current list of wind currents.
		*/
		void update(void);

		/** Adds a wind current to the wind current list.
			Any wind current shorter than mMinimumCurrentLength will not be added and
			an entry will be written to the log.
			@param windCurrent A pointer to the wind current which should be added to the wind map.
		*/
		void addWindCurrent(WindCurrent* windCurrent);

		/** Removes a wind current from the current wind current list
			@param windCurrent A pointer to the wind current which should be deleted from the wind map.
		*/
		void removeWindCurrent(WindCurrent* windCurrent);

		/** Gets the wind vector at the given map coordinates
			@param x The x coordinate in map coordinates of the wind vector to retrieve.
			@param y The y coordinate in map coordinates of the wind vector to retrieve.
		*/
		Ogre::Vector2 getWindVector(Ogre::Real x, Ogre::Real y);

		/** Gets the wind vector at the given map coordinates.
			@param position The x and y coordinates in map coordinates of the wind vector to retrieve.
		*/
		Ogre::Vector2 getWindVector(Ogre::Vector2 position);

		/** Gets the average wind vector in the given area of map coordinates.
			@param topLeftx The top left x coordinate in map coordinates
			@param topLefty The top left y coordinate in map coordinates
			@param botRightx The bottom right x coordinate in map coordinates
			@param botRighty The bottom right y coordinate in map coordinates
		*/
		Ogre::Vector2 getAverageWindVector(Ogre::Real topLeftx, Ogre::Real topLefty, Ogre::Real botRightx, Ogre::Real botRighty);

		/** Gets the average wind vector in the given area of map coordinates.
			@param topLeft The top left corner of the area in map coordinates
			@param botRight The bottom right corner of the area in map coordinates
		*/
		Ogre::Vector2 getAverageWindVector(Ogre::Vector2 topLeft, Ogre::Vector2 botRight);

		/** Gets the current list of wind currents.
		*/
		std::list<WindCurrent*>* getWindCurrents();

		/** Unexpectedly gets the alpha vector.
			@return A vector, that's the alpha vector. I bet you weren't expecting that...
		*/
		Ogre::Vector2 getAlphaVector() { return this->mAlphaVector; }

		/** Gets the offset of the wind map to the center of the wind map
			@return A Vector2 pointing to the center of the wind map
		*/
		Ogre::Vector2 getOffset() { return this->mOffset; }
	};
}
#endif
