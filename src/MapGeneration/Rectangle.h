#ifndef NIMBUS_RECTANGLE_H
#define NIMBUS_RECTANGLE_H

#include "Voronoi.h"

/**	Helper class for bounding box.
	Capable of checking whether a point is inside a box.
*/
namespace Nimbus
{
	namespace Voronoi
	{
		class Rectangle
		{
		public:
			double x, y, width, height, right, bottom, left, top;
			Rectangle();
			Rectangle(double x, double y, double width, double height);
			bool liesOnAxes(Point *p);
			bool inBounds(Point *p);
			bool inBounds(double x0, double y0);
		};
	}
}

#endif