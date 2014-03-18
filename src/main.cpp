#include <iostream>
#include "Map.h"
#include "MapGeneration\Voronoi.h"

using namespace Nimbus::Voronoi;

void pause(){
	std::cout << "Press Enter.\n";
	std::cin.get();
}

void testVoronoi(){
	Edge::initList(0,1,2);
	Edge::initQueue();
	Site site1 = Site(new Point(0,0), 0, 1.0);
	Site site2 = Site(new Point(0,4), 1, 3.0);
	Edge edge = *Edge::createBisectingEdge(&site1, &site2);

	edge.clipVertices(new Rectangle(-1,-1,2,2));
	std::cout << edge.getClippedEnds()->at(LR_LEFT)->y << " " << edge.getClippedEnds()->at(LR_LEFT)->x << "\n";
	std::cout << edge.getClippedEnds()->at(LR_RIGHT)->y << " " << edge.getClippedEnds()->at(LR_RIGHT)->x;

	pause();
}

/** The main function to start the Nimbus application.
*/
int main(int argc, char* argv[])
{
	testVoronoi();
}