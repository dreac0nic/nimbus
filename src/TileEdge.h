typedef Ogre::Vector2 Point;

class TileEdge{
public:
	int index;
	Tile *d0, *d1;
	Corner *v0, *v1;
	Point *midpoint;
	int river;

	void setVoronoi(Corner *c0, Corner *c1);
	void toString();
};