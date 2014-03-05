#ifndef NIMBUS_MAP_H
#define NIMBUS_MAP_H

#include <map>
#include <vector>
#include "Voronoi.h"

namespace Nimbus{
	
	/*Map is a structure to hold the map tiles.

	This is a Voronoi graph, which means the tiles are not square.  They are irregular polygons,
	and stored by their centers, edges, and corners.
	*/
	class Map{
	private:
		typedef std::map<Voronoi::Point, Voronoi::Tile*> _tilesByLocation;
		Voronoi::TileList _tiles;
		std::vector<Voronoi::Edge> _edges;
		Voronoi::Rectangle _bounds;

		void addSites(std::vector<Voronoi::Point> points);
		void addSite(Voronoi::Point, int index);

	public:
		Map(int numSites, double maxWidth, double maxHeight);

		std::vector<Voronoi::Edge> edges();
		std::vector<Voronoi::Point> region(Voronoi::Point p);
		std::vector<Voronoi::Point> neighborSitesForSite(Voronoi::Point coord);
		std::vector<Voronoi::Edge> selectEdgesForSitePoint(Voronoi::Point coord, std::vector<Voronoi::Edge> edgesToTest);
		std::vector<Voronoi::LineSegment> visibleLineSegments(std::vector<Voronoi::Edge> edges);
	};//end class Map
}//end namespace Nimbus
#endif