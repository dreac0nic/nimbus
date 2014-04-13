#ifndef TERRAIN_H
#define TERRAIN_H

#include <boost/polygon/voronoi.hpp>
#include "World.h"

namespace Nimbus
{
	class Terrain
	{
	public:
		Terrain(World* world);
		~Terrain();

	private:
		boost::polygon::voronoi_diagram<double> diagram;
		World* mWorld;

	public:
		/** Return the current diagram of tile cells.
			@return Return a voronoi_diagram listing all cells, edges, and points in the graph.
		*/
		boost::polygon::voronoi_diagram<double>* getDiagram(void) { return &diagram; }
	};
}

#endif