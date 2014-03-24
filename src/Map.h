#ifndef NIMBUS_MAP_H
#define NIMBUS_MAP_H

#include <utility>

#include <OGRE\OgreVector2.h>

#include "Tile.h"
#include "MapGeneration/Voronoi.h"

namespace Nimbus
{
	class Map;

	class Map
	{
	private:
		// Generation functions
		void improveCorners();
		TileEdge *edgeWithTiles(Tile *t1, Tile *t2);
		void buildGraph(Voronoi::Voronoi *v);
		void addToCornerList(std::vector<Corner*> *list, Corner *c);
		void addToTileList(std::vector<Tile*> *list, Tile *c);
		Corner *makeCorner(std::map<int, Corner*> *pointCornerMap, Point *p);
		void assignCornerElevations();
		bool isWater(Point *p);
		void assignOceanCoastAndLand();
		std::vector<Corner*> *landCorners();
		void redistributeElevations(std::vector<Corner*> *landcorners);
		void assignPolygonElevations();
		void calculateDownslopes();
		void createRivers();
		TileEdge *lookupEdgeFromCorner(Corner *c, Corner *downslope);
		void assignCornerMoisture();
		void redistributeMoisture(std::vector<Corner*> *landCorners);
		void assignPolygonMoisture();
		void assignBiomes();

	public:
		//////////
		// Member Variables

		// Map content objects

		/* List of all edges on the map. */
		std::vector<TileEdge*> edges;

		/* List of all corners on the map. */
		std::vector<Corner*> corners;

		/* List of all tile centers on the map. */
		std::vector<Tile*> centers;

		/* Bounds of map. */
		Voronoi::Rectangle bounds;

		// Map generation elements

		double **noise;
		double ISLAND_FACTOR;
		int bumps;
		double startAngle;
		double dipAngle;
		double dipWidth;

		//////////
		// Public Member Functions

		/** Constructs a map using a voronoi generator
		@param v A pointer to a Voronoi generator
		@param numLloydRelaxations
		*/
		Map(Voronoi::Voronoi *v, int numLloydRelaxations);

		/** Gets the biome of a given tile... may need to change.
		@param tile A pointer to a tile to get the biome of.
		@return The enumeration of which biome the given tile is.
		*/
		Biome getBiome(Tile *p);

		/**Finds the nearest tile to a given point.
		@param x The x coordinate of the tile.
		@param y The y coordinate of the tile.
		@return A pointer to the tile closest to the given point.
		*/
		Tile *getTileAt(double x, double y);

		/** Mystical function of converting stuff to yarn.
		*/
		void toString();
	};
}

#endif