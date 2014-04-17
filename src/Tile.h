#ifndef NIMBUS_TILE_H
#define NIMBUS_TILE_H

#include <boost/polygon/voronoi.hpp>

namespace Nimbus
{
	typedef boost::polygon::voronoi_diagram_traits<double>::cell_type cellType;

	class Tile
	{
	public:
		Tile(const cellType cell);
		~Tile();

	private:
		/** The cell that this tile is represented by. */
		cellType* mCell;

		/** A list of all neighbouring tiles that we can traverse. */
		std::vector<Tile*> mNeighbours;

	public:
		/** Returns a pointer to the cell that identifies this tile.
		 */
		cellType* getCell(void);

		/** Returns a pointer to the list of neighbouring tiles. 
		 */
		std::vector<Tile*>* getNeighbours(void);

		/** A fire-once function to set the neighbours of a tile.
		 In normal operation, this should only be called once after generation.

		 @param neighbours A vector of Tiles that neighbour this tile, passed in by map.
		 */
		void setNeighbours(std::vector<Tile*> neighbours);
	};
}

#endif
