#ifndef NIMBUS_CAMERA_H
#define NIMBUS_CAMERA_H

#include "EventSystem.h"
#include <OgreVector3.h>

namespace Nimbus
{
	class Camera
	{
	private:
		// Actual camera objects
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;

		// Positioning information
		Ogre::Vector3* mPosition;
		Ogre::Vector3* mFacing;

	protected:
		// Event listeners

		/** Listens to events which should move the camera. */
		class MousePositionListener :
			public EventListener
		{
		private:
			Camera* mParent;
		
		public:
			MousePositionListener(Camera* parent) : mParent(parent) {}
			virtual ~MousePositionListener() {}

			// From Nimbus::EventListener
			void handleEvent(payloadmap payload, EventListener* responder = NULL);

		}* mMousePositionListener;

	public:
		Camera();
		virtual ~Camera();

		/** Initializes and add the camera.
			@param renderWindow Used to create the viewport to which the camera renders.
			@param sceneMgr The scene manager used to create the camera.
		*/
		void initialize(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneMgr);

		/** Initializes and add the camera.
			@param renderWindow Used to create the viewport to which the camera renders.
			@param sceneMgr The scene manager used to create the camera.
			@param position The initial position of the camera.
			@param facingVector The initial facing vector of the camera.
		*/
		void initialize(Ogre::RenderWindow* renderWindow, Ogre::SceneManager* sceneMgr, Ogre::Vector3* position, Ogre::Vector3* facingVector);

		/** Updates the position and rotation of the camera. */
		void update(void);
	};
}

#endif