#ifndef WIND_MAP_H
#define WIND_MAP_H

#include <list>
#include <OgreRoot.h>
#include "VectorMap.h"
#include "WindCurrent.h"

namespace Nimbus
{
	/** Stores all information relating to wind movement
	 Contains an VectorRow
	 Contains an list of WindCurrents
	 */
	class WindMap
	{
	private:
		// Stores two dimensional array of Vector2s
		VectorMap* vectorMap;
		
	public:
		WindMap(int mapSizeX, int mapSizeY);
		virtual ~WindMap(void);

		// Stores list of WindCurrents
		std::list<WindCurrent> currents;

		// Stores size of vectorMap
		int sizeX;
		int sizeY;

		// Gets Vector2 at position, will find average vector if given non-integer numbers
		Ogre::Vector2 getVector(double x, double y);

		// Gets Vector2 at Vector2, will find average vector if given non-integer numbers
		Ogre::Vector2 getVector(Ogre::Vector2 position);

		// Gets average Vector2 in a square area
		Ogre::Vector2 getAreaAverage(double topLeftx, double topLefty, double botRightx, double botRighty);
		Ogre::Vector2 getAreaAverage(Ogre::Vector2 topLeft, Ogre::Vector2 botRight);

		// Sets Vector2 at position to strength
		void setVector(double posx, double posy, double strx, double stry);

		// Sets Vector2 at Vector2 to vector
		void setVector(Ogre::Vector2 position, Ogre::Vector2 strength);
	};
}
#endif
