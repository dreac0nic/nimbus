#include "Soaring.h"
#include <OgreVector3.h>

using namespace Nimbus;

Soaring::Soaring(BehaviourType type, World* world):
	Behaviour(type, world)
{
}

Soaring::Soaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
}

Soaring::Soaring(Soaring* other, World* world, int id) :
	Behaviour(other, other->mWorld, id)
{
}

Soaring::~Soaring(void)
{
	// DESTROY ALL OF THE THINGS
}

void Soaring::init(void)
{
}

void Soaring::startup(void)
{
	// STARTUP FOR FLOCK SOARING
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Soaring::update(void)
{
	// UPDATE THE FLOCK SOARING
	/*
		Update the soaring entity. Weeeee.
	*/
}

void Soaring::shutdown(void)
{
	// SUT DOWN THE FLOCK SOARING
	/*
		LOLWUT
	*/
}

Behaviour* Soaring::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Soaring(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Soaring::clone(int id)
{
	return new Soaring(this, this->mWorld, id);
}