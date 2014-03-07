#include "Soaring.h"

using namespace Nimbus;

Nimbus::Soaring::Soaring(World* world):
	Positional(world)
{
}

Nimbus::Soaring::Soaring(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Positional(world, initializingSettings)
{
}

Nimbus::Soaring::~Soaring(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::Soaring::startup(void)
{
	// STARTUP FOR FLOCK SOARING
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Nimbus::Soaring::update(void)
{
	// UPDATE THE FLOCK SOARING
	/*
		Update the soaring entity. Weeeee.
	*/
}

void Nimbus::Soaring::shutdown(void)
{
	// SUT DOWN THE FLOCK SOARING
	/*
		LOLWUT
	*/
}

Behaviour* Nimbus::Soaring::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Soaring(this->mWorld, initializingSettings);
}