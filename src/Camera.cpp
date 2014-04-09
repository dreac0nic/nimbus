#include "Camera.h"
#include "NimbusApplication.h"
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <OgreRenderWindow.h>
#include <sstream>

#define ARBITRARY_SLOW_DOWN_COUNT 10
#define ARBITRARY_SLOW_DOWN_STARTING_VALUE_WITH_A_REALLY_LONG_NAME 0
#define NEAR_CLIP_DISTANCE 5

using namespace Nimbus;
using namespace Ogre;

int Nimbus::Camera::numCamera = 0;

Real Nimbus::Camera::threshold = 60.0;

Nimbus::Camera::Camera() :
	mPosition(Vector3::ZERO),
	mFacing(Vector3::ZERO),
	mPositionDelta(Vector3::ZERO),
	slowDown(0),
	accelerationFactor((Ogre::Real)0.2),
	accelerationMax((Ogre::Real)0.5),
	essentiallyZero((Ogre::Real)0.1)
{
	this->mMousePositionListener = new MousePositionListener(this);
}

Nimbus::Camera::~Camera()
{
	delete this->mMousePositionListener;
}

void Nimbus::Camera::initialize(SceneManager* sceneMgr)
{
	initialize(sceneMgr, &Vector3(0, 300, 0), &Vector3(0,-300,0));
}

void Nimbus::Camera::initialize(SceneManager* sceneMgr, Vector3* position, Vector3* facingVector)
{
	// Generate the camera's name
	std::stringstream cameraName;
	cameraName << "Camera" << numCamera++;

	// Store the position attributes
	this->mPosition = *position;
	this->mFacing = *facingVector;

	// Create the camera
	this->mCamera = sceneMgr->createCamera(cameraName.str());

	// Set up the initial attributes of the camera
	this->mCamera->setPosition(*position);
	this->mCamera->lookAt(*position + *facingVector);
	this->mCamera->setNearClipDistance(NEAR_CLIP_DISTANCE);

	// Create the viewport
	this->mViewport = NimbusApplication::getRenderWindow()->addViewport(this->mCamera);

	// Resize the camera to the viewport size
	this->mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));

	// Set the threshold based on the actual screen width
	//threshold = mViewport->getActualWidth() *.1;

	// Register the render window listener
	WindowEventUtilities::addWindowEventListener(NimbusApplication::getRenderWindow(), this);

	EventSystem::getSingleton()->registerListener(this->mMousePositionListener, EventSystem::EventType::MOUSE_POSITION_UPDATE);
}

void Nimbus::Camera::update(void)
{
	// Update the position vector
	mPosition += mPositionDelta;

	// If slowing down and not yet zero
	if(slowDown <= ARBITRARY_SLOW_DOWN_STARTING_VALUE_WITH_A_REALLY_LONG_NAME &&
		mPositionDelta.length() > essentiallyZero)
	{
		Vector3 tempVector = mPositionDelta.normalisedCopy();
		mPositionDelta -= mPositionDelta * accelerationFactor;
	}
	// If slowing down and pretty much zero
	else if(slowDown <= ARBITRARY_SLOW_DOWN_STARTING_VALUE_WITH_A_REALLY_LONG_NAME)
	{
		mPositionDelta = Vector3::ZERO;
	}
	// If not slowing down, start slowing down... unless overridden by input
	else
	{
		//slowDown--;
	}

	// Sync the camera to the position vectors
	this->mCamera->setPosition(mPosition);
	this->mCamera->lookAt(mPosition + mFacing);
}

void Nimbus::Camera::windowClosed(RenderWindow* rw)
{
}

bool Nimbus::Camera::windowClosing(RenderWindow* rw)
{
	return true;
}

void Nimbus::Camera::windowFocusChange(RenderWindow* rw)
{
	// STOP MOVING!!!
	this->mPositionDelta = Vector3::ZERO;
}

void Nimbus::Camera::windowMoved(RenderWindow* rw)
{
}

void Nimbus::Camera::windowResized(RenderWindow* rw)
{
	// Update the aspect ratio
	this->mCamera->setAspectRatio(Real(this->mViewport->getActualWidth())/ Real(this->mViewport->getActualHeight()));
}

void Nimbus::Camera::MousePositionListener::handleEvent(payloadmap payload, EventListener* responder)
{
	// Verify that the event is for our application context
	std::string context = *static_cast<std::string*>(payload["Context"]);
	if(context.compare("Screen") != 0)
	{
		return;
	}

	// Get the screen position
	if(payload.find("ScreenPosition") != payload.end())
	{
		Vector2 screenPosition = *static_cast<Ogre::Vector2*>(payload["ScreenPosition"]);
		Vector3 accelerationVector = Vector3::ZERO;

		// Going left
		if(screenPosition.x < threshold)
		{
			accelerationVector -= Vector3::UNIT_X;
		}
		// Going right
		else if(screenPosition.x > mParent->mViewport->getActualWidth() - threshold)
		{
			accelerationVector += Vector3::UNIT_X;
		}
		
		// Going up
		if(screenPosition.y < threshold)
		{
			accelerationVector -= Vector3::UNIT_Z;
		}
		// Going down
		else if(screenPosition.y > mParent->mViewport->getActualHeight() - threshold)
		{
			accelerationVector += Vector3::UNIT_Z;
		}

		if(accelerationVector == Vector3::ZERO)
		{
			mParent->slowDown = ARBITRARY_SLOW_DOWN_STARTING_VALUE_WITH_A_REALLY_LONG_NAME;
		}
		else
		{
			// Don't slow down since we just received acceleration input
			mParent->slowDown = ARBITRARY_SLOW_DOWN_COUNT;
		}

		// Normalize the acceleration vector
		accelerationVector.normalise();

		// Limit the acceleration of the delta
		if(mParent->mPositionDelta.length() < mParent->accelerationMax)
		{
			// Accelerate the delta
			mParent->mPositionDelta += accelerationVector * mParent->accelerationFactor * 2;
		}
	}
}
