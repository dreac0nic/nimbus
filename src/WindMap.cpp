#include <list>
#include <math.h>
#include <OgreRoot.h>
#include "VectorMap.h"
#include "WindCurrent.h"
#include "WindMap.h"

namespace Nimbus
{
	WindMap::WindMap(int mapSizeX, int mapSizeY)
	{
		sizeX = mapSizeX;
		sizeY = mapSizeY;
		vectorMap = new VectorMap(sizeX, sizeY);
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
		Ogre::Vector2 topLeft = vectorMap->getVector(flox, floy);
		Ogre::Vector2 botLeft = vectorMap->getVector(flox, 1 - floy);
		Ogre::Vector2 topRight = vectorMap->getVector(1 - flox, floy);
		Ogre::Vector2 botRight = vectorMap->getVector(1 - flox, 1 - floy);
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

	/*Ogre::Vector2 WindMap::getAreaAverage(double topLeftx, double topLefty, double botRightx, double botRighty)
	{

	}
	Ogre::Vector2 WindMap::getAreaAverage(Ogre::Vector2 topLeft, Ogre::Vector2 botRight)
	{
	return getAreaAverage(topLeft.x, topLeft.y, botRight.x, botRight.y);
	}*/

	void WindMap::setVector(double posx, double posy, double strx, double stry)
	{
		vectorMap->setVector(posx, posy, strx, stry);
	}

	void WindMap::setVector(Ogre::Vector2 position, Ogre::Vector2 strength)
	{
		setVector(position.x, position.y, strength.x, strength.y);
	}
}
