#include "FlockSoaring.h"

using namespace Nimbus;

Nimbus::FlockSoaring::FlockSoaring(BehaviourType type, World* world):
	Soaring(type, world)
{
	this->init();
}

Nimbus::FlockSoaring::FlockSoaring(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Soaring(type, world, initializingSettings)
{
	this->init();
}

Nimbus::FlockSoaring::FlockSoaring(FlockSoaring* other, World* world, int id) :
	Soaring(other, world, id)
{
	this->init();
}

Nimbus::FlockSoaring::~FlockSoaring(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::FlockSoaring::init()
{
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

Behaviour* Nimbus::FlockSoaring::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::FlockSoaring(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Nimbus::FlockSoaring::clone(int id)
{
	return new Nimbus::FlockSoaring(this, this->mWorld, id);
}