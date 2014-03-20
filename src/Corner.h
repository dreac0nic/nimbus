#include "Tile.h"
#include "TileEdge.h"

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