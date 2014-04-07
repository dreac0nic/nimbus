#ifndef NIMBUS_POLYGON_H
#define NIMBUS_POLYGON_H

#include <vector>

#include "Voronoi.h"

namespace Nimbus
{
	namespace Voronoi
	{
		class Polygon
		{
		private:
			double signedDoubleArea();
			std::vector<Point*> _vertices;

		public:
			Polygon(std::vector<Point*> *vertices);
			~Polygon();
			Winding winding();
		};
	}
}

#endif