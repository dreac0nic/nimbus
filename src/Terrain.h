#ifndef TERRAIN_H
#define TERRAIN_H

#include <boost/polygon/voronoi.hpp>

namespace Nimbus
{
	class Terrain
	{
	public:
		Terrain();
		~Terrain();

	private:
		boost::polygon::voronoi_diagram<double> diagram;

	public:
		boost::polygon::voronoi_diagram<double>* getDiagram(void) { return &diagram; }
	};
}

#endif