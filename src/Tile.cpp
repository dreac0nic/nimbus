
#include "Tile.h"
#include "Corner.h"

using namespace Nimbus;

int Tile::deltaX = 0;
int Tile::deltaY = 0;

Tile::Tile()
{
}


// create function with inputs for humidity, temperature, ground saturation, Point, and elevation respectively
Tile::Tile(double humid, double temper, double grnd, Point *loc, double elev)
{
this->loc = *loc;
humidity = humid;
temp = temper;
grndSat = grnd;
elevation = elev;
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

	void Tile::setHumidity(double humi)
	{
		humidity = humi;
	}

	void Tile::setTemp(double temper)
	{
		temp= temper;
	}

	void Tile::setSat(double grnd)
	{
		grndSat= grnd;
	}

	void Tile::updateTile()
	{
		//Needs to recieve tiles neighbors
		// Will call all set functions
	}

	void Tile::setType()
	{
		//Biome Number
		//Barren = 1;Sand = 2;Savanna = 3; Plains = 4; Tundra = 5;Tropical 6
		//Temperate = 7; Taiga = 8; Oceans = 9; 
		// Artic = 10; Rivers = 11; Lakes = 12; *these are extra
		biome= 1;
	}

	std::string Tile::getType()
	{
		return "words";
	}

	void Tile::spawn()
	{

	}


