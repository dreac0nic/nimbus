#include "Tile.h"
#include "Corner.h"

#include <OGRE\OgreMeshManager.h>

using namespace Nimbus;
using namespace Ogre;

int Tile::deltaX = 0;
int Tile::deltaY = 0;

Tile::Tile()
{
}

Tile::Tile(Point *loc)
{
	this->loc = *loc;
}

Tile::~Tile(void)
{
}

void Tile::calculateBox(void)
{
	for (unsigned int i = 0; i < corners.size(); i++){
		double x = std::abs(corners.at(i)->loc->x - loc.x);
		if(x > deltaX){
			deltaX = x; // DOUBLE -> INT POSSIBLE LOSS OF DATA
		}

		double y = std::abs(corners.at(i)->loc->y - loc.y);
		if(y > deltaY){
			deltaY = y; // DOUBLE -> INT POSSIBLE LOSS OF DATA
		}
	}
}

void Tile::toString(void)
{
	std::cout << "Tile, x:" << loc.x << ", y:" << loc.y << " " << (border? "b": "-") << (ocean? "o": "-") << (water? "w": "-") << (coast? "c": "-") << "\n";
}

MeshPtr Tile::getMesh(void)
{
	// Create the overall mesh.
	MeshPtr tileMesh = MeshManager::getSingleton().createManual("tileName", "mapTiles"); // Fix name issues. tileName not repeatable.

	// Create the submesh for this mesh.
	SubMesh* mesh = tileMesh->createSubMesh();

	// Define the vertices
	size_t index = 0;
	size_t vertCount = this->corners.size() + 1; // number of corners + cent

	Real* vertices = new Real[vertCount*3*2]; // vertCount * 3 [x, y, z] * 2 [coord, normal]

	// Add center to first vector set.
	vertices[index++] = this->loc.x;
	vertices[index++] = this->elevation;
	vertices[index++] = this->loc.y;

	// Add center normal to second vector second.
	Vector3 norm = Vector3::UNIT_Y;

	vertices[index++] = norm.x;
	vertices[index++] = norm.y;
	vertices[index++] = norm.z;

	for(vector<Corner*>::iterator it = this->corners.begin(); it != this->corners.end(); ++it) {
		// Add to the next point to the array.
		Vector3 vec = (*it)->getVector3();

		vertices[index++] = vec.x;
		vertices[index++] = vec.y;
		vertices[index++] = vec.z;

		// Add the normal for the next point to the array.
		Vector3 norm = Vector3::UNIT_Y; // Fake normal vector.

		vertices[index++] = norm.x;
		vertices[index++] = norm.y;
		vertices[index++] = norm.z;
	}

	// Define the triangles
	int faceCount = vertCount - 1; // Face count = vertCount - cent;
	int center = 0;
	int last = 1;
	int curr = 2;

	unsigned short* faces = new unsigned short[faceCount*3];

	for(size_t i = 0; i < faceCount; ++i) {
		assert(last < vertCount && curr < vertCount); // Panic check.

		faces[i]   = center;
		faces[++i] = curr;
		faces[++i] = last;

		last = curr++; // Move along the array.
	}

	// Deallocate the vertices and faces arrays.
	delete[] vertices;
	delete[] faces;

	// Return finished mesh.
	return tileMesh;
}