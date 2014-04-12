#ifndef VORONOI_POINT_H
#define VORONOI_POINT_H

#include "OgreRoot.h"

namespace Nimbus
{
	namespace Voronoi
	{
		class Point
		{
		public:
			Point(int x, int y) : x(x), y(y) { point = Ogre::Vector3(x, 0.0F, y); }
			~Point() { }

		private:
			Ogre::Vector3 point;

		public:
			int x;
			int y;

			Ogre::Vector3 getPoint(void) { return point; }
		};
	}
}

#endif