#include "TestMode.h"

#include <math.h>

#include <OgreRoot.h>
#include <OgreVector2.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreColourValue.h>

#include <OGRE\OgreMesh.h>
#include <OGRE\OgreSubMesh.h>
#include <OGRE\OgreMeshManager.h>
#include <OGRE\OgreHardwareBufferManager.h>
#include <OGRE\OgreResourceGroupManager.h>

#include "NimbusApplication.h"

#include "Map.h"
#include "Corner.h"
#include "Tile.h"

using namespace Nimbus;
using namespace Ogre;

TestMode::TestMode(void)
{
}

TestMode::~TestMode(void)
{
}

RunMode* TestMode::run(const FrameEvent& evt)
{
	// Attempt to initialize the run mode
	if(!this->initialized && !this->initialize())
	{
		LogManager::getSingletonPtr()->logMessage("(Nimbus) Failed to initialize TestMode");

		// Terminate the application if we fail to initialize
		return 0;
	}

	// Continue to run this runmode
	return this;
}

bool TestMode::initialize()
{
	Entity* dragon;
	SceneNode* dragonNode;

	Light* light;

	// Create the scene manager
	mSceneMgr = Root::getSingleton().createSceneManager("DefaultSceneManager");

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	//////////
	// Set up the camera

	// Position the camera
	mCamera->setPosition(Vector3(0, 50, 80));
	mCamera->lookAt(Vector3(0, 0, -100));
	mCamera->setNearClipDistance(5);

	// Add a viewport for the camera
	mViewport = NimbusApplication::getRenderWindow()->addViewport(mCamera);

	mViewport->setBackgroundColour(ColourValue(0.5f, 0.2f, 0.3f, 1.0f));

	//////////
	// Set up the appropriate models

	// Correct the aspect ratio of the camera
	mCamera->setAspectRatio(
		Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	mCamera->setPolygonMode(Ogre::PM_WIREFRAME);

	// Load the dragon
	dragon = mSceneMgr->createEntity("Dragon", "dragon.mesh");
	dragonNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	// dragonNode->attachObject(dragon);

	dragonNode->setPosition(0, 0, -100);
	dragonNode->setScale(40.0, 40.0, 40.0);
	dragonNode->pitch(Degree(90));

	//////////
	// Set up light sources

	// Set the ambient light
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	// Create a point light for main light
	light = mSceneMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
	// Generate some fake map information and throw it into the scene!
	const int pointCount = 6;
	const float lPI = 3.14159f;
	const float tileScalar = 10.0f;
	Tile testTile;

	Corner* corn1 = new Corner();
	Corner* corn2 = new Corner();
	Corner* corn3 = new Corner();
	Corner* corn4 = new Corner();
	Corner* corn5 = new Corner();
	Corner* corn6 = new Corner();

	corn1->loc = new Ogre::Vector2(0.0f, 1.0f);
	corn1->elevation = 0.5f;
	corn2->loc = new Ogre::Vector2(-1.0f, 1.0f);
	corn2->elevation = -0.5f;
	corn3->loc = new Ogre::Vector2(-1.0f, 0.0f);
	corn3->elevation = 0.2f;
	corn4->loc = new Ogre::Vector2(0.0f, -1.0f);
	corn4->elevation = 0.0f;
	corn5->loc = new Ogre::Vector2(1.0f, -1.0f);
	corn5->elevation = 0.2f;
	corn6->loc = new Ogre::Vector2(1.5f, 0.2f);
	corn6->elevation = 0.125f;

	testTile.corners.push_back(corn1);
	testTile.corners.push_back(corn2);
	testTile.corners.push_back(corn3);
	testTile.corners.push_back(corn5);
	testTile.corners.push_back(corn4);
	testTile.corners.push_back(corn6);

	testTile.loc = Ogre::Vector2(0.0f, 0.0f);
	testTile.elevation = 0.0f;

	// Create material
	MaterialPtr material = MaterialManager::getSingleton().create(
		"Tiles/Default",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	material->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_AMBIENT);

	// Create entity and add to the scene.
	Entity* tileEntity = mSceneMgr->createEntity("testTile", testTile.getMesh());
	SceneNode* tileNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	std::cout << "Blah!" << std::endl;

	tileEntity->setMaterialName("Tiles/Default");

	tileNode->attachObject(tileEntity);

	tileNode->setPosition(0, 0, -50);
	tileNode->setScale(10.0f, 10.0f, 10.0f);

	// BLARG TESTING
	    /// Create the mesh via the MeshManager
    Ogre::MeshPtr msh = MeshManager::getSingleton().createManual("ColourCube", "General");
 
    /// Create one submesh
    SubMesh* sub = msh->createSubMesh();
 
    const float sqrt13 = 0.577350269f; /* sqrt(1/3) */
 
    /// Define the vertices (8 vertices, each consisting of 2 groups of 3 floats
    const size_t nVertices = 6;
    const size_t vbufCount = 3*2*nVertices;
    float vertices[vbufCount] = {
    		testTile.loc.x, 0.0f, testTile.loc.y,
    		0.0f,1.0f,0.0f,
            corn1->loc->x,0.0f,corn1->loc->y,        //0 position
            0.0f,1.0f,0.0f,     //0 normal
            corn2->loc->x,0.0f,corn2->loc->y,         //1 position
            0.0f,1.0f,0.0f,      //1 normal
            corn3->loc->x,0.0f,corn3->loc->y,        //2 position
            0.0f,1.0f,0.0f,     //2 normal
            corn4->loc->x,0.0f,corn4->loc->y,       //3 position
            0.0f,1.0f,0.0f,    //3 normal
            corn5->loc->x,0.0f,corn5->loc->y,         //4 position
            0.0f,1.0f,0.0f,      //4 normal
    };
 
    RenderSystem* rs = Root::getSingleton().getRenderSystem();
    RGBA colours[nVertices];
    RGBA *pColour = colours;
    // Use render system to convert colour value since colour packing varies
    rs->convertColourValue(ColourValue(1.0,0.0,0.0), pColour++); //0 colour
    rs->convertColourValue(ColourValue(1.0,1.0,0.0), pColour++); //1 colour
    rs->convertColourValue(ColourValue(0.0,1.0,0.0), pColour++); //2 colour
    rs->convertColourValue(ColourValue(0.0,0.0,0.0), pColour++); //3 colour
    rs->convertColourValue(ColourValue(1.0,0.0,1.0), pColour++); //4 colour
    rs->convertColourValue(ColourValue(0.0,1.0,1.0), pColour++); //4 colour
 
    /// Define 12 triangles (two triangles per cube face)
    /// The values in this table refer to vertices in the above table
    const size_t ibufCount = 15;
    unsigned short faces[ibufCount] = {
            0,2,1,
            0,3,2,
            0,4,3,
            0,5,4,
            0,1,5
    };
 
    /// Create vertex data structure for 8 vertices shared between submeshes
    msh->sharedVertexData = new VertexData();
    msh->sharedVertexData->vertexCount = nVertices;
 
    /// Create declaration (memory format) of vertex data
    VertexDeclaration* decl = msh->sharedVertexData->vertexDeclaration;
    size_t offset = 0;
    // 1st buffer
    decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    HardwareVertexBufferSharedPtr vbuf = 
        HardwareBufferManager::getSingleton().createVertexBuffer(
        offset, msh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);
 
    /// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
    VertexBufferBinding* bind = msh->sharedVertexData->vertexBufferBinding; 
    bind->setBinding(0, vbuf);
 
    // 2nd buffer
    offset = 0;
    decl->addElement(1, offset, VET_COLOUR, VES_DIFFUSE);
    offset += VertexElement::getTypeSize(VET_COLOUR);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
        offset, msh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), colours, true);
 
    /// Set vertex buffer binding so buffer 1 is bound to our colour buffer
    bind->setBinding(1, vbuf);
 
    /// Allocate index buffer of the requested number of vertices (ibufCount) 
    HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().
        createIndexBuffer(
        HardwareIndexBuffer::IT_16BIT, 
        ibufCount, 
        HardwareBuffer::HBU_STATIC_WRITE_ONLY);
 
    /// Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);
 
    /// Set parameters of the submesh
    sub->useSharedVertices = true;
    sub->indexData->indexBuffer = ibuf;
    sub->indexData->indexCount = ibufCount;
    sub->indexData->indexStart = 0;
 
    /// Set bounding information (for culling)
    msh->_setBounds(AxisAlignedBox(-100,-100,-100,100,100,100));
    msh->_setBoundingSphereRadius(Math::Sqrt(3*100*100));
 
    /// Notify -Mesh object that it has been loaded
    msh->load();

    MaterialPtr materialThing = MaterialManager::getSingleton().create("Test/ColourTest", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	materialThing->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_AMBIENT);

	Entity* thisEntity = mSceneMgr->createEntity("cc", "ColourCube");
	thisEntity->setMaterialName("Test/ColourTest");
	SceneNode* thisSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	thisSceneNode->setPosition(0, 0, -100);
	thisSceneNode->setScale(10.0f, 10.0f, 10.0f);
	thisSceneNode->yaw(Ogre::Radian(90.0f));
	thisSceneNode->attachObject(thisEntity);

	// Note that the RunMode has been initialized
	this->initialized = true;
	return true;
}
