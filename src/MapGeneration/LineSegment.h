#ifndef NIMBUS_LINESEGMENT_H
#define NIMBUS_LINESEGMENT_H

#include "Voronoi.h"

namespace Nimbus
{
	namespace Voronoi
	{
		class LineSegment
		{
		public:
			std::pair<Point*, Point*> points;

			LineSegment(Point *point0, Point *point1);
			LineSegment(std::pair <Point*, Point*> *points);
			~LineSegment();
			static double compareLengthsMax(LineSegment *segment0, LineSegment *segment1);
			static double compareLengths(LineSegment *edge0, LineSegment *edge1);
		};
	}
}

#endif