#include "Voronoi.h"

using namespace Nimbus::Voronoi;

std::stack<Vertex*> *Vertex::_pool = new std::stack<Vertex*>;
int Vertex::_numVertices = 0;
Vertex *Vertex::VERTEX_AT_INFINITY = new Vertex(DBL_MAX, DBL_MAX);

Vertex::Vertex(double x, double y){
	init(x,y);
}

Vertex *Vertex::create(double x, double y){
	if (x == DBL_MAX || y== DBL_MAX) {
            return VERTEX_AT_INFINITY;
        }
        if (_pool->size() > 0) {

            Vertex *temp = _pool->top()->init(x, y);
			_pool->pop();
			return temp;
        } else {
            return new Vertex(x, y);
        }
}

Vertex::~Vertex(){

}

Point *Vertex::getCoord(){
	return _coord;
}

int Vertex::getVertexIndex(){
	return _vertIndex;
}

double Vertex::getX(){
	return _coord->x;
}

double Vertex::getY(){
	return _coord->y;
}

Vertex *Vertex::init(double x, double y){
	_coord = new Point(x,y);
	return this;
}

Vertex *Vertex::intersect(Halfedge *halfedge0, Halfedge *halfedge1){
	Edge *edge0, *edge1, *edge;
        Halfedge *halfedge;
        double determinant, intersectionX, intersectionY;
        bool rightOfSite;

        edge0 = halfedge0->edge;
        edge1 = halfedge1->edge;
        if (edge0 == NULL || edge1 == NULL) {
            return NULL;
        }
        if (edge0->getRightSite() == edge1->getRightSite()) {
            return NULL;
        }

        determinant = edge0->a * edge1->b - edge0->b * edge1->a;
        if (-1.0e-10 < determinant && determinant < 1.0e-10) {
            // the edges are parallel
            return NULL;
        }

        intersectionX = (edge0->c * edge1->b - edge1->c * edge0->b) / determinant;
        intersectionY = (edge1->c * edge0->a - edge0->c * edge1->a) / determinant;

        if (Voronoi::compareByYThenX(edge0->getRightSite(), edge1->getRightSite()->getCoord()) < 0) {
            halfedge = halfedge0;
            edge = edge0;
        } else {
            halfedge = halfedge1;
            edge = edge1;
        }
        rightOfSite = intersectionX >= edge->getRightSite()->getX();
        if ((rightOfSite && halfedge->leftRight == LR_LEFT)
                || (!rightOfSite && halfedge->leftRight == LR_RIGHT)) {
            return NULL;
        }

        return Vertex::create(intersectionX, intersectionY);
}

void Vertex::setIndex(){
	_vertIndex = _numVertices++;
}