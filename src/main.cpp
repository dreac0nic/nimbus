#include <iostream>
#include "Map.h"
#include "MapGeneration\Voronoi.h"
#include "MapGeneration\Edge.h"
#include "MapGeneration\Halfedge.h"

using namespace Nimbus::Voronoi;

void pause(){
	std::cout << "Press Enter.\n";
	std::cin.get();
}

void testVoronoi(){
	Edge::initList(0,1,2);
	Edge::initQueue();
	Halfedge::initQueue(0, 1, 2);
	Site site1 = Site(new Point(0,0), 0, 1.0);
	Site site2 = Site(new Point(0,4), 1, 3.0);

	Edge edge = *Edge::createBisectingEdge(&site1, &site2);

	edge.clipVertices(new Rectangle(-1,-1,2,2));

	pause();
}

/** The main function to start the Nimbus application.
*/
int main(int argc, char* argv[])
{
	double width = 600;
	double height = 600;
	int numsites = 3000;
	std:: vector<Point*> points;
	srand(94870395729);

	Voronoi v = Voronoi(numsites, width, height);
}