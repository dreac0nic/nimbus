#include <iostream>
#include "Map.h"

using namespace Nimbus;

/** The main function to start the Nimbus application.
 */
int main(int argc, char* argv[])
{

	Map map = Map(new Voronoi::Voronoi(20, 100.0, 100.0),2);

	for (int i = 0; i < map.edges->size(); i++){
		map.edges->at(i)->toString();
	}
	for (int i = 0; i < map.corners->size(); i++){
		map.corners->at(i)->toString();
	}
	for (int i = 0; i < map.centers->size(); i++){
		map.centers->at(i)->toString();
	}

	return 0;
}