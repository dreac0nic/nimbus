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
	this->mWorld = world;

	// The way this works now is a bit different... so consider changing this.
	if(initializingSettings->find("name") != initializingSettings->end())
	{
		this->mBehaviourType = (*(initializingSettings->find("name"))).second;
	}

	// Grabbing the actual mesh name
	if(initializingSettings->find("model") != initializingSettings->end())
	{
		this->mModelPath = (*(initializingSettings->find("model"))).second;
		this->mModel = this->mWorld->getSceneManager()->createEntity("Dragon", this->mModelPath);
	}
}

Nimbus::Renderable::Renderable(Nimbus::Renderable* other, World* world):
	Behaviour(other, world)
{
	this->mBehaviourType = other->getBehaviourType();
	this->mWorld = world;
	this->mModel = other->getEntity();
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

Behaviour* Nimbus::Renderable::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Renderable(this->mWorld, initializingSettings);
}

Behaviour* Nimbus::Renderable::clone()
{
	return new Nimbus::Renderable(this, this->mWorld);
}