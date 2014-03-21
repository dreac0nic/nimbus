#include "Renderable.h"
#include <OgreSceneManager.h>

using namespace Ogre;
using namespace Nimbus;

Nimbus::Renderable::Renderable(BehaviourType type, World* world):
	Behaviour(type, world)
{
	this->init(NULL, Vector3::ZERO);
}

Nimbus::Renderable::Renderable(BehaviourType type, World* world, ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world)
{
	std::string ogreName;

	// The way this works now is a bit different... so consider changing this.
	if(initializingSettings->find("ogreName") != initializingSettings->end())
	{
		ogreName = initializingSettings->find("ogreName")->second;
	}

	// Grabbing the actual mesh name
	if(initializingSettings->find("model") != initializingSettings->end())
	{
		std::string modelPath = initializingSettings->find("model")->second;
		std::stringstream nameStream;
		nameStream << ogreName << this->mWorld->getCurrentId();
		this->mModel = this->mWorld->getSceneManager()->createEntity(nameStream.str(), modelPath);
	}

	// Grabbing the initial position
	if(initializingSettings->find("position") != initializingSettings->end())
	{
		std::string positions = (*(initializingSettings->find("position"))).second;
		std::stringstream positionStream;
		positionStream << positions;
		Ogre::Real x, y, z;
		positionStream >> x >> y >> z;
		this->mPosition = Ogre::Vector3(x, y, z);
	}

	// Grabbing the initial position
	if(initializingSettings->find("scale") != initializingSettings->end())
	{
		std::string scales = (*(initializingSettings->find("scale"))).second;
		std::stringstream scaleStream;
		scaleStream << scales;
		Ogre::Real x, y, z;
		scaleStream >> x >> y >> z;
		this->mScale = Ogre::Vector3(x, y, z);
	}

	// Grabbing the initial position
	if(initializingSettings->find("rotation") != initializingSettings->end())
	{
		std::string rotations = (*(initializingSettings->find("rotation"))).second;
		std::stringstream rotationStream;
		rotationStream << rotations;
		Ogre::Real x, y, z;
		rotationStream >> x >> y >> z;
		this->mRotation = Ogre::Vector3(x, y, z);
	}

	// Initialize the object
	this->init(model, scale);
}

Nimbus::Renderable::Renderable(Renderable* other, World* world, int id):
	Behaviour(other, world, id)
{
	this->init(other->getModel(), other->getScale());
}

Nimbus::Renderable::~Renderable()
{
	// Empty destructor
}

void Nimbus::Renderable::init(Ogre::Entity* model, Vector3 scale)
{
	this->mModel = model;
	this->mScale = scale;

	this->mParentId;

	this->mPositionListener = new PositionListener(this);
}

void Nimbus::Renderable::startup(void)
{
	this->mNode = mWorld->getWorldNode()->createChildSceneNode();
	this->mNode->attachObject(this->mModel);

	// Setting initial properties
	this->mNode->setScale(mScale.x, mScale.y, mScale.z);
}

void Nimbus::Renderable::update(void)
{
	//this->mNode->yaw(Degree(0.5), Node::TS_WORLD);
}

void Nimbus::Renderable::shutdown(void)
{
	// SHUTDOWN FOR RENDERABLE
	
	// Detatch the scene node
	mWorld->getWorldNode()->removeChild(this->mNode->getName());
}

Ogre::Entity* Nimbus::Renderable::getModel()
{
	return this->mModel;
}

void Nimbus::Renderable::setModel(Ogre::Entity* model)
{
	this->mModel = model;
}

Ogre::Vector3 Nimbus::Renderable::getScale()
{
	return this->mScale;
}

Behaviour* Nimbus::Renderable::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Renderable(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Nimbus::Renderable::clone(int id)
{
	return new Nimbus::Renderable(this, this->mWorld, id);
}

{
}