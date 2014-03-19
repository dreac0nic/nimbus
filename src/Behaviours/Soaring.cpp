#include "Soaring.h"

using namespace Nimbus;

Nimbus::Soaring::Soaring(BehaviourType type, World* world):
	Positional(type, world)
{
}

Nimbus::Soaring::Soaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Positional(type, world, initializingSettings)
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

Behaviour* Nimbus::Soaring::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Soaring(this->mWorld, initializingSettings);
}

Behaviour* Nimbus::Soaring::clone(void)
{
	return new Nimbus::Soaring(this, this-mWorld);