#include "Map.h"
#include "Terrain.h"
#include "Tile.h"

using namespace Nimbus;

Map::Map(World* world)
{
	mTerrain = new Terrain(world);

	for (boost::polygon::voronoi_diagram<double>::const_cell_iterator itr = mTerrain->getDiagram()->cells().begin();
		itr != mTerrain->getDiagram()->cells().end(); ++itr)
	{
		Tile* newTile = new Tile(*itr);
		mTiles.push_back(newTile);
		mCellTiles[*itr] = newTile;
	}

	return;
}

Map::~Map() {

}