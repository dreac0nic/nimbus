#include "Map.h"

using namespace Nimbus;

void TileEdge::setVoronoi(Corner *c0, Corner *c1){
	v0 = c0;
	v1 = c1;
	midpoint = new Point((v0->loc->x + v1 ->loc->x) / 2, (v0->loc->y + v1 ->loc->y) / 2);
}
