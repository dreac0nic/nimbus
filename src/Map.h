#ifndef NIMBUS_MAP_H
#define NIMBUS_MAP_H

#include <vector>
#include <boost/polygon/voronoi.hpp>

namespace Nimbus
{
	class Terrain;
	class Tile;
	class World;

	class Map
	{
	public:
		Map(World* world);
		~Map();

	private:
		/** The terrain that represents this map in three-dimensional space */
		Terrain* mTerrain;

		/** A map of cells to their associated tiles, for quick reference. */
		std::map<cellType, Tile*> mCellTiles;

		/** The list of tiles that makes up this map */
		std::vector<Tile*> mTiles;
	};
}

#endif
