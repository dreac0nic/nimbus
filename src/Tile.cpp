
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
Tile::Tile(double humid, double temper, double grnd, double elev)
{
humidity = humid;
temp = temper;
grndSat = grnd;
elevation = elev;
nextHumidity = humidity;
nextTemperature = temp;
nextGrndSat = grndSat;
biomeX=0;
biomeY=0;
biomeZ=0;
biome=biomeArray[biomeX][biomeY][biomeZ];
raining=false;
}
	
Tile::~Tile(void)
{
}


	
	//=======

	void Tile::rainCloud()
	{
		raining=!raining;
	}



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
		if(tempTemp >temp)
		{nextTemperature = temp +((tempTemp - temp)/2);}
		if(tempTemp <temp)
		{nextTemperature = temp -((temp - tempTemp/2));}
		if(tempHumi >humidity)
		{nextHumidity = humidity +((tempHumi - humidity)/2);}
		if(tempHumi <humidity)
		{nextHumidity = humidity -((humidity - tempHumi)/2);}

		//if raining, tile gains 10 % of its current humidity
		//if raining, tile gains 20% of its current ground sat
		//if raining, tile is moves towards 50 temp by 1
		if(raining)
		{
		
		}
		
	}

	void Tile::updateVar()
	{
		grndSat=nextGrndSat;
		temp = nextTemperature;
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
		if(temp<23)
		{biomeX=0;}
		if(temp>43 && temp<56 )
		{biomeX=1;}
		if(temp>77)
		{biomeX=2;}

		//Changes For Ground Saturation
		if(temp<23)
		{biomeY=0;}
		if(temp>43 && temp<56 )
		{biomeY=1;}
		if(temp>77)
		{biomeY=2;}

		//Changes for Humidity
		if(temp<23)
		{biomeZ=0;}
		if(temp>43 && temp<56 )
		{biomeZ=1;}
		if(temp>77)
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


