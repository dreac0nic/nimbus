#include "Map.h"

#define PI 3.1415927

using namespace Nimbus;

Map::Map(Voronoi::Voronoi *v, int numLloydRelaxations){
	bumps = rand()*5 + 1;
	startAngle = rand() * 2 * PI;
	dipAngle = rand() * 2 * PI;
	dipWidth = rand() * .5 + .2;
	bounds = v->getPlotBounds();
	for (int i = 0; i < numLloydRelaxations; i++) {
		std::vector<Point*> *points = v->siteCoords();
		for (int i = 0; i < points->size(); i++) {
			std::vector<Point*> *region = v->region(points->at(i));
			float x = 0;
			float y = 0;
			for (int j = 0; j < region->size(); i++) {
				x += region->at(i)->x;
				y += region->at(i)->y;
			}
			x /= region->size();
			y /= region->size();
			points->at(i)->x = x;
			points->at(i)->y = y;
		}
		v = new Voronoi::Voronoi(points, v->getPlotBounds());
	}
	buildGraph(v);
	improveCorners();

	assignCornerElevations();
	assignOceanCoastAndLand();
	redistributeElevations(landCorners());
	assignPolygonElevations();

	calculateDownslopes();
	createRivers();
	assignBiomes();
}

Biome Map::getBiome(Tile *p){
	//not yet implimented
	return OCEAN;
}

Tile *Map::getTileAt(float x, float y){
	Point p = Point(x, y);
	Tile *tile;
	float minDistance= Tile::deltaX + Tile::deltaY; //Will be higher than any possible value.

	for (int i = 0; i < centers->size(); i++){
		if(Voronoi::Util::closeEnough(x, centers->at(i)->loc->x, Tile::deltaX) && Voronoi::Util::closeEnough(y, centers->at(i)->loc->y, Tile::deltaY)){
			if(centers->at(i)->loc->distance(p) < minDistance){
				tile = centers->at(i);
				minDistance = centers->at(i)->loc->distance(p) < minDistance;
			}
		}
	}

	return tile;
}

void Map::improveCorners(){
	Point *newP= (Point*)malloc(sizeof(Point)*corners->size());
	for (int i = 0; i < corners->size(); i++) {
		if (corners->at(i)->border) {
			newP[corners->at(i)->index] = *corners->at(i)->loc;
		} else {
			float x = 0;
			float y = 0;
			for (int j = 0; j < corners->at(i)->touches->size(); j++) {
				x += corners->at(i)->touches->at(j)->loc->x;
				y += corners->at(i)->touches->at(j)->loc->y;
			}
			newP[corners->at(i)->index] = Point(x / corners->at(i)->touches->size(), y / corners->at(i)->touches->size());
		}
	}
	for (int i = 0; i < corners->size(); i++) {
		corners->at(i)->loc = &newP[corners->at(i)->index];
	}
	for (int i = 0; i < edges->size(); i++) {
		if (edges->at(i)->v0 != NULL && edges->at(i)->v1 != NULL) {
			edges->at(i)->setVoronoi(edges->at(i)->v0, edges->at(i)->v1);
		}
	}
}

TileEdge *Map::edgeWithTiles(Tile *t1, Tile *t2){
	for (int i = 0; i < t1->borders->size(); i++) {
		if (t1->borders->at(i)->d0 == t2 || t1->borders->at(i)->d1 == t2) {
			return t1->borders->at(i);
		}
	}
	return NULL;
}

void Map::buildGraph(Voronoi::Voronoi *v){
	std::map<Point*, Tile*> *pointCenterMap = new std::map<Point*, Tile*>();
	std::vector<Point*> *points = v->siteCoords();
	for (int i = 0; i < points->size(); i++) {
		Tile *c = new Tile();
		c->loc = points->at(i);
		c->index = centers->size();
		centers->insert(centers->end(), c);
		pointCenterMap->insert(std::pair<Point*, Tile*>(points->at(i), c));
	}

	//bug fix
	for (int i = 0; i < centers->size(); i++) {
		v->region(centers->at(i)->loc);
	}

	std::vector<Voronoi::Edge*> *libedges = v->edges();
	std::map<int, Corner*> *pointCornerMap = new std::map<int, Corner*>();

	for (int i = 0; i < libedges->size(); i++) {
		Voronoi::LineSegment *vEdge = libedges->at(i)->voronoiEdge();
		Voronoi::LineSegment *dEdge = libedges->at(i)->delaunayLine();

		TileEdge *edge = new TileEdge();
		edge->index = edges->size();
		edges->insert(edges->end(), edge);

		edge->v0 = makeCorner(pointCornerMap, vEdge->points->first);
		edge->v1 = makeCorner(pointCornerMap, vEdge->points->second);
		edge->d0 = pointCenterMap->at(dEdge->points->first);
		edge->d1 = pointCenterMap->at(dEdge->points->second);

		// Centers point to edges-> Corners point to edges->
		if (edge->d0 != NULL) {
			edge->d0->borders->insert(edge->d0->borders->end(), edge);
		}
		if (edge->d1 != NULL) {
			edge->d1->borders->insert(edge->d0->borders->end(), edge);
		}
		if (edge->v0 != NULL) {
			edge->v0->protrudes->insert(edge->d0->borders->end(), edge);
		}
		if (edge->v1 != NULL) {
			edge->v1->protrudes->insert(edge->d0->borders->end(), edge);
		}

		// Centers point to centers->
		if (edge->d0 != NULL && edge->d1 != NULL) {
			addToTileList(edge->d0->neighbors, edge->d1);
			addToTileList(edge->d1->neighbors, edge->d0);
		}

		// Corners point to corners
		if (edge->v0 != NULL && edge->v1 != NULL) {
			addToCornerList(edge->v0->adjacent, edge->v1);
			addToCornerList(edge->v1->adjacent, edge->v0);
		}

		// Centers point to corners
		if (edge->d0 != NULL) {
			addToCornerList(edge->d0->corners, edge->v0);
			addToCornerList(edge->d0->corners, edge->v1);
		}
		if (edge->d1 != NULL) {
			addToCornerList(edge->d1->corners, edge->v0);
			addToCornerList(edge->d1->corners, edge->v1);
		}

		// Corners point to centers
		if (edge->v0 != NULL) {
			addToTileList(edge->v0->touches, edge->d0);
			addToTileList(edge->v0->touches, edge->d1);
		}
		if (edge->v1 != NULL) {
			addToTileList(edge->v1->touches, edge->d0);
			addToTileList(edge->v1->touches, edge->d1);
		}
	}
}

void Map::addToCornerList(std::vector<Corner*> *list, Corner *c){
	if (c != NULL) {
		for (int i = 0; i < list->size(); i++)
		{
			if(c = list->at(i)){
				return;
			}
		}
		list->insert(list->end(), c);
	}
}

void Map::addToTileList(std::vector<Tile*> *list, Tile *t){
	if (t != NULL) {
		for (int i = 0; i < list->size(); i++)
		{
			if(t = list->at(i)){
				return;
			}
		}
		list->insert(list->end(), t);
	}
}

Corner *Map::makeCorner(std::map<int, Corner*> *pointCornerMap, Point *p){
	if (p == NULL) {
		return NULL;
	}
	int index = (int) ((int) p->x + (int) (p->y) * bounds->width * 2);
	Corner *c = pointCornerMap->at(index);
	if (c == NULL) {
		c = new Corner();
		c->loc = p;
		c->border = bounds->liesOnAxes(p);
		c->index = corners->size();
		corners->insert(corners->end(), c);
		pointCornerMap->insert(std::pair<int, Corner*>(index, c));
	}
	return c;
}

void Map::assignCornerElevations(){
	std::list<Corner*> *queue = new std::list<Corner*>();
	for (int i = 0; i < corners->size(); i++) {
		corners->at(i)->water = isWater(corners->at(i)->loc);
		if (corners->at(i)->border) {
			corners->at(i)->elevation = 0;
			queue->insert(queue->end(), corners->at(i));
		} else {
			corners->at(i)->elevation = FLT_MAX;
		}
	}

	while (!queue->empty()) {
		Corner *c = queue->front();
		queue->pop_front();

		for (int i = 0; i < c->adjacent->size(); i++) {
			float newElevation = 0.01 + c->elevation;
			if (!c->water && !c->adjacent->at(i)->water) {
				newElevation += 1;
			}
			if (newElevation < c->adjacent->at(i)->elevation) {
				c->adjacent->at(i)->elevation = newElevation;
				queue->insert(queue->end(), c->adjacent->at(i));
			}
		}
	}
}

bool Map::isWater(Point *p){
	p = new Point(2 * (p->x / bounds->width - 0.5), 2 * (p->y / bounds->height - 0.5));

	float angle = std::atan2(p->y, p->x);
	float length = 0.5 * (std::max(std::abs(p->x), std::abs(p->y)) + p->length());

	float r1 = 0.5 + 0.40 * std::sin(startAngle + bumps * angle + std::cos((bumps + 3) * angle));
	float r2 = 0.7 - 0.20 * std::sin(startAngle + bumps * angle - std::sin((bumps + 2) * angle));
	if (std::abs(angle - dipAngle) < dipWidth
		|| std::abs(angle - dipAngle + (2 * PI)) < dipWidth
		|| std::abs(angle - dipAngle - (2 * PI)) < dipWidth) {
			r1 = r2 = 0.2;
	}
	return !(length < r1 || (length > r1 * ISLAND_FACTOR && length < r2));
}

void Map::assignOceanCoastAndLand(){
	std::list<Tile*> *queue = new std::list<Tile*>();
	float waterThreshold = .3;
	for (int i = 0; i < centers->size(); i++) {
		int numWater = 0;
		for (int j = 0; j < centers->at(i)->corners->size(); j++) {
			if(centers->at(i)->corners->at(j)->border){
				centers->at(i)->corners->at(j)->border = centers->at(i)->corners->at(j)->water = centers->at(i)->corners->at(j)->ocean = true;
				queue->insert(queue->end(), centers->at(i));
			}
			if (centers->at(i)->corners->at(j)->water) {
				numWater++;
			}

		}
		centers->at(i)->water = centers->at(i)->ocean || ((float) numWater / centers->at(i)->corners->size() >= waterThreshold);
	}
	while (!queue->empty()) {
		Tile *center = queue->back();
		queue->pop_back();

		for (int i = 0; i < center->neighbors->size(); i++) {
			if (center->neighbors->at(i)->water && !center->neighbors->at(i)->ocean) {
				center->neighbors->at(i)->ocean = true;
				queue->insert(queue->end(), center->neighbors->at(i));
			}
		}
	}
	for (int i = 0; i < centers->size(); i++) {
		bool oceanNeighbor = false;
		bool landNeighbor = false;
		for (int i = 0; i < centers->at(i)->neighbors->size(); i++) {
			oceanNeighbor |= centers->at(i)->neighbors->at(i)->ocean;
			landNeighbor |= !centers->at(i)->neighbors->at(i)->water;
		}
		centers->at(i)->coast = oceanNeighbor && landNeighbor;
	}

	for (int i = 0; i < corners->size(); i++) {
		int numOcean = 0;
		int numLand = 0;
		for (int j = 0; j < corners->at(i)->touches->size(); i++) {
			numOcean += corners->at(i)->touches->at(j)->ocean ? 1 : 0;
			numLand += !corners->at(i)->touches->at(j)->water ? 1 : 0;
		}
		corners->at(i)->ocean = numOcean == corners->at(i)->touches->size();
		corners->at(i)->coast = numOcean > 0 && numLand > 0;
		corners->at(i)->water = corners->at(i)->border || ((numLand != corners->at(i)->touches->size()) && !corners->at(i)->coast);
	}
}

std::vector<Corner*> *Map::landCorners(){
	std::vector<Corner*> *list = new std::vector<Corner*>();
	for (int i = 0; i < corners->size(); i++) {
		if (!corners->at(i)->ocean && !corners->at(i)->coast) {
			list->insert(list->end(), corners->at(i));
		}
	}
	return list;
}

void Map::redistributeElevations(std::vector<Corner*> *landcorners){
	//implementation not complete.  Need to learn how to use std::sort.  This doesn't seem right.
	std::sort(landcorners->begin(), landcorners->end());

	float SCALE_FACTOR = 1.1;
	for (int i = 0; i < landcorners->size(); i++) {
		float y = (float) i / landcorners->size();
		float x = std::sqrt(SCALE_FACTOR) - std::sqrt(SCALE_FACTOR * (1 - y));
		x = std::min(x, (float)1.0);
		landcorners->at(i)->elevation = x;
	}

	for (int i = 0; i < corners->size(); i++) {
		if (corners->at(i)->ocean || corners->at(i)->coast) {
			corners->at(i)->elevation = 0.0;
		}
	}
}

void Map::assignPolygonElevations(){
	for (int i = 0; i < centers->size(); i++) {
		float total = 0;
		for (int j = 0; j < centers->at(i)->corners->size(); j++) {
			total += centers->at(i)->corners->at(j)->elevation;
		}
		centers->at(i)->elevation = total / centers->at(i)->corners->size();
	}
}

void Map::calculateDownslopes(){
	for (int i = 0; i < corners->size(); i++) {
		Corner *down = corners->at(i);
		//System.out.println("ME: " + c.elevation);
		for (int j = 0; j < down->adjacent->size(); j++) {
			//System.out.println(a.elevation);
			if (down->adjacent->at(j)->elevation <= down->elevation) {
				down = down->adjacent->at(j);
			}
		}
		corners->at(i)->downslope = down;
	}
}

void Map::createRivers(){
	for (int i = 0; i < bounds->width / 2; i++) {
		Corner *c = corners->at(rand()*corners->size());
		if (c->ocean || c->elevation < 0.3 || c->elevation > 0.9) {
			continue;
		}
		// Bias rivers to go west: if (q.downslope.x > q.x) continue;
		while (!c->coast) {
			if (c == c->downslope) {
				break;
			}
			TileEdge *edge = lookupEdgeFromCorner(c, c->downslope);
			if (!edge->v0->water || !edge->v1->water) {
				edge->river++;
				c->river++;
				c->downslope->river++;  // TODO: fix float count
			}
			c = c->downslope;
		}
	}
}

TileEdge *Map::lookupEdgeFromCorner(Corner *c, Corner *downslope){
	for (int i = 0; i <  c->protrudes->size(); i++) {
		if (c->protrudes->at(i)->v0 == downslope || c->protrudes->at(i)->v1 == downslope) {
			return c->protrudes->at(i);
		}
	}
	return NULL;
}

void Map::assignBiomes(){
	for (int i = 0; i < centers->size(); i++) {
		centers->at(i)->biome = getBiome(centers->at(i));
	}
}
