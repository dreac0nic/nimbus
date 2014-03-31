#include "Flocking.h"
#include <algorithm>

using namespace Nimbus;

Flocking::Flocking(BehaviourType type, World* world) :
	Behaviour(type, world), componentInfluenceFactor(1.0)
{
	this->init();
}

Flocking::Flocking(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings), componentInfluenceFactor(1.0)
{
	this->init();
}

Flocking::Flocking(Flocking* other, World* world, int id) :
	Behaviour(other, world, id), componentInfluenceFactor(1.0)
{
	this->init();
}

Flocking::~Flocking(void)
{
	// DESTROY ALL OF THE THINGS
	this->mSoarListener;
}

void Flocking::init()
{
	this->mSoarListener = new SoarListener(this);

	this->mPositionDelta = Ogre::Vector3::ZERO;
}

void Flocking::startup(void)
{
	// STARTUP FOR FLOCK Flocking
	/*
		NO IDEA WHAT GOES HERE
	*/
}

void Flocking::update(void)
{
	// UPDATE THE FLOCK Flocking
	/*
		Update the Flocking entity. Weeeee.
	*/
}

void Flocking::shutdown(void)
{
	// SUT DOWN THE FLOCK Flocking
	/*
		LOLWUT
	*/
}

Behaviour* Flocking::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Flocking(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Flocking::clone(int id)
{
	return new Flocking(this, this->mWorld, id);
}

void Flocking::SoarListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Return if we do not have this entity id in our entity list
	if(std::find(
		mParent->mEntities.begin(),
		mParent->mEntities.end(),
		*static_cast<int*>(payload["EntityId"])) == mParent->mEntities.end())
	{
		return;
	}

	// Default to delta of zero
	Ogre::Vector3 positionDelta = Ogre::Vector3::ZERO;

	// Get the position delta
	if(payload.find("PositionDelta") != payload.end())
	{
		positionDelta = *static_cast<Ogre::Vector3*>(payload["PositionDelta"]);
	}

	// Scale the position delta relative to the number of components
	positionDelta.normalise();
	positionDelta *= Ogre::Real(mParent->componentInfluenceFactor / mParent->mEntities.size());

	// Aggregate the position delta of this component
	mParent->mPositionDelta += positionDelta;
}