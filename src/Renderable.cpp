#include "Renderable.h"

using namespace Nimbus;

Renderable::Renderable(World* world):
	Behaviour(world)
{
}

Renderable::Renderable(World* world, Ogre::ConfigFile::SettingsMultiMap* intializingSettings):
	Behaviour(world, intializingSettings)
{
}

Renderable::~Renderable()
{
}

void Renderable::startup(void)
{
	// STARTUP FOR RENDERABLE
	/*
		Initialize all the various renderable stuff for
		Ogre and meshes and scenegraphing and yeah.
	*/
}

void Renderable::update(void)
{
	// UPDATE FOR RENDERABLE
	/*
		Update rendering information.
	*/
}

void Renderable::shutdown(void)
{
	// SHUTDOWN FOR RENDERABLE
	/*
		Deinitialize all the renderable stuff.
	*/
}