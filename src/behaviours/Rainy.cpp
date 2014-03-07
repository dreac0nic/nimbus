#include "Rainy.h"

using namespace Nimbus;

Nimbus::Rainy::Rainy(World* world):
	Behaviour(world)
{
}

Nimbus::Rainy::Rainy(World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(world, initializingSettings)
{
}

Nimbus::Rainy::~Rainy(void)
{
	// DESTROY ALL OF THE THINGS
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
	return new Nimbus::Rainy(this->mWorld, initializingSettings);
}