#include "Voronoi.h"

using namespace Nimbus::Voronoi;

void Voronoi::init(){
	Edge::initList(0,1,2);
	Edge::initQueue();
	Halfedge::initQueue(0, 1, 2);
	Site::initList();
}

void Voronoi::addSites(std::vector<Point*> *points){
	int length = points->size();
	for (int i = 0; i < length; ++i) {
		addSite(points->at(i), i);
	}

}

void Voronoi::addSite(Point *p, int index){
	double weight = rand() * 100;
	Site *site = Site::create(p, index, weight);
	Site::push(site);
	_sitesIndexedByLocation->insert(std::pair<Point*,Site*>(p, site));
}

std::vector<LineSegment*> *Voronoi::delaunayLinesForEdges(std::vector<Edge*> *edges){
	std::vector<LineSegment*> *segments = new std::vector<LineSegment*>;

	for (int i = 0; i < edges->size(); i++) {
		segments->insert(segments->end(), edges->at(i)->delaunayLine());
	}

	return segments;
}

std::vector<LineSegment*> *Voronoi::delaunayLinesForSite(Point *coord){
	return delaunayLinesForEdges(selectEdgesForSitePoint(coord, _edges));
}

void Voronoi::fortunesAlgorithm(){
	Site *newSite, *bottomSite, *topSite, *tempSite;
	Vertex *v, *vertex;
	Point *newintstar = NULL;
	LR leftRight;
	Halfedge *lbnd, *rbnd, *llbnd, *rrbnd, *bisector;
	Edge *edge;

	Rectangle *dataBounds = Site::getSitesBounds();

	int sqrt_nsites = (int) sqrt(Site::getLength() + 4);
	Halfedge::initQueue(dataBounds->y, dataBounds->height, sqrt_nsites);
	Edge::initList(dataBounds->x, dataBounds->width, sqrt_nsites);
	std::vector<Halfedge*> *halfEdges = new std::vector<Halfedge*>;
	std::vector<Vertex*> *vertices = new std::vector<Vertex*>;
	Site *bottomMostSite = Site::next();
	newSite = Site::next();

	for (;;) {
		
		if (Halfedge::empty() == false) {
			newintstar = Halfedge::min();
		}

		if (newSite != NULL
			&& (Halfedge::empty() || compareByYThenX(newSite, newintstar) < 0)) {
				/* new site is smallest */
				//trace("smallest: new site " + newSite);

				// Step 8:
				lbnd = Edge::edgeListLeftNeighbor(newSite->getCoord());	// the Halfedge just to the left of newSite
				//trace("lbnd: " + lbnd);
				rbnd = lbnd->edgeListRightNeighbor;		// the Halfedge just to the right
				//trace("rbnd: " + rbnd);
				bottomSite = rightRegion(lbnd, bottomMostSite);		// this is the same as leftRegion(rbnd)
				// this Site determines the region containing the new site
				//trace("new Site is in region of existing site: " + bottomSite);

				// Step 9:
				edge = Edge::createBisectingEdge(bottomSite, newSite);
				//trace("new edge: " + edge);
				_edges->push_back(edge);
				
				bisector = Halfedge::create(edge, LR_LEFT);
				halfEdges->insert(halfEdges->end(), bisector);
				// inserting two Halfedges into edgeList constitutes Step 10:
				// insert bisector to the right of lbnd:
				Edge::insert(lbnd, bisector);

				// first half of Step 11:
				if ((vertex = Vertex::intersect(lbnd, bisector)) != NULL) {
					vertices->insert(vertices->end(), vertex);
					Halfedge::remove(lbnd);
					lbnd->vertex = vertex;
					lbnd->ystar = vertex->getY() + newSite->dist(vertex->getCoord());
					Halfedge::insert(lbnd);
				}

				lbnd = bisector;
				bisector = Halfedge::create(edge, LR_RIGHT);
				halfEdges->insert(halfEdges->end(), bisector);
				// second Halfedge for Step 10:
				// insert bisector to the right of lbnd:
				Edge::insert(lbnd, bisector);

				// second half of Step 11:
				if ((vertex = Vertex::intersect(bisector, rbnd)) != NULL) {
					vertices->insert(vertices->end(), vertex);
					bisector->vertex = vertex;
					bisector->ystar = vertex->getY() + newSite->dist(vertex->getCoord());
					Halfedge::insert(bisector);
				}

				newSite = Site::next();
		} else if (Halfedge::empty() == false) {
			/* intersection is smallest */
			lbnd = Halfedge::extractMin();
			llbnd = lbnd->edgeListLeftNeighbor;
			rbnd = lbnd->edgeListRightNeighbor;
			rrbnd = rbnd->edgeListRightNeighbor;
			bottomSite = leftRegion(lbnd, bottomMostSite);
			topSite = rightRegion(rbnd, bottomMostSite);

			v = lbnd->vertex;
			v->setIndex();
			lbnd->edge->setVertex(lbnd->leftRight, v);
			rbnd->edge->setVertex(rbnd->leftRight, v);
			Edge::remove(lbnd);
			Halfedge::remove(rbnd);
			Edge::remove(rbnd);
			leftRight = LR_LEFT;
			if (bottomSite->getY() > topSite->getY()) {
				tempSite = bottomSite;
				bottomSite = topSite;
				topSite = tempSite;
				leftRight = LR_RIGHT;
			}
			edge = Edge::createBisectingEdge(bottomSite, topSite);
			_edges->insert(_edges->end(), edge);
			bisector = Halfedge::create(edge, leftRight);
			halfEdges->insert(halfEdges->end(), bisector);
			Edge::insert(llbnd, bisector);
			edge->setVertex(LR((leftRight+1)%2), v);
			if ((vertex = Vertex::intersect(llbnd, bisector)) != NULL) {
				vertices->insert(vertices->end(), vertex);
				Halfedge::remove(llbnd);
				llbnd->vertex = vertex;
				llbnd->ystar = vertex->getY() + bottomSite->dist(vertex->getCoord());
				Halfedge::insert(llbnd);
			}
			if ((vertex = Vertex::intersect(bisector, rrbnd)) != NULL) {
				vertices->insert(vertices->end(), vertex);
				bisector->vertex = vertex;
				bisector->ystar = vertex->getY() + bottomSite->dist(vertex->getCoord());
				Halfedge::insert(bisector);
			}
		} else {

			break;
		}
	}

	// heap should be empty now
	Halfedge::disposeQueue();
	//Edge::disposeList();

	//for (Halfedge halfEdge : halfEdges) {
	//Halfedge::reallyDispose();
	//}
	halfEdges->clear();

	// we need the vertices to clip the edges
	for (int i = 0; i < _edges->size(); i++) {
		_edges->at(i)->clipVertices(_plotBounds);
	}
	// but we don't actually ever use them again!
	//for (int i = 0; i < vertices->size(); i++) {
	//vertices->at(0)->dispose();
	//}
	vertices->clear();
}

std::vector<Edge*> *Voronoi::hullEdges(){
	std::vector<Edge*> *filtered = new std::vector<Edge*>;

	for (int i = 0; i < _edges->size(); i++) {
		if (_edges->at(i)->isPartOfConvexHull()) {
			filtered->insert(filtered->end(), _edges->at(i));
		}
	}



	return filtered;
}

void Voronoi::init(std::vector<Point*> *points, Rectangle *plotBounds){
	init();
	_sitesIndexedByLocation = new std::map<Point*,Site*>();
	addSites(points);
	_plotBounds = plotBounds;
	_edges = new std::vector<Edge*>();
}

std::vector<Edge*> *Voronoi::selectEdgesForSitePoint(Point *coord, std::vector<Edge*> *edgesToTest){
	std::vector<Edge*> *filtered = new std::vector<Edge*>();

	for (int i = 0; i < edgesToTest->size(); i++) {
		if (((edgesToTest->at(i)->getLeftSite() != NULL && edgesToTest->at(i)->getLeftSite()->getCoord() == coord)
			|| (edgesToTest->at(i)->getRightSite() != NULL && edgesToTest->at(i)->getRightSite()->getCoord() == coord))) {
				filtered->insert(filtered->end(), edgesToTest->at(i));
		}
	}
	return filtered;
}

std::vector<LineSegment*> *Voronoi::visibleLineSegments(std::vector<Edge*> *edgesToTest){
	std::vector<LineSegment*> *segments = new std::vector<LineSegment*>();

	for (int i = 0; i < edgesToTest->size(); i++) {
		if (edgesToTest->at(i)->getVisible()) {
			Point *p1 = edgesToTest->at(i)->getClippedEnds()->at(LR_LEFT);
			Point *p2 = edgesToTest->at(i)->getClippedEnds()->at(LR_RIGHT);
			segments->insert(segments->end(), new LineSegment(p1, p2));
		}
	}

	return segments;
}

int Voronoi::compareByYThenX(Site *s1, Point *p1){
	if (s1->getY() < p1->y) {
		return -1;
	}
	if (s1->getY() > p1->y) {
		return 1;
	}
	if (s1->getX() < p1->x) {
		return -1;
	}
	if (s1->getX() > p1->x) {
		return 1;
	}
	return 0;
}

Voronoi::Voronoi(std::vector<Point*> *points, Rectangle *plotBounds){
	init(points, plotBounds);
	fortunesAlgorithm();
}

Voronoi::Voronoi(std::vector<Point*> *points){
	double maxWidth = 0, maxHeight = 0;
	for (int i = 0; i < points->size(); i++) {
		maxWidth = std::max(maxWidth, (double) points->at(i)->x);
		maxHeight = std::max(maxHeight, (double) points->at(i)->y);
	}
	init(points, new Rectangle(0, 0, maxWidth, maxHeight));
	fortunesAlgorithm();
}

Voronoi::Voronoi(int numSites, double maxWidth, double maxHeight){
	std::vector<Point*> *points = new std::vector<Point*>;

	for (int i = 0; i < numSites; i++) {
		points->push_back(new Point((double)rand() / RAND_MAX * maxWidth, (double)rand() / RAND_MAX * maxHeight));
	}

	init(points, new Rectangle(0, 0, maxWidth, maxHeight));
	
	fortunesAlgorithm();
}

Voronoi::~Voronoi(){

}

std::vector<Edge*> *Voronoi::edges(){
	return _edges;
}

Rectangle *Voronoi::getPlotBounds(){
	return _plotBounds;
}

std::vector<LineSegment*> *Voronoi::hull(){
	return delaunayLinesForEdges(hullEdges());
}

std::vector<Point*> *Voronoi::hullPointsInOrder(){
	std::vector<Edge*> *hulledges = hullEdges();

	std::vector<Point*> *points = new std::vector<Point*>;
	if (hulledges->empty()) {
		return points;
	}

	Edge::initQueue();
	hulledges = Edge::reorderBySite(hulledges);
	std::vector<LR> *orientations = Edge::getEdgeOrientations();
	//Edge::disposeQueue();

	LR orientation;

	int n = hulledges->size();
	for (int i = 0; i < n; ++i) {
		Edge *edge = hulledges->at(i);
		orientation = orientations->at(i);
		points->insert(points->end(), edge->site(orientation)->getCoord());
	}
	return points;
}

Site *Voronoi::leftRegion(Halfedge *he, Site *bottomMostSite){
	Edge *edge = he->edge;
	if (edge == NULL) {
		return bottomMostSite;
	}
	return edge->site(he->leftRight);
}

Site *Voronoi::rightRegion(Halfedge *he, Site *bottomMostSite){
	Edge *edge = he->edge;
	if (edge == NULL) {
		return bottomMostSite;
	}
	return edge->site((LR)(he->leftRight+1%2));
}

std::vector<Point*> *Voronoi::neighborSitesForSite(Point *coord){
	std::vector<Point*> *points = new std::vector<Point*>;
	Site *site = _sitesIndexedByLocation->at(coord);
	if (site == NULL) {
		return points;
	}
	std::vector<Site*> *sites = site->neighborSites();
	for (int i = 0; i < sites->size(); i++) {
		points->insert(points->end(), sites->at(i)->getCoord());
	}
	return points;
}

std::vector<Point*> *Voronoi::region(Point *p){
	Site *site = _sitesIndexedByLocation->at(p);
	if (site == NULL) {
		return new std::vector<Point*>;
	}
	return site->region(_plotBounds);
}

std::vector< std::vector<Point*> *> *Voronoi::regions(){
	return Site::regions(_plotBounds);
}

std::vector<Point*> *Voronoi::siteCoords(){
	return Site::siteCoords();
}

std::vector<LineSegment*> *Voronoi::voronoiBoundaryForSite(Point *coord){
	return visibleLineSegments(selectEdgesForSitePoint(coord, _edges));
}

std::vector<LineSegment*> *Voronoi::voronoiDiagram(){
	return visibleLineSegments(_edges);
}
