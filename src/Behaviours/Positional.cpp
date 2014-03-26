#include <OgreLogManager.h>

#include "Positional.h"

using namespace Nimbus;
using namespace Ogre;

Positional::Positional(BehaviourType type, World* world):
	Behaviour(type, world)
{
	this->init(Vector3::ZERO, Vector3::ZERO);
}

Positional::Positional(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings):
	Behaviour(type, world, initializingSettings)
{
	stringstream optionParser;
	Vector3 initialPosition = Vector3::ZERO;
	Vector3 facingVector = Vector3::ZERO;

	// Get the position vector
	if(initializingSettings->find("position") != initializingSettings->end())
	{
		optionParser = stringstream(initializingSettings->find("position")->second);
		optionParser >> initialPosition.x >> initialPosition.y >> initialPosition.z;
	}

	// Get the facing vector
	if(initializingSettings->find("face") != initializingSettings->end())
	{
		optionParser = stringstream(initializingSettings->find("face")->second);
		optionParser >> facingVector.x >> facingVector.y >> facingVector.z;
		stringstream temp;
		temp << "(Nimbus) Facing vector (" << facingVector.x << ", " << facingVector.y << ", " << facingVector.z << ")";
		Ogre::LogManager::getSingleton().logMessage(temp.str());
	}

	this->init(initialPosition, facingVector);
}

Positional::Positional(Positional* other, World* world, int id):
	Behaviour(other, world, id)
{
	this->init(other->mPosition, other->mFacingVector);
}

Positional::~Positional(void)
{
	// DESTROY ALL OF THE THINGS
	delete this->mMovementListener;
}

void Positional::init(Vector3 initialPosition, Vector3 facingDirection)
{
	this->mPosition = initialPosition;
	this->mFacingVector = facingDirection;

	// Would be nice to calculate this based on the facing vector
	this->mRotationVector = Vector3::ZERO;

	this->mMovementListener = new MovementListener(this);
}

void Positional::startup(void)
{
	// STARTUP FOR POSITIONAL
	this->mDeltaPosition = Vector3::ZERO;
	this->mDeltaRotation = Vector3::ZERO;
	this->mNewFacing = Vector3::ZERO;

	// Register event listeners.
	EventSystem::getSingleton()->registerListener(mMovementListener, EventSystem::EventType::POSITION_ENTITY);
	EventSystem::getSingleton()->registerListener(mMovementListener, EventSystem::EventType::BEGIN_TRANSLATE_ENTITY);
	EventSystem::getSingleton()->registerListener(mMovementListener, EventSystem::EventType::END_TRANSLATE_ENTITY);

	// Force an update when things start rendering
	this->forceUpdate();
}

void Positional::update(void)
{
	if(this->requireUpdate)
	{
		payloadmap initialMovePayload;

		// Send off an initial move entity event so that renderable can (hopefully) update...
		// It just occured to me that renderable may not yet be registered for events... that could be a problem.
		initialMovePayload["EntityId"] = &this->mParentId;
		initialMovePayload["PositionVector"] = &this->mPosition;
		initialMovePayload["FacingVector"] = &this->mFacingVector;

		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::ENTITY_MOVED, initialMovePayload);

		this->requireUpdate = false;
	}

	// UPDATE THE POSITIONAL
	if(this->mDeltaPosition != Vector3::ZERO
		|| this->mDeltaRotation != Vector3::ZERO
		|| this->mNewFacing != Vector3::ZERO) {
		payloadmap eventMovePayload;

		// Update the position if necessary
		if(this->mDeltaPosition != Vector3::ZERO)
		{
			this->mPosition += this->mDeltaPosition;
			eventMovePayload["PositionVector"] = &this->mPosition;
		}

		// Update the rotation if necessary
		if(this->mDeltaRotation != Vector3::ZERO)
		{
			this->mRotationVector += this->mDeltaRotation;
			eventMovePayload["RotationVector"] = &this->mRotationVector;
		}
		
		// Update the facing vector if necessary
		if(this->mNewFacing != Vector3::ZERO)
		{
			this->mFacingVector = this->mNewFacing;
			eventMovePayload["FacingVector"] = &this->mFacingVector;
		}

		// Send off the movement event
		eventMovePayload["EntityId"] = &this->mParentId;
		EventSystem::getSingleton()->fireEvent(EventSystem::EventType::ENTITY_MOVED, eventMovePayload);

		// Reset the delta vectors
		this->mNewFacing = Vector3::ZERO;
	}
}

void Positional::shutdown(void)
{
	// SUT DOWN THE POSITIONAL STUFF
	// Deregister event listeners.
	EventSystem::getSingleton()->unregisterListener(mMovementListener, EventSystem::EventType::POSITION_ENTITY);
	EventSystem::getSingleton()->unregisterListener(mMovementListener, EventSystem::EventType::BEGIN_TRANSLATE_ENTITY);
	EventSystem::getSingleton()->unregisterListener(mMovementListener, EventSystem::EventType::END_TRANSLATE_ENTITY);
}

void Positional::forceUpdate(void)
{
	this->requireUpdate = true;
}

Behaviour* Positional::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings)
{
	return new Positional(this->mBehaviourType, this->mWorld, initializingSettings);
}

Behaviour* Positional::clone(int id)
{
	return new Positional(this, this->mWorld, id);
}

void Positional::MovementListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Return as fast as possible if this event isn't for me
	if((*static_cast<int*>(payload["EntityId"])) != this->parent->mParentId)
	{
		return;
	}

	Vector3* movementVector, *rotationVector, *facingVector;

	// Check to see if this is a cancel movement event
	if(payload.find("EndTranslate") != payload.end())
	{
		parent->mDeltaPosition = Vector3::ZERO;
		parent->mDeltaRotation = Vector3::ZERO;

		return;
	}

	// If the event specifies an absolute position
	if(payload.find("PositionVector") != payload.end())
	{
		movementVector = static_cast<Ogre::Vector3*>(payload["PositionVector"]);

		// Directly sets the position
		parent->mPosition = *movementVector;

		// The position's changed, so update
		parent->forceUpdate();
	}
	// If the event specifies a relative position
	else if(payload.find("PositionDelta") != payload.end())
	{
		movementVector = static_cast<Ogre::Vector3*>(payload["PositionDelta"]);

		// Add the delta to the current position delta as part of aggregating moves in a frame
		parent->mDeltaPosition = *movementVector;
	}

	// If the event specifies an absolute rotation
	if(payload.find("RotationVector") != payload.end())
	{
		rotationVector = static_cast<Ogre::Vector3*>(payload["RotationVector"]);

		// Directly set the rotation
		parent->mRotationVector = *rotationVector;

		// The rotation's changed, so update
		parent->forceUpdate();
	}
	// If the event specifies a relative rotation
	else if(payload.find("RotationDelta") != payload.end())
	{
		rotationVector = static_cast<Ogre::Vector3*>(payload["RotationDelta"]);

		// Add the delta to the current rotation delta as part of aggregating rotations in a frame
		parent->mDeltaRotation = *rotationVector;
	}

	// If the event specifies a facing vector
	if(payload.find("FacingVector") != payload.end())
	{
		facingVector = static_cast<Ogre::Vector3*>(payload["FacingVector"]);

		// Set the new facing vector... there is no facing vector aggregation. Last one wins.
		parent->mNewFacing = *facingVector;
	}
}
