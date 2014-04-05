#ifndef NIMBUS_EDGE_H
#define NIMBUS_EDGE_H

#include <map>
#include <stack>
#include <vector>

#include "LineSegment.h"
#include "Rectangle.h"
#include "Site.h"
#include "Vertex.h"

namespace Nimbus
{
	namespace Voronoi
	{
		/** Edges exist between (bisect) two Sites and between (bisect) two Vertices.
			Edges can only be created by extrapolating from two Sites.
		*/
		class Edge
		{
		private:
			//Edge related
			static int _numEdges;
			static std::stack<Edge*> _pool;
			static Edge *create();

			std::map<LR, Point*> _clippedVertices;
			std::map<LR, Site*> _sites;
			int _edgeIndex;
			Vertex *_leftVertex;
			Vertex *_rightVertex;
			Edge();

			//Edge list
			static double _deltax;
			static double _xmin;
			static int _hashSize;
			static std::vector<Halfedge*> _hash;
			static Halfedge *getHash(int b);

			//Edge sorting
			static std::vector<Edge*> _edges;
			static std::vector<LR> _edgeOrientations;

		public:
			//Edge related
			static Edge *DELETED;
			//returns 1 if edge 1 is longer, -1 if edge0 is longer, or 0 if they are equal
			static double compareSitesDistances_MAX(Edge *edge0, Edge *edge1);
			//returns 1 if edge 1 is shorter, -1 if edge0 is shorter, or 0 if they are equal
			static double compareSitesDistances(Edge *edge0, Edge *edge1);

			bool operator< (const Edge &other) const;

			static Edge *createBisectingEdge(Site *site0, Site *site1);
			double a, b, c;
			~Edge();
			void clipVertices(Rectangle *bounds);
			LineSegment *delaunayLine();
			std::map<LR, Point*> *getClippedEnds();
			Site *getLeftSite();
			Vertex *getLeftVertex();
			Site *getRightSite();
			Vertex *getRightVertex();
			bool getVisible();
			bool isPartOfConvexHull();
			void setVertex(LR leftRight, Vertex *v);
			void setLeftSite(Site *s);
			void setRightSite(Site *s);
			Site *site(LR leftRight);
			double sitesDistance();
			Vertex *vertex(LR leftRight);
			LineSegment *voronoiEdge();

			//Edge list
			static Halfedge *_leftEnd;
			static Halfedge *_rightEnd;
			static void initList(double xmin, double deltax, int sqrt_nsites);
			static void insert(Halfedge *lb, Halfedge *newHalfedge);
			static void remove(Halfedge *halfEdge);
			static Halfedge *edgeListLeftNeighbor(Point *p);

			//Edge sorting
			static void initQueue();
			static std::vector<Edge*> *getEdges();
			static std::vector<LR> *getEdgeOrientations();
			static std::vector<Edge*> *reorderBySite(std::vector<Edge*> *origEdges);
			static std::vector<Edge*> *reorderByVertex(std::vector<Edge*> *origEdges);
		};
	}
}

#endif