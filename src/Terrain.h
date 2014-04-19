#ifndef NIMBUS_TERRAIN_H
#define NIMBUS_TERRAIN_H

#include <OGRE/OgreMesh.h>
#include <boost/polygon/voronoi.hpp>

#include "Point.h"

namespace Nimbus
{
	class World;

	class Terrain
	{
	public:
		Terrain(World* world);
		~Terrain(void);

	private:
		boost::polygon::voronoi_diagram<double> diagram;
		std::vector<Point> pointList;
		World* mWorld;

	public:
		/** Return the current diagram of tile cells.
			@return Return a pointer to a voronoi_diagram with all cells, edges, and points.
		*/
		boost::polygon::voronoi_diagram<double>* getDiagram(void) { return &diagram; }

		/** Return the list of points used to generate cells - these are the centerpoints.
			@return A pointer to a vector of Voronoi::Point
		*/
		std::vector<Point>* getPointList(void) { return &pointList; }

		/** Returns a MeshPtr to a generated mesh resembling the terrain.
			@param meshName The name to give this mesh object.
			@return A MeshPtr holding the generated mesh.
		*/
		MeshPtr getMesh(std::string meshName);
	};
}

#endif
