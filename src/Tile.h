#include "Corner.h"

//Temporary; fill or replace
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