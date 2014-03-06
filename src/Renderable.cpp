#include "Renderable.h"
#include <OgreSceneManager.h>

using namespace Ogre;
using namespace Nimbus;

Nimbus::Renderable::Renderable(World* world):
	Behaviour(world)
{
	this->mWorld = world;
}

Nimbus::Renderable::Renderable(World* world, ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(world, initializingSettings)
{
	std::string ogreName;
	this->mWorld = world;

	// The way this works now is a bit different... so consider changing this.
	if(initializingSettings->find("ogreName") != initializingSettings->end())
	{
		ogreName = (*(initializingSettings->find("ogreName"))).second;
	}

	// Grabbing the actual mesh name
	if(initializingSettings->find("model") != initializingSettings->end())
	{
		std::string modelPath = (*(initializingSettings->find("model"))).second;
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
}

Nimbus::Renderable::Renderable(Nimbus::Renderable* other, World* world):
	Behaviour(other, world)
{
	this->mWorld = world;
	this->mModel = other->getEntity();
	this->mPosition = other->getPosition();
	this->mScale = other->getScale();
	this->mRotation = other->getRotation();
}

Nimbus::Renderable::~Renderable()
{
	// Empty destructor
}

void Nimbus::Renderable::startup(void)
{
	// STARTUP FOR RENDERABLE
	/*
		Initialize all the various renderable stuff for
		Ogre and meshes and scenegraphing and yeah.
	*/
	Ogre::SceneNode* node = mWorld->getWorldNode()->createChildSceneNode();
	node->attachObject(this->mModel);

	// Setting initial properties
	node->setPosition(mPosition.x, mPosition.x, mPosition.z);
	node->setScale(mScale.x, mScale.y, mScale.z);
	node->pitch(Degree(mRotation.x));
	node->yaw(Degree(mRotation.y));
	node->roll(Degree(mRotation.z));
}

void Nimbus::Renderable::update(void)
{
	// UPDATE FOR RENDERABLE
	/*
		Update rendering information.
	*/
}

void Nimbus::Renderable::shutdown(void)
{
	// SHUTDOWN FOR RENDERABLE
	/*
		Deinitialize all the renderable stuff.
	*/
}

Ogre::Entity* Nimbus::Renderable::getEntity()
{
	return this->mModel;
}

Ogre::Vector3 Nimbus::Renderable::getPosition()
{
	return this->mPosition;
}

Ogre::Vector3 Nimbus::Renderable::getScale()
{
	return this->mScale;
}

Ogre::Vector3 Nimbus::Renderable::getRotation()
{
	return this->mRotation;
}

Behaviour* Nimbus::Renderable::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Renderable(this->mWorld, initializingSettings);
}

Behaviour* Nimbus::Renderable::clone()
{
	return new Nimbus::Renderable(this, this->mWorld);
}