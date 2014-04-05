#ifndef VORONOI_H
#define VORONOI_H

#include <OgreVector2.h>
#include "OGRE\OgreVector2.h"
#include <stack>
#include <utility>
#include <stdlib.h>

namespace Nimbus
{
	namespace Voronoi
	{
		//numbers are testing priority ratings (High numbers done first)
		class Rectangle;//tested
		class Edge;//3
		class Halfedge;//5
		class LineSegment;//tested
		class Polygon;//tested
		class Site;//2
		class Vertex;//4
		class Voronoi;//1

		enum Winding {CLOCKWISE, COUNTERCLOCKWISE, NONE};
		enum LR {LR_LEFT, LR_RIGHT};
		enum BoundsCheck {TOP, BOTTOM, LEFT, RIGHT};

		typedef Ogre::Vector2 Point;

		class Util{
		public:
			static bool closeEnough(double d1, double d2, double diff);
		};

		/*The Voronoi object is the only map generation object that should interact with outside classes.

		This object holds all of the information pertaining to a generated map.  After the info is copied
		out of the class, Voronoi and all of the objects it creates are to be garbage collected, leaving
		the map information in other options.
		*/
		class Voronoi
		{
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