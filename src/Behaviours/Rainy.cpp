#include "Rainy.h"
#include "../EventSystem.h"

using namespace Nimbus;

Nimbus::Rainy::Rainy(BehaviourType type, World* world, EventSystem* eventSystem):
	Behaviour(type, world, eventSystem)
{
	this->init();
}

Nimbus::Rainy::Rainy(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem):
	Behaviour(type, world, initializingSettings, eventSystem)
{
	this->init();
}

Nimbus::Rainy::Rainy(Rainy* other, World* world, int id, EventSystem* eventSystem) :
	Behaviour(other, world, id, eventSystem)
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

Behaviour* Nimbus::Rainy::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem)
{
	return new Nimbus::Rainy(this->mBehaviourType, this->mWorld, initializingSettings, eventSystem);
}

Behaviour* Nimbus::Rainy::clone(int id, EventSystem* eventSystem)
{
	return new Nimbus::Rainy(this, this->mWorld, id, eventSystem);
}