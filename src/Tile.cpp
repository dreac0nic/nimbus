
#include "Tile.h"


using namespace std;
using namespace Nimbus;


int Tile::biomeArray[][][]={{{1,2,3},{4,5,6},{7,8,9}}
							 {{10,11,12},{13,14,15},{16,17,18}}
							 {19,20,21},{22,23,24}{25,26,27}};
		


Tile::Tile()
{
}


// create function with inputs for humidity, temperature, ground saturation, Point, and elevation respectively
Tile::Tile(const cellType cell)
{

}
	
Tile::~Tile(void)
{
}


	
	//=======

	void Tile::rainCloud()
	{
		raining=!raining;
	}

	void Tile::windBlowing(Ogre::Real power, bool into)
	{
		//Wind power is calculated for determining how much humidity leaves or enters a tile
		windPower = power;

	}

	double Tile::getHumidity()
	{
		return humidity;
	}

	double Tile::getTemp()
	{
		return temperature;
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
		temperature = temper;
	}

	void Tile::setSat(double grnd)
	{
		grndSat= grnd;
	}

	void Tile::tileInit(double humid, double temper, double grnd, double elev)
	{
	humidity = humid;
	temperature = temper;
	grndSat = grnd;
	elevation = elev;
	nextHumidity = humidity;
	nextTemperature = temperature;
	nextGrndSat = grndSat;
	biomeX=0;
	biomeY=0;
	biomeZ=0;
	biome=biomeArray[biomeX][biomeY][biomeZ];
	raining=false;
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

		tempGrnd = tempGrnd/this->neighbors.size();
		tempTemp = tempTemp/this->neighbors.size();
		tempHumi = tempHumi/this->neighbors.size();

		//Todo add functionallity for raining and wind

		


		//Based off of wind power, tile loses .15* wind power
		//Based off of wind power, tile gains .1 * wind power

		if(tempGrnd >grndSat)
		{nextGrndSat = grndSat +((tempGrnd - grndSat)/2);}
		if(tempGrnd <grndSat)
		{nextGrndSat = grndSat -((grndSat - tempGrnd)/2);}
		if(tempTemp > temperature)
		{nextTemperature = temperature +((tempTemp - temperature)/2);}
		if(tempTemp < temperature)
		{nextTemperature = temperature -((temperature - tempTemp/2));}
		if(tempHumi >humidity)
		{nextHumidity = humidity +((tempHumi - humidity)/2);}
		if(tempHumi <humidity)
		{nextHumidity = humidity -((humidity - tempHumi)/2);}

		//if raining, tile gains 10 % of its current humidity
		//if raining, tile gains 20% of its current ground sat
		//if raining, tile is moves towards 50 temperature by 1
		if(raining)
		{
		nextHumidity = nextHumidity + (.1 * humidity);
		nextGrndSat = nextGrndSat + (.2 * grndSat);
		if(temperature > 50)
			{
				nextTemperature= nextTemperature -1
			}

		if(temperature > 50)
			{
				nextTemperature = nextTemperature + 1
			}
		}
		
	}

	void Tile::updateVar()
	{
		grndSat=nextGrndSat;
		temperature = nextTemperature;
		humidity=nextHumidity;
	}

	void Tile::setType()
	{
		//Biome Number
		//Barren = 1;Sand = 2;Savanna = 3; Plains = 4; Tundra = 5;Tropical 6
		//Temperate = 7; Taiga = 8; Oceans = 9; 
		// Artic = 10; Rivers = 11; Lakes = 12; *these are extra

		//low 33-;med 34-66; high = 67+
		//Threshold is 10

		//Changes for Temperature
		if(temperature < 23)
		{biomeX=0;}
		if(temperature > 43 && temperature < 56)
		{biomeX=1;}
		if(temperature > 77)
		{biomeX=2;}

		//Changes For Ground Saturation
		if(temperature < 23)
		{biomeY=0;}
		if(temperature > 43 && temperature < 56)
		{biomeY=1;}
		if(temperature > 77)
		{biomeY=2;}

		//Changes for Humidity
		if(temperature < 23)
		{biomeZ=0;}
		if(temperature > 43 && temperature < 56)
		{biomeZ=1;}
		if(temperature > 77)
		{biomeZ=2;}

		biome=biomeArray[biomeX][biomeY][biomeZ];
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


