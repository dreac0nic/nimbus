#include <OgreRoot.h>
#include "VectorMap.h"

namespace Nimbus
{
	VectorMap::VectorMap(int xLength, int yLength)
	{
		map.resize(xLength * yLength);
		length = xLength;
	}

	VectorMap::~VectorMap(void)
	{
	}

	Ogre::Vector2 VectorMap::getVector(int posx, int posy)
	{
		return map.at(posx * length + posy);
	}

	void VectorMap::setVector(int posx, int posy, double strx, double stry)
	{
		map[length * length + posy] = Ogre::Vector2((Ogre::Real)strx, (Ogre::Real)stry);
	}
}