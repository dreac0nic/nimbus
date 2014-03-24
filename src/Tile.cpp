
#include "Tile.h"
#include "Corner.h"

using namespace Nimbus;

int Tile::deltaX = 0;
int Tile::deltaY = 0;

Tile::Tile()
{
}


// create function with inputs for humidity, temperature, and ground saturation respectively
Tile::Tile(double humid, double temper, double grnd, Point *loc )
{
this->loc = *loc;
}
	
Tile::~Tile(void)
{
}

void Tile::calculateBox(void)
{
	for (unsigned int i = 0; i < corners.size(); i++){
		double x = std::abs(corners.at(i)->loc->x - loc.x);
		if(x > deltaX){
			deltaX = x; // DOUBLE -> INT POSSIBLE LOSS OF DATA
		}

		double y = std::abs(corners.at(i)->loc->y - loc.y);
		if(y > deltaY){
			deltaY = y; // DOUBLE -> INT POSSIBLE LOSS OF DATA
		}
	}
}

void Tile::toString(void)
{
	std::cout << "Tile, x:" << loc.x << ", y:" << loc.y << " " << (border? "b": "-") << (ocean? "o": "-") << (water? "w": "-") << (coast? "c": "-") << "\n";
}
	
	//=======





	double Tile::getHumidity()
	{
		return humidity;
	}

	double Tile::getTemp()
	{
		return temp;
	}

	double Tile::getSat()
	{
		return grndSat;
	}

	void Tile::setHumidity()
	{
		
	}

	void Tile::setTemp()
	{

	}

	void Tile::setSat()
	{

	}

	void Tile::updateTile()
	{

	}

	void Tile::setType()
	{

	}

	String Tile::getType()
	{

		return "words";
	}

	void Tile::spawn()
	{

	}


