#include "FlockSoaring.h"

using namespace Nimbus;

Nimbus::FlockSoaring::FlockSoaring(World* world):
	Soaring(world)
{
}

Nimbus::FlockSoaring::FlockSoaring(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
}

Nimbus::FlockSoaring::~FlockSoaring(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::FlockSoaring::startup(void)
{
	// STARTUP FOR FLOCK SOARING
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Nimbus::FlockSoaring::update(void)
{
	// UPDATE THE FLOCK SOARING
	/*
		Update the soaring entity. Weeeee.
	*/
}

void Nimbus::FlockSoaring::shutdown(void)
{
	// SUT DOWN THE FLOCK SOARING
	/*
		LOLWUT
	*/
}

Behaviour* Nimbus::FlockSoaring::clone(ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::FlockSoaring(this->mWorld, initializingSettings);
}