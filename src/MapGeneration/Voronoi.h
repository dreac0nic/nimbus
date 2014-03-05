#ifndef VORONOI_H
#define VORONOI_H

#include <OgreVector2.h>
#include <stack>
#include <utility>

namespace Nimbus{
	namespace Voronoi{
		class Rectangle;
		class Edge;
		class Halfedge;
		class LineSegment;
		class Polygon;
		class Site;
		class Vertex;
		class Voronoi;

		enum Winding{CLOCKWISE, COUNTERCLOCKWISE, NONE};
		enum LR{LR_LEFT, LR_RIGHT};
		enum BoundsCheck{TOP, BOTTOM, LEFT, RIGHT};

		typedef Ogre::Vector2 Point;

		/*Helper class for bounding box.
		
		Capable of checking whether a point is inside a box.
		*/
		class Rectangle{
		public:
			float x, y, width, height, right, bottom, left, top;
			Rectangle(float x, float y, float width, float height);
			bool liesOnAxes(Point *p);
			bool inBounds(Point *p);
			bool inBounds(float x0, float y0);
		};

		/*Edges exist between (bisect) two Sites and between (bisect) two Vertices.

		Edges can only be created by extrapolating from two Sites.
		*/
		class Edge{
		private:
			//Edge related
			static int _numEdges;
			static std::stack<Edge*> *_pool;
			static Edge *create();

			std::map<LR, Point*> *_clippedVertices;
			std::map<LR, Site*> *_sites;
			int _edgeIndex;
			Vertex *_leftVertex;
			Vertex *_rightVertex;
			void init();
			Edge();
			Edge *createBisectingEdge(Site *site0, Site *site1);

			//Edge list
			float _deltax;
			float _xmin;
			int _hashSize;
			std::vector<Halfedge*> *_hash;
			Halfedge *getHash(int b);

			//Edge sorting
			std::vector<Edge*> *_edges;
			std::vector<LR*> *_edgeOrientations;

		public:
			//Edge related
			static const Edge DELETED;

			float a, b, c;
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
			float sitesDistance();
			Vertex *vertex(LR leftRight);
			LineSegment *voronoiEdge();

			//Edge list
			Halfedge *_leftEnd;
			Halfedge *_rightEnd;
			void initList(float xmin, float deltax, int sqrt_nsites);
			void insert(Halfedge *lb, Halfedge *newHalfedge);
			void remove(Halfedge *halfEdge);
			Halfedge *edgeListLeftNeighbor(Point *p);

			//Edge sorting
			std::vector<Edge*> *getEdges();
			std::vector<LR> *getEdgeOrientations();
			std::vector<Edge*> *reorderBySite(std::vector<Edge> *origEdges);
			std::vector<Edge*> *reorderByVertex(std::vector<Edge> *origEdges);
		};

		/*Each Edge has two Halfedges (right and left).
		
		Halfedges are useful in creating Corners where Edges meet.
		*/
		class Halfedge{
		private:
			static std::stack<Halfedge*> _pool;
			static std::vector<Halfedge*> _hash;
			static int _count;
			static int _hashSize;
			static float _ymin;
			static float _deltay;
			static bool isEmpty();
			static void adjustMinBucket();

			Halfedge *init(Edge *edge, LR lr);

		public:
			static void initQueue(float ymin, float deltay, int sqrt_nsites);
			static void disposeQueue();
			static void insert(Halfedge *halfedge);
			static void remove(Halfedge *halfedge);
			static int bucket(Halfedge *halfedge);
			static bool empty();
			static Point *min();
			static Halfedge *extractMin();

			Halfedge *edgeListLeftNeighbor;
			Halfedge *edgeListRightNeighbor;
			Halfedge *nextInPriorityQueue;
			Edge edge;
			LR leftRight;
			Vertex vertex;
			float ystar;
			Halfedge(Edge *edge, LR lr);
			~Halfedge();
			static Halfedge *create(Edge *edge, LR lr);
			static Halfedge *createDummy();
			bool isLeftOf(Point *p);
		};

		class LineSegment{
		public:
			std::pair<Point, Point> points;

			~LineSegment();
			LineSegment(std::pair <Point*, Point*> *points);
			LineSegment(Point *point0, Point *point1);
			static float compareLengthsMax(LineSegment *segment0, LineSegment *segment1);
			static float compareLengths(LineSegment *edge0, LineSegment *edge1);
		};

		class Polygon{
		private:
			float signedDoubleArea();
			std::vector<Point> _vertices;
		public:
			Polygon(std::vector<Point*> *vertices);
			~Polygon();
			float area();
			Winding winding();

		};

		 /*Sites equate to the contents of a tile.
		 
		 The center of a site is a random point, and the interior contains all points closer to that vertex
		 than any other.  The edges of each site are formed along the perpendicular bisector between two
		 neighboring vertices.
		 */
		class Site{
		private:
			static std::stack<Site> _pool;
			static Site *create(Point *p, int index, float weight);
			static void sortSites(std::vector<Site> *sites);
			static float compare(Site *s1, Site *s2);
			static bool closeEnough(Point *p0, Point *p1);
			static int check(Point *point, Rectangle *bounds);

			Point _coord;
			int _siteIndex;
			std::vector<LR> *_edgeOrientations;
			std::vector<Point> *_region;
			void clear();
			void reorderEdges();
			std::vector<Point> *clipToBounds(Rectangle *bounds);
			void connect(std::vector<Point> *points, int j, Rectangle *bounds, bool closingUp);

		public:
			float weight;
			std::vector<Edge> _edges;
			Site(Point *p, int index, float weight);
			~Site();
			Point *getCoord();
			void addEdge(Edge *edge);
			Edge *nearestEdge();
			std::vector<Site> *neighborSites();
			Site *neighborSite(Edge *edge);
			std::vector<Point> *region(Rectangle *clippingbounds);
			float getX();
			float getY();
			float dist(Point *p);

		};

		/*Provides a way to create points at the intersection of two edges.

		Tracks the total number of vertices and stores them by index.
		*/
		class Vertex{
		private:
			static std::stack<Vertex> _pool;
			static int _numVertices;
			Point _coord;
			int _vertIndex;

			static Vertex *create(float x, float y);
			Vertex(float x, float y);
		public:
			~Vertex();

			Point getCoord();
			int getVertexIndex();
			float getX();
			float getY();

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
			std::vector<Edge> _edges;
			Rectangle _plotBounds;
			std::map<Point, Site> _sitesIndexedByLocation;
			void addSites(std::vector<Point> *points);
			void addSite(Point *p, int index);
			std::vector<LineSegment> *delaunayLinesForSite(Point *coord);
			void fortunesAlgorithm();
			std::vector<Edge> *hullEdges();
			void init(std::vector<Point> *points, Rectangle *plotBounds);
			std::vector<Edge> *selectEdgesForSitePoint(Point *coord, std::vector<Edge> *edgesToTest);
			std::vector<LineSegment> *visibleLineSegments(std::vector<Edge> *edgesToTest);

		public:
			static int compareByYThenX(Site *s1, Point *p1);
			static int compareByXThenY(Site *s1, Point *p1);

			Voronoi(std::vector<Point> *points, Rectangle *plotBoutnds);
			Voronoi(std::vector<Point> *points);
			Voronoi(int numSites, float maxWidth, float maxHeight);
			~Voronoi();
			std::vector<Edge> *edges();
			Rectangle *getPlotBounds();
			std::vector<LineSegment> *hull();
			std::vector<Point> *hullPointsInOrder();
			Site *leftRegion(Halfedge *he, Site *bottomMostSite);
			Site *rightRegtion(Halfedge *he, Site *bottomMostSite);
			std::vector<Point> *neighborSitesForSite(Point *coord);
			std::vector<Point> *region(Point *p);
			std::vector< std::vector<Point> > *regions();
			std::vector<Point> *siteCoords();
			std::vector<LineSegment> *voronoiBoundaryForSite(Point *coord);
			std::vector<LineSegment> *voronoiDiagram();
		};
	}//end namespace Voronoi
}//end namespace Nimbus
#endif