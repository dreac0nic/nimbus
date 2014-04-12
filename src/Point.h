#ifndef VORONOI_POINT_H
#define VORONOI_POINT_H

namespace Nimbus
{
	namespace Voronoi
	{
		class Point
		{
		public:
			Point(int x, int y) : x(x), y(y) { }
			~Point() { }

			int x;
			int y;
		};
	}
}

#endif