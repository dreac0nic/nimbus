#include "Positional.h"

using namespace Nimbus;

Nimbus::Positional::Positional(World* world):
	Behaviour(world)
{
}

Nimbus::Positional::Positional(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(world, intializingSettings)
{
}

Nimbus::Positional::~Positional(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::Positional::startup(void)
{
	// STARTUP FOR POSITIONAL
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Nimbus::Positional::update(void)
{
	// UPDATE THE POSITIONAL
	/*
		POSITION THINGS YEAH
	*/
}

void Nimbus::Positional::shutdown(void)
{
	// SUT DOWN THE POSITIONAL STUFF
	/*
		YARHARFIDDLEEDEE
	*/
}

Behaviour* Nimbus::Positional::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Positional(this->mWorld, initializingSettings);
}