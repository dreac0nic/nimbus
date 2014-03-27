#include "Flocking.h"
#include "Behaviour.h"

using namespace Nimbus;

Nimbus::Flocking::Flocking(BehaviourType type, World* world):
	Behaviour(type, world)
{
	this->init();
}

Nimbus::Flocking::Flocking(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
	this->init();
}

Nimbus::Flocking::Flocking(Flocking* other, World* world, int id) :
	Behaviour(other, world, id)
{
	this->init();
}

Nimbus::Flocking::~Flocking(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::Flocking::init()
{
}

void Nimbus::Flocking::startup(void)
{
	// STARTUP FOR FLOCK Flocking
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Nimbus::Flocking::update(void)
{
	// UPDATE THE FLOCK Flocking
	/*
		Update the Flocking entity. Weeeee.
	*/
}

void Nimbus::Flocking::shutdown(void)
{
	// SUT DOWN THE FLOCK Flocking
	/*
		LOLWUT
	*/
}

Behaviour* Nimbus::Flocking::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Flocking(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Nimbus::Flocking::clone(int id)
{
	return new Nimbus::Flocking(this, this->mWorld, id);
}