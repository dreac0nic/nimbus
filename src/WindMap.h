#ifndef WIND_MAP_H
#define WIND_MAP_H

#include <list>
#include <OgreRoot.h>

namespace Nimbus
{
	/** Stores all information relating to wind movement
	 Contains an VectorRow
	 Contains an list of WindCurrents
	 */
	class WindMap
	{
	public:
		WindMap(int mapSizeX, int mapSizeY);
		virtual ~WindMap(void);

		// Gets Vector2 at position, will find average vector if given non-integer numbers
		Ogre::Vector2 getVector(double x, double y);

		// Gets Vector2 at Vector2, will find average vector if given non-integer numbers
		Ogre::Vector2 getVector(Ogre::Vector2 position);

		// Gets average Vector2 in a square area
		Ogre::Vector2 getAreaAverage(double topLeftx, double topLefty, double botRightx, double botRighty);
		Ogre::Vector2 getAreaAverage(Ogre::Vector2 topLeft, Ogre::Vector2 botRight);
	};
}
#endif