#include "Renderable.h"

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
	this->mBehaviourType = (*(initializingSettings->find("name"))).second;
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

Behaviour* Nimbus::Renderable::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Renderable(this->mWorld, initializingSettings);
}