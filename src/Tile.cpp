
#include "Tile.h"
#include "Corner.h"

#include <vector>

using namespace std;
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
		double tempGrnd=0,tempTemp=0,tempHumi=0;
		//Needs to recieve tiles neighbors
		// Will call all set functions
		for(vector<Tile*>::iterator it; it != this->neighbors.end(); it++)
		{
			Tile* tempTile = *it;// Pointer on neighbor tile
			tempGrnd= tempGrnd + tempTile->getSat();
			tempTemp= tempTemp + tempTile->getTemp();
			tempHumi= tempHumi + tempTile->getHumidity();
			//if(tempTile->water) cout << "IT IS WATER!!!" << endl;
		}

		tempGrnd = tempGrnd/this->neighbors.size;
		tempTemp = tempTemp/this->neighbors.size;
		tempHumi = tempHumi/this->neighbors.size;

		if(tempGrnd >grndSat)
		{grndSat = grndSat +((tempGrnd - grndSat)/2);}
		if(tempGrnd <grndSat)
		{grndSat = grndSat -((grndSat - tempGrnd)/2);}
		if(tempTemp >temp)
		{temp = temp +((tempTemp - temp)/2);}
		if(tempTemp <temp)
		{temp = temp -((temp - tempTemp/2));}
		if(tempHumi >humidity)
		{humidity = humidity +((tempHumi - humidity)/2);}
		if(tempHumi <humidity)
		{humidity = humidity -((humidity - tempHumi)/2);}


	}

	void Tile::setType()
	{
		//Biome Number
		//Barren = 1;Sand = 2;Savanna = 3; Plains = 4; Tundra = 5;Tropical 6
		//Temperate = 7; Taiga = 8; Oceans = 9; 
		// Artic = 10; Rivers = 11; Lakes = 12; *these are extra
		
		biome= 1;

		//low 33-;med 34-66; high = 67+



	}

	std::string Tile::getType()
	{
		string toReturn;

		switch (biome){
		case 1: toReturn= "Barren";
			break;
		case 2: toReturn= "Sand";
			break;
		case 3: toReturn= "Savanna";
			break;
			case 4: toReturn= "Plains";
			break;
			case 5: toReturn= "Tundra";
			break;
			case 6: toReturn= "Tropical";
			break;
			case 7: toReturn= "Temperate";
			break;
			case 8: toReturn= "Taiga";
			break;
			case 9: toReturn= "Oceans";
			break;
			case 10: toReturn= "Artic";
			break;

		}
		return toReturn;
	}

	void Tile::spawn()
	{

	}


