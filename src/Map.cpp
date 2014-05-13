#include <vector>

#include <OGRE/OgreMesh.h>
#include <OGRE/OgreSubMesh.h>
#include <OGRE/OgreMeshManager.h>

#include "Map.h"
#include "Tile.h"
#include "Corner.h"

#define PI 3.1415927

using namespace Nimbus;
using namespace Ogre;

Map::Map(void)
{
	// Generation parameters.
	size_t numberOfPoints = 20;

	// Origin to offset the entire map.
	Vector3 origin = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 spacing = Vector3(1.0f, 1.0, 1.0f);
	Vector3 cornerSpacing = spacing/2;
	
	Vector3 midOffset = (spacing*(Ogre::Real)numberOfPoints)/2.0f;
	midOffset.y = 0.0f;

	// Generate the points.
	for(size_t i = 0; i < numberOfPoints; ++i) {
		for(size_t j = 0; j < numberOfPoints; ++j) {
			// Create the tile information ...
			Vector3 localOrigin(origin + Vector3(spacing.x*i, spacing.y, spacing.z*j) - midOffset);

			std::vector<Corner*> tempCorners;

			Corner* temp = new Corner();

			// Create the corners of the tile.
			temp->loc = new Ogre::Vector2(localOrigin.x + cornerSpacing.x, localOrigin.z + cornerSpacing.z);
			temp->elevation = localOrigin.y;
			tempCorners.push_back(temp);

			temp = new Corner();
			temp->loc = new Ogre::Vector2(localOrigin.x - cornerSpacing.x, localOrigin.z + cornerSpacing.z);
			temp->elevation = localOrigin.y;
			tempCorners.push_back(temp);

			temp = new Corner();
			temp->loc = new Ogre::Vector2(localOrigin.x - cornerSpacing.x, localOrigin.z - cornerSpacing.z);
			temp->elevation = localOrigin.y;
			tempCorners.push_back(temp);

			temp = new Corner();
			temp->loc = new Ogre::Vector2(localOrigin.x + cornerSpacing.x, localOrigin.z - cornerSpacing.z);
			temp->elevation = localOrigin.y;
			tempCorners.push_back(temp);

			// Create a new tile.
			Tile* currentTile = new Tile();

			// Set the tile's center position and elevation.
			currentTile->setPosition(localOrigin.x, localOrigin.z);
			currentTile->setElevation(localOrigin.y);

			// Add the corners to the tile.
			currentTile->setCorners(tempCorners);

			// Synthesize values for tile.

			// Add the tile to the map.
			this->tiles.push_back(currentTile);
		}
	}
}

Ogre::MeshPtr Map::getMesh(std::string meshName)
{
	// Create overall mesh.
	Ogre::MeshPtr mapMesh = Ogre::MeshManager::getSingleton().createManual(meshName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); // Watch out for collision issues.

	// Iterate over all tiles and add them to the mapMesh.
	for(std::vector<Tile*>::iterator it = this->tiles.begin(); it != this->tiles.end(); ++it) {
		(*it)->addSubMesh(mapMesh);
	}

	// Set bounding information (for culling)
    mapMesh->_setBounds(AxisAlignedBox(-5,-5,-5,5,5,5));
    mapMesh->_setBoundingSphereRadius(Math::Sqrt(3*5*5));

    // Signal the finalization of the mesh.
    mapMesh->load();

	// Return the mapMesh
	return mapMesh;
}
