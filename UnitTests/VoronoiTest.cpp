//This file was written by Brady Wrench Steed to test classes in the Voronoi namespace.
//Use the Test Explorer window to run this code.

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Nimbus::Voronoi;

TEST_MODULE_INITIALIZE(ModuleStartup)
{
	Logger::WriteMessage(L"Starting Voronoi test...");
}

TEST_MODULE_CLEANUP(ModuleFinalize)
{
	Logger::WriteMessage(L"Finalizing Voronoi test.");
}

TEST_CLASS(UtilTest)
{
public:

	TEST_METHOD(closeEnoughTest)
	{
		Assert::IsTrue(Util::closeEnough(1.0, 1.5, .6));
		Assert::IsTrue(Util::closeEnough(1.0, 1.4, .6));
		Assert::IsFalse(Util::closeEnough(1.0, 1.7, .6));
	}

};

TEST_CLASS(RectangleTest)
{
public:

	TEST_METHOD(initialTest)
	{
		Rectangle rect(1.3, 4.0, 2.1, 3.0);
		Assert::IsTrue(Util::closeEnough(rect.left, 1.3, 0.0001));
		Assert::IsTrue(Util::closeEnough(rect.top, 4.0, 0.0001));
		Assert::IsTrue(Util::closeEnough(rect.right, 3.4, 0.0001));
		Assert::IsTrue(Util::closeEnough(rect.bottom, 7.0, 0.0001));
		Assert::IsTrue(Util::closeEnough(rect.width, 2.1, 0.0001));
		Assert::IsTrue(Util::closeEnough(rect.height, 3.0, 0.0001));
	}

	TEST_METHOD(liesOnAxesTest)
	{
		Rectangle rect(1.3, 4.0, 4.0, 4.0);
		Point p1= Point(1.4, 0);
		Assert::IsTrue(rect.liesOnAxes(&p1));
		p1= Point(5.2, 0);
		Assert::IsTrue(rect.liesOnAxes(&p1));
		p1= Point(0, 4.1);
		Assert::IsTrue(rect.liesOnAxes(&p1));
		p1= Point(0, 7.7);
		Assert::IsTrue(rect.liesOnAxes(&p1));
		p1= Point(2.4, 0);
		Assert::IsFalse(rect.liesOnAxes(&p1));
	}

	TEST_METHOD(inBoundsTest)
	{
		Rectangle rect(1.3, 4.0, 4.0, 4.0);
		Assert::IsTrue(rect.inBounds(2.5, 5.0));
		Assert::IsTrue(rect.inBounds(1.35, 5.0));
		Assert::IsFalse(rect.inBounds(1.25, 5.0));
		Assert::IsFalse(rect.inBounds(0, 5.0));
		Assert::IsFalse(rect.inBounds(2.5, 0));
		Assert::IsFalse(rect.inBounds(9, 5.0));
		Assert::IsFalse(rect.inBounds(2.5, 9));
	}
};

TEST_CLASS(EdgeTest)
{
	TEST_METHOD(creationTest)
	{
		Site site1 = Site(new Point(0,0), 0, 3.0);
		Site site2 = Site(new Point(0,2), 1, 3.0);
		Edge edge = *Edge::createBisectingEdge(&site1, &site2);
		Assert::IsTrue(&site1 == edge.getLeftSite());
		Assert::IsTrue(&site2 == edge.getRightSite());
	}
};

TEST_CLASS(PolygonTest)
{
	TEST_METHOD(windingTest)
	{
		std::vector<Point *> points;
		points.push_back(new Point(0,0));
		points.push_back(new Point(1,0));
		points.push_back(new Point(0,1));

		Assert::IsTrue((new Polygon(&points))->winding() == COUNTERCLOCKWISE);

		delete points.back();
		points.pop_back();

		points.push_back(new Point(0,-1));

		Assert::IsTrue((new Polygon(&points))->winding() == CLOCKWISE);

		delete points.back();
		points.pop_back();
		delete points.back();
		points.pop_back();
		delete points.back();
		points.pop_back();
	}
};

TEST_CLASS(LineSegmentTest)
{
	TEST_METHOD(initialTest)
	{
		Point p1(0, 0);
		Point p2(1, 0);

		LineSegment line1(&p1, &p2);

		Assert::IsTrue(line1.points.first == &p1);
		Assert::IsTrue(line1.points.second == &p2);
		Assert::IsFalse(line1.points.first == &p2);
		Assert::IsFalse(line1.points.second == &p1);
	}

	TEST_METHOD(compareTest)
	{
		Point p1(0, 0);
		Point p2(1, 0);
		Point p3(2, 0);

		LineSegment line1(&p1, &p2);
		LineSegment line2(&p1, &p3);
		Assert::IsTrue(Util::closeEnough(LineSegment::compareLengths(&line1, &line2), -1, 0.001));
		Assert::IsTrue(Util::closeEnough(LineSegment::compareLengths(&line2, &line1), 1, 0.001));
		Assert::IsTrue(Util::closeEnough(LineSegment::compareLengths(&line1, &line1), 0, 0.001));


		Assert::IsTrue(Util::closeEnough(LineSegment::compareLengthsMax(&line1, &line2), 1, 0.001));
		Assert::IsTrue(Util::closeEnough(LineSegment::compareLengthsMax(&line2, &line1), -1, 0.001));
		Assert::IsTrue(Util::closeEnough(LineSegment::compareLengthsMax(&line1, &line1), 0, 0.001));
	}
};

TEST_CLASS(HalfEdgeTest){
	TEST_METHOD(initialTest){
		Edge *edge1 = Edge::createBisectingEdge(new Site(new Point(0,1),0,0),new Site(new Point(1,1),0,0));
		Halfedge he1(edge1, LR_LEFT);

		Assert::IsTrue(edge1 == he1.edge);
	}


};