#ifndef VORONOI_POINT_H
#define VORONOI_POINT_H

#include "OgreRoot.h"

namespace Nimbus
{
	class Point
	{
	public:
		Point(int x, int y) : x(x), y(y) { point = Ogre::Vector3(Ogre::Real(x), 0.0F, Ogre::Real(y)); }
		~Point() { }

	private:
		Ogre::Vector3 point;

	public:
		int x;
		int y;

		Ogre::Vector3 getPoint(void) { return point; }
	};
}

#endif
