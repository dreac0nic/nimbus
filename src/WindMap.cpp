#include <list>
#include <math.h>
#include <OgreRoot.h>
#include "WindCurrent.h"
#include "WindMap.h"

namespace Nimbus
{
	WindMap::WindMap(int mapSizeX, int mapSizeY)
	{
		vectorMap = new Grid<Ogre::Vector2>(mapSizeX, mapSizeY);

		// Initialize the vector map
		for(int x = 0; x < vectorMap->getXDimension(); ++x)
		{
			for(int y = 0; y < vectorMap->getYDimension(); ++y)
			{
				vectorMap->set(x, y, Ogre::Vector2::UNIT_X);
			}
		}
	}
	WindMap::~WindMap(void)
	{
	}

	Ogre::Vector2 WindMap::getVector(double posx, double posy)
	{
		double resultX;
		double resultY;
		int flox = (int)floor(posx);
		int floy = (int)floor(posy);
		Ogre::Vector2 topLeft = vectorMap->get(flox, floy);
		Ogre::Vector2 botLeft = vectorMap->get(flox, 1 - floy);
		Ogre::Vector2 topRight = vectorMap->get(flox, 1- floy);
		Ogre::Vector2 botRight = vectorMap->get(flox, 1-floy);
		double left = flox - posx + 1;
		double top = floy - posy + 1;
		resultX = left * top * topLeft.x + left * (1-top) * botLeft.x + (1-left) * top * topRight.x + (1-left) * (1-top) * botRight.x;
		resultY = left * top * topLeft.y + left * (1-top) * botLeft.y + (1-left) * top * topRight.y + (1-left) * (1-top) * botRight.y;
		
		return Ogre::Vector2((Ogre::Real)resultX, (Ogre::Real)resultY);
	}

	Ogre::Vector2 WindMap::getVector(Ogre::Vector2 position)
	{
		return getVector(position.x, position.y);
	}

	Ogre::Vector2 WindMap::getAreaAverage(double topLeftx, double topLefty, double botRightx, double botRighty)
	{
		// Placeholder functionality... replace asap
		return getVector(topLeftx, topLefty);
	}
	Ogre::Vector2 WindMap::getAreaAverage(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
	{
		return getAreaAverage(topLeft.x, topLeft.y, botRight.x, botRight.y);
	}
}
