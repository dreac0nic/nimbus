#ifndef VORONOI_H
#define VORONOI_H

#include <OgreVector2.h>
#include "OGRE\OgreVector2.h"
#include <stack>
#include <utility>
#include <stdlib.h>

namespace Nimbus{
	namespace Voronoi{
		//numbers are testing priority ratings (High numbers done first)
		class Rectangle;//tested
		class Edge;//3
		class Halfedge;//5
		class LineSegment;//tested
		class Polygon;//tested
		class Site;//2
		class Vertex;//4
		class Voronoi;//1

		enum Winding{CLOCKWISE, COUNTERCLOCKWISE, NONE};
		enum LR{LR_LEFT, LR_RIGHT};
		enum BoundsCheck{TOP, BOTTOM, LEFT, RIGHT};

		typedef Ogre::Vector2 Point;

		class Util{
		public:
			static bool closeEnough(double d1, double d2, double diff);
		};

		/*Helper class for bounding box.

		Capable of checking whether a point is inside a box.
		*/
		class Rectangle{
		public:
			double x, y, width, height, right, bottom, left, top;
			Rectangle();
			Rectangle(double x, double y, double width, double height);
			bool liesOnAxes(Point *p);
			bool inBounds(Point *p);
			bool inBounds(double x0, double y0);
		};

		/*Edges exist between (bisect) two Sites and between (bisect) two Vertices.

		Edges can only be created by extrapolating from two Sites.
		*/
		class Edge{
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

		/*Each Edge has two Halfedges (right and left).

		Halfedges are useful in creating Corners where Edges meet.
		*/
		class Halfedge{
		private:
			static std::stack<Halfedge*> _pool;
			static std::vector<Halfedge*> _hash;
			static int _count;
			static int _minBucket;
			static int _hashSize;
			static double _ymin;
			static double _deltay;
			static bool isEmpty(int bucket);
			static void adjustMinBucket();

			Halfedge *init(Edge *edge, LR lr);

		public:
			static void initQueue(double ymin, double deltay, int sqrt_nsites);
			static void disposeQueue();
			static void insert(Halfedge *halfEdge);
			static void remove(Halfedge *halfEdge);
			static int bucket(Halfedge *halfEdge);
			static bool empty();
			static Point *min();
			static Halfedge *extractMin();

			Halfedge *edgeListLeftNeighbor;
			Halfedge *edgeListRightNeighbor;
			Halfedge *nextInPriorityQueue;
			Edge *edge;
			Vertex *vertex;
			LR leftRight;
			double ystar;
			Halfedge(Edge *edge, LR lr);
			~Halfedge();
			static Halfedge *create(Edge *edge, LR lr);
			static Halfedge *createDummy();
			bool isLeftOf(Point *p);
		};

		class LineSegment{
		public:
			std::pair<Point*, Point*> points;

			LineSegment(Point *point0, Point *point1);
			LineSegment(std::pair <Point*, Point*> *points);
			~LineSegment();
			static double compareLengthsMax(LineSegment *segment0, LineSegment *segment1);
			static double compareLengths(LineSegment *edge0, LineSegment *edge1);
		};

		class Polygon{
		private:
			double signedDoubleArea();
			std::vector<Point*> _vertices;

		public:
			Polygon(std::vector<Point*> vertices);
			~Polygon();
			Winding winding();
		};

		/*Sites equate to the contents of a tile.

		The center of a site is a random point, and the interior contains all points closer to that vertex
		than any other.  The edges of each site are formed along the perpendicular bisector between two
		neighboring vertices.
		*/
		class Site{
		private:
			static std::stack<Site*> _pool;
			static const double EPSILON;
			static double compare(Site *s1, Site *s2);
			static bool closeEnough(Point *p0, Point *p1);
			static int check(Point *point, Rectangle *bounds);
			static std::vector<Site*> _sites;
			static int _currentIndex;
			static bool _sorted;

			Point _coord;
			int _siteIndex;
			std::vector<LR> *_edgeOrientations;
			std::vector<Point*> *_region;
			void clear();
			void reorderEdges();
			std::vector<Point*> *clipToBounds(Rectangle *bounds);
			void connect(std::vector<Point*> *points, int j, Rectangle *bounds, bool closingUp);
			Site *init(Point *p, int index, double weight);

		public:
			static Site *create(Point *p, int index, double weight);
			static void initList();
			static void disposeList();
			static int push(Site *site);
			static int getLength();
			static Site *next();
			static Rectangle *getSitesBounds();
			static std::vector<Point *> *siteCoords();
			static std::vector< std::vector<Point *> *> *regions(Rectangle * plotBounds);

			bool operator< (const Site &other) const;

			double weight;
			std::vector<Edge*> *_edges;
			Site(Point *p, int index, double weight);
			~Site();
			int comp(const void *a,const void *b);
			Point *getCoord();
			void addEdge(Edge *edge);
			Edge *nearestEdge();
			std::vector<Site*> *neighborSites();
			Site *neighborSite(Edge *edge);
			std::vector<Point*> *region(Rectangle *clippingbounds);
			double getX();
			double getY();
			double dist(Point *p);
		};

		/*Provides a way to create points at the intersection of two edges.

		Tracks the total number of vertices and stores them by index.
		*/
		class Vertex{
		private:
			static std::stack<Vertex*> *_pool;
			static int _numVertices;
			Point *_coord;
			int _vertIndex;

			static Vertex *create(double x, double y);
			Vertex(double x, double y);
			Vertex();
		public:
			static Vertex *VERTEX_AT_INFINITY;
			~Vertex();

			Vertex *init(double x, double y);
			Point *getCoord();
			int getVertexIndex();
			double getX();
			double getY();
			void setIndex();

			//This is the only way to make a vertex
			static Vertex *intersect(Halfedge *halfedge0, Halfedge *halfedge1);
		};

		/*The Voronoi object is the only map generation object that should interact with outside classes.

		This object holds all of the information pertaining to a generated map.  After the info is copied
		out of the class, Voronoi and all of the objects it creates are to be garbage collected, leaving
		the map information in other options.
		*/
		class Voronoi{
		private:
			std::vector<Edge*> *_edges;
			Rectangle *_plotBounds;
			std::map<Point*, Site*> *_sitesIndexedByLocation;

			void addSites(std::vector<Point*> *points);
			void addSite(Point *p, int index);
			std::vector<LineSegment*> *delaunayLinesForEdges(std::vector<Edge*> *edges);
			void fortunesAlgorithm();
			std::vector<Edge*> *hullEdges();
			void init(std::vector<Point*> *points, Rectangle *plotBounds);
			std::vector<Edge*> *selectEdgesForSitePoint(Point *coord, std::vector<Edge*> *edgesToTest);
			std::vector<LineSegment*> *visibleLineSegments(std::vector<Edge*> *edgesToTest);

		public:
			static int compareByYThenX(Site *s1, Point *p1);
			static void init();

			Voronoi(std::vector<Point*> *points, Rectangle *plotBounds);
			Voronoi(std::vector<Point*> *points);
			Voronoi(int numSites, double maxWidth, double maxHeight);
			~Voronoi();
			std::vector<LineSegment*> *delaunayLinesForSite(Point *coord);
			std::vector<Edge*> *edges();
			Rectangle *getPlotBounds();
			std::vector<LineSegment*> *hull();
			std::vector<Point*> *hullPointsInOrder();
			Site *leftRegion(Halfedge *he, Site *bottomMostSite);
			Site *rightRegion(Halfedge *he, Site *bottomMostSite);
			std::vector<Point*> *neighborSitesForSite(Point *coord);
			std::vector<Point*> *region(Point *p);
			std::vector< std::vector<Point*> *> *regions();
			std::vector<Point*> *siteCoords();
			std::vector<LineSegment*> *voronoiBoundaryForSite(Point *coord);
			std::vector<LineSegment*> *voronoiDiagram();
		};
	}//end namespace Voronoi
}//end namespace Nimbus
#endif