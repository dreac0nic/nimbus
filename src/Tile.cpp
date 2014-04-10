#include "Tile.h"
#include "Corner.h"

#include <OGRE\OgreRoot.h>
#include <OGRE\OgreMesh.h>
#include <OGRE\OgreSubMesh.h>
#include <OGRE\OgreMeshManager.h>
#include <OGRE\OgreHardwareBufferManager.h>
#include <OGRE\OgreResourceGroupManager.h>

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

void Tile::_generateSubMesh(MeshPtr& mesh)
{
	// Create a submesh to the given mesh.
	SubMesh* tileMesh = mesh->createSubMesh();

	// Define the vertices.
	size_t index = 0;
	size_t vertCount = this->corners.size() + 1; // corner count + center corner

	Real* vertices = new Real[vertCount*3*2];    // (number of verts)*(x, y, z)*(coord, normal) -or- vertCount*3*2

	// Manually add center vertex.
	// -- Position (ord: x, y, z)
	vertices[index++] = this->loc.x;
	vertices[index++] = this->elevation;
	vertices[index++] = this->loc.y;

	// -- Normal (ord: x, y, z)
	Vector3 norm = Vector3::UNIT_Y;

	vertices[index++] = norm.x;
	vertices[index++] = norm.y;
	vertices[index++] = norm.z;

	// Add the rest of the vertices to data buffer.
	for(vector<Corner*>::iterator it = this->corners.begin(); it != this->corners.end(); ++it) {
		// Add to the next point to the array.
		// -- Position
		Vector3 vector = (*it)->getVector3();

		vertices[index++] = vector.x;
		vertices[index++] = vector.y;
		vertices[index++] = vector.z;

		// -- Normal
		Vector3 normal = Vector3::UNIT_Y;

		vertices[index++] = normal.x;
		vertices[index++] = normal.y;
		vertices[index++] = normal.z;
	}

	// Define vertices color.
	RenderSystem* rs = Root::getSingleton().getRenderSystem();
	RGBA* colors = new RGBA[vertCount];

	for(size_t i = 0; i < vertCount; ++i)
		rs->convertColourValue(ColourValue(0.0f + 0.175f*i, 0.2f, 1.0f - 0.175f*i), colors + i);

	// Define the triangles.
	size_t faceCount = vertCount - 1; // Face count = vertCount - cent

	size_t center = 0;
	size_t last   = 1; //collin was here
	size_t curr   = 2;

	unsigned short* faces = new unsigned short[faceCount*3];

	index = 0;

	for(size_t i = 0; i < faceCount; ++i) {
		assert(last < vertCount && curr < vertCount); // Panic check

		faces[index++] = center;
		faces[index++] = curr;
		faces[index++] = last;

		std::cout << "Faces: " << center << ", " << curr << ", " << last << std::endl;

		last = curr++;

		if(curr >= vertCount) curr = 1;
	}

	// All information has been generated, move into mesh structures.
	//   Note: Currently does not implement or used any sort of shared
	//     vertices. This is intentional and should be changed at the 
	//     soonest conveienence. IE -- Never. ;P
	tileMesh->useSharedVertices = false;
	tileMesh->vertexData = new VertexData();
	tileMesh->vertexData->vertexCount = vertCount;

	// Create memory footprint for vertex data.
	size_t offset = 0;
	VertexDeclaration* decl = tileMesh->vertexData->vertexDeclaration;

	// Position and normal buffer.
	// -- Position
	decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
	offset += VertexElement::getTypeSize(VET_FLOAT3);

	// -- Normal
	decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
	offset += VertexElement::getTypeSize(VET_FLOAT3);

	// Allocate a vertex buffer for a number of vertices and vertex size.
	HardwareVertexBufferSharedPtr vertBuff = 
		HardwareBufferManager::getSingleton().createVertexBuffer(
			offset, // Size of a vertex, in bytes.
			tileMesh->vertexData->vertexCount,
			HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Write our data to vertex buffer.
	vertBuff->writeData(0, vertBuff->getSizeInBytes(), vertices, true);

	// Set the buffer's bind location.
	VertexBufferBinding* vertBind = tileMesh->vertexData->vertexBufferBinding;
	vertBind->setBinding(0, vertBuff);

	// Color buffer for vertices
	offset = 0;
	decl->addElement(1, offset, VET_COLOUR, VES_DIFFUSE);
	offset += VertexElement::getTypeSize(VET_COLOUR);

	// Allocate a new buffer for colors.
	vertBuff = HardwareBufferManager::getSingleton().createVertexBuffer(
			offset, // Size of a vertex, in bytes.
			tileMesh->vertexData->vertexCount,
			HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Write color data to buffer.
	vertBuff->writeData(0, vertBuff->getSizeInBytes(), colors, true);

	// Set the color buffer's bind location
	vertBind->setBinding(1, vertBuff);

	// Allocate a buffer for the index information
	HardwareIndexBufferSharedPtr indexBuff = HardwareBufferManager::getSingleton().createIndexBuffer(
		HardwareIndexBuffer::IT_16BIT,
		faceCount*3,
		HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Write data to the buffer.
	indexBuff->writeData(0, indexBuff->getSizeInBytes(), faces, true);

	// Finalize submesh.
	tileMesh->indexData->indexBuffer = indexBuff;
	tileMesh->indexData->indexCount = faceCount*3;
	tileMesh->indexData->indexStart = 0;

	// Deallocate the vertex and face arrays.
	if(vertices) delete[] vertices;
	if(faces) delete[] faces;
}

MeshPtr Tile::getMesh(std::string tileName)
{
	// Create the overall mesh.
	MeshPtr tileMesh = MeshManager::getSingleton().createManual(tileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); // Fix name issues. tileName not repeatable.

	// Add the tile as a submesh to mesh major.
	this->_generateSubMesh(tileMesh);

    /// Set bounding information (for culling)
    tileMesh->_setBounds(AxisAlignedBox(-5,-5,-5,5,5,5));
    tileMesh->_setBoundingSphereRadius(Math::Sqrt(3*5*5));

	// Signal that the mesh has loaded.
	tileMesh->load();

	// Return finished mesh.
	return tileMesh;
}

bool Tile::addSubMesh(Ogre::MeshPtr& mesh)
{
	// Add the submesh using the internal function.
	this->_generateSubMesh(mesh);

	// Return true for operation successful!
	return true;
}