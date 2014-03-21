#include "Rainy.h"

using namespace Nimbus;

Nimbus::Rainy::Rainy(BehaviourType type, World* world):
	Behaviour(type, world)
{
	this->init();
}

Nimbus::Rainy::Rainy(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
	this->init();
}

Nimbus::Rainy::Rainy(Rainy* other, World* world, int id) :
	Behaviour(other, world, id)
{
	this->init();
}

Nimbus::Rainy::~Rainy(void)
{
	// DESTROY ALL OF THE THINGS
}

void Nimbus::Rainy::init()
{
}

void Nimbus::Rainy::startup(void)
{
	// STARTUP FOR RAINY
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Nimbus::Rainy::update(void)
{
	// UPDATE THE RAINY
	/*
		POSITION THINGS YEAH
	*/
}

void Nimbus::Rainy::shutdown(void)
{
	// SUT DOWN THE RAINY STUFF
	/*
		YARHARFIDDLEEDEE
	*/
}

Behaviour* Nimbus::Rainy::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Nimbus::Rainy(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Nimbus::Rainy::clone(int id)
{
	return new Nimbus::Rainy(this, this->mWorld, id);
}