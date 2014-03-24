#ifndef MAP_H
#define MAP_H


#include <OgreVector2.h>
#include "OGRE\OgreVector2.h"
#include <utility>
#include <stdlib.h>
#include "MapGeneration/Voronoi.h"

namespace Nimbus{
	typedef Ogre::Vector2 Point;

	class Tile;
	class Corner;
	class TileEdge;
	class Map; // wars

	//Temporary; complete or replace later
	enum Biome{MOUNTAIN, DESERT, COAST, OCEAN};

	//This is the Center class renamed to fit our terms.
	class Tile{
	public:
		static int deltaX;
		static int deltaY;

		int index;
		Point loc;
		std::vector<Corner*> corners;
		std::vector<Tile*> neighbors;
		std::vector<TileEdge*> borders;
		bool border, ocean, water, coast;
		double elevation;
		double moisture;
		double area;
		Biome biome;

		Tile();
		Tile(Point *loc);
		void calculateBox();
		void toString();
	};

	class Corner{
	public:
		std::vector<Tile*> touches;
		std::vector<Corner*> adjacent;
		std::vector<TileEdge*> protrudes;
		Point *loc;
		int index;
		bool border;
		double elevation;
		bool water, ocean, coast;
		Corner *downslope;
		int river;
		double moisture;

		bool operator< (const Corner &other) const;
		void toString();
	};

	class TileEdge{
	public:
		int index;
		Tile *d0, *d1;
		Corner *v0, *v1;
		Point *midpoint;
		int river;

		void setVoronoi(Corner *c0, Corner *c1);
		void toString();
	};

	class Map{
	private:
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
		std::vector<TileEdge*> edges;
		std::vector<Corner*> corners;
		std::vector<Tile*> centers;
		Voronoi::Rectangle bounds;
		double **noise;
		double ISLAND_FACTOR;
		int bumps;
		double startAngle;
		double dipAngle;
		double dipWidth;

		Map(Voronoi::Voronoi *v, int numLloydRelaxations);
		Biome getBiome(Tile *p);
		//Finds the nearest tile to a given point.
		Tile *getTileAt(double x, double y);
		void toString();
	};
}

#endif