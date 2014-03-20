#include "Corner.h"

using namespace Nimbus;

Corner::Corner(void)
{
}

Corner::~Corner(void)
{
}

bool Corner::operator< (const Corner &other) const
{
	return (elevation < other.elevation);
}

void Corner::toString()
{
	std::cout << "Corner, x:" << loc->x << ", y:" << loc->y << " " << (water ? "w" : "-") << (ocean ? "o" : "-") << (coast ? "c" : "-");
}
