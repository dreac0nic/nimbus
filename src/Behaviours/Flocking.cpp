#include "Flocking.h"
#include <algorithm>

using namespace Nimbus;
using namespace Ogre;

Flocking::Flocking(BehaviourType type, World* world) :
	Behaviour(type, world)
{
	this->init(1.0, 0.1);
}

Flocking::Flocking(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
	stringstream optionsParser;

	double influenceFactor = 1.0;
	double overrideFactor = 0.1;

	// Load the influence factor for the flocking group
	if(initializingSettings->find("influence") != initializingSettings->end())
	{
		optionsParser = stringstream(initializingSettings->find("influence")->second);
		optionsParser >> influenceFactor;
	}

	// Load the override factor for the flocking group
	if(initializingSettings->find("override") != initializingSettings->end())
	{
		optionsParser = stringstream(initializingSettings->find("override")->second);
		optionsParser >> overrideFactor;
	}

	this->init(influenceFactor, overrideFactor);
}

Flocking::Flocking(Flocking* other, World* world, int id) :
	Behaviour(other, world, id)
{
	this->init(other->componentInfluenceFactor, other->componentOverrideFactor);
}

Flocking::~Flocking(void)
{
	// DESTROY ALL OF THE THINGS
	this->mSoarListener;
}

void Flocking::init(double influenceFactor, double overrideFactor)
{
	// Initialize constants
	mComponentInfluenceFactor = influenceFactor;
	mComponentOverrideFactor = overrideFactor;

	// Initial delta is zero
	this->mPositionDelta = Ogre::Vector3::ZERO;

	// Create the listener(s)
	this->mSoarListener = new SoarListener(this);	
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
	// Get the entity id
	GameEntityId entityId = *static_cast<GameEntityId*>(payload["EntityId"]);

	// Return if we do not have this entity id in our entity list
	if(std::find(
		mParent->mEntities.begin(),
		mParent->mEntities.end(),
		entityId) == mParent->mEntities.end())
	{
		return;
	}

	// Default to delta of zero
	Ogre::Vector3 positionDelta = Ogre::Vector3::ZERO;

	// Get the position delta
	if(payload.find("PositionDelta") != payload.end())
	{
		// Get the component entity's delta
		positionDelta = *static_cast<Ogre::Vector3*>(payload["PositionDelta"]);

		// Store the component delta
		mParent->mEntities[entityId] = positionDelta;

		// Scale the position delta relative to the number of components
		positionDelta.normalise();
		positionDelta *= Ogre::Real(mParent->mComponentInfluenceFactor / mParent->mEntities.size());
	}

	// Aggregate the position delta of this component
	mParent->mPositionDelta += positionDelta;
}