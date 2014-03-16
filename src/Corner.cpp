#include "Map.h"

using namespace Nimbus;

void Corner::toString(){
	std::cout << "Corner, x:" << loc->x << ", y:" << loc->y << " " << (water ? "w" : "-") << (ocean ? "o" : "-") << (coast ? "c" : "-");
}
