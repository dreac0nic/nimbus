#include "Map.h"

using namespace Nimbus;

int Tile::deltaX = 0;
int Tile::deltaY = 0;

Tile::Tile(){

}

Tile::Tile(Point *loc){
	Tile::loc = loc;
}

void Tile::calculateBox(){
	for (int i = 0; i < corners->size(); i++){
		float x = std::abs(corners->at(i)->loc->x - loc->x);
		if(x > deltaX){
			deltaX = x;
		}

		float y = std::abs(corners->at(i)->loc->y - loc->y);
		if(y > deltaY){
			deltaY = y;
		}
	}
}