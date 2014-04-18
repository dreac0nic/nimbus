#include "Transformational.h"

using namespace Nimbus;
using namespace Ogre;

Transformational::Transformational(BehaviourType type, World* world, EventSystem* eventSystem):
	Behaviour(type, world, eventSystem)
{
	this->init(Vector3::ZERO, Vector3::ZERO, Vector3::UNIT_SCALE);
}

Transformational::Transformational(BehaviourType type, World* world, Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem):
	Behaviour(type, world, initializingSettings, eventSystem)
{
	stringstream optionParser;
	Vector3 initialPosition = Vector3::ZERO;
	Vector3 facingVector = Vector3::ZERO;
	Vector3 scaleVector = Vector3::UNIT_SCALE;

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

	// Get the scale vector
	if(initializingSettings->find("scale") != initializingSettings->end())
	{
		optionParser = stringstream(initializingSettings->find("scale")->second);
		optionParser >> scaleVector.x >> scaleVector.y >> scaleVector.y;
	}

	this->init(initialPosition, facingVector, scaleVector);
}

Transformational::Transformational(Transformational* other, World* world, int id, EventSystem* eventSystem):
	Behaviour(other, world, id, eventSystem)
{
	this->init(other->mPosition, other->mFacingVector, other->mScale);
}

Transformational::~Transformational(void)
{
	// DESTROY ALL OF THE THINGS
	delete this->mTranslationListener;
	delete this->mTranslationQueryListener;
}

void Transformational::init(Vector3 initialPosition, Vector3 facingDirection, Vector3 scale)
{
	this->mPosition = initialPosition;
	this->mFacingVector = facingDirection;
	this->mScale = scale;

	// Would be nice to calculate this based on the facing vector
	this->mRotationVector = Vector3::ZERO;

	this->mTranslationListener = new TranslationListener(this);
	this->mTranslationQueryListener = new TranslationQueryListener(this);
}

void Transformational::startup(void)
{
	// STARTUP FOR TRANSLATIONAL
	this->mDeltaPosition = Vector3::ZERO;
	this->mDeltaRotation = Vector3::ZERO;
	this->mNewFacing = Vector3::ZERO;

	// Register event listeners.
	filtermap entityFilter;
	entityFilter["EntityId"] = &this->mParentId;

	this->mEntityEventSystem->registerListener(mTranslationListener, EventSystem::EventType::TRANSLATE_ENTITY, entityFilter);
	this->mEntityEventSystem->registerListener(mTranslationQueryListener, EventSystem::EventType::TRANSLATION_QUERY, entityFilter);

	// Force an update when things start rendering
	this->forceUpdate();
}

void Transformational::update(void)
{
	if(this->requireUpdate)
	{
		payloadmap initialMovePayload;

		// Send off an initial move entity event so that renderable can (hopefully) update...
		// It just occured to me that renderable may not yet be registered for events... that could be a problem.
		initialMovePayload["EntityId"] = &this->mParentId;
		initialMovePayload["PositionVector"] = &this->mPosition;
		initialMovePayload["FacingVector"] = &this->mFacingVector;

		this->mEntityEventSystem->fireEvent(EventSystem::EventType::ENTITY_TRANSLATED, initialMovePayload);

		this->requireUpdate = false;
	}

	// UPDATE THE TRANSLATIONAL
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
		this->mEntityEventSystem->fireEvent(EventSystem::EventType::ENTITY_TRANSLATED, eventMovePayload);

		// Reset the delta vectors
		this->mNewFacing = Vector3::ZERO;
	}
}

void Transformational::shutdown(void)
{
	// SUT DOWN THE TRANSLATIONAL STUFF
	// Unregister event listeners.
	filtermap entityFilter;
	entityFilter["EntityId"] = &this->mParentId;

	this->mEntityEventSystem->unregisterListener(mTranslationListener, EventSystem::EventType::TRANSLATE_ENTITY, entityFilter);
	this->mEntityEventSystem->unregisterListener(mTranslationQueryListener, EventSystem::EventType::TRANSLATION_QUERY, entityFilter);
}

void Transformational::forceUpdate(void)
{
	this->requireUpdate = true;
}

Behaviour* Transformational::clone(Ogre::ConfigFile::SettingsMultiMap* initializingSettings, EventSystem* eventSystem)
{
	return new Transformational(this->mBehaviourType, this->mWorld, initializingSettings, eventSystem);
}

Behaviour* Transformational::clone(int id, EventSystem* eventSystem)
{
	return new Transformational(this, this->mWorld, id, eventSystem);
}

void Transformational::TranslationListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Return as fast as possible if this event isn't for me
	if((*static_cast<int*>(payload["EntityId"])) != this->parent->mParentId)
	{
		return;
	}

	Vector3* movementVector, *rotationVector, *facingVector;

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

void Transformational::TranslationQueryListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Make sure the request is for us
	if(mParent->mParentId != *static_cast<GameEntityId*>(payload["EntityId"]))
	{
		return;
	}

	// If there is a responder
	if(responder != NULL)
	{
		// Send it all positional information
		payloadmap returnPayload;
		returnPayload["PositionVector"] = &mParent->mPosition;
		returnPayload["FacingVector"] = &mParent->mFacingVector;
		returnPayload["RotationVector"] = &mParent->mRotationVector;
		responder->handleEvent(returnPayload);
	}
}