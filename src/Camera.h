#ifndef NIMBUS_CAMERA_H
#define NIMBUS_CAMERA_H

#include "EventSystem/EventSystem.h"
#include <OgreVector3.h>
#include <OgreWindowEventUtilities.h>

namespace Nimbus
{
	class Camera :
		public Ogre::WindowEventListener
	{
	private:
		// Number of cameras, used to generate unique camera ids
		static int numCamera;

		// Screen margin threshold
		static Ogre::Real threshold;

		// Actual camera objects
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;

		// Positioning information
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mFacing;

		// Information for moving the camera
		Ogre::Vector3 mPositionDelta;
		int slowDown; // Hmm... this feels ugly

		// Camera sensitivity factors
		Ogre::Real accelerationFactor;
		Ogre::Real essentiallyZero;
		Ogre::Real accelerationMax;

	protected:
		// From Ogre::WindowEventListener
		void windowClosed(Ogre::RenderWindow* rw);
		bool windowClosing(Ogre::RenderWindow* rw);
		void windowFocusChange(Ogre::RenderWindow* rw);
		void windowMoved(Ogre::RenderWindow* rw);
		void windowResized(Ogre::RenderWindow* rw);

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
            std::string str();

		}* mMousePositionListener;

	public:
		Camera();
		virtual ~Camera();

		/** Initializes and add the camera.
			@param sceneMgr The scene manager used to create the camera.
		*/
		void initialize(Ogre::SceneManager* sceneMgr);

		/** Initializes and add the camera.
			@param sceneMgr The scene manager used to create the camera.
			@param position The initial position of the camera.
			@param facingVector The initial facing vector of the camera.
		*/
		void initialize(Ogre::SceneManager* sceneMgr, Ogre::Vector3* position, Ogre::Vector3* facingVector);

		/** Updates the position and rotation of the camera. */
		void update(void);

		/** Gets the Ogre::Camera. */
		Ogre::Camera* getCamera() { return mCamera; }

		/** Gets the Ogre::Viewport. */
		Ogre::Viewport* getViewport() { return mViewport; }

		/** Gets the screen threshold for movement. */
		static Ogre::Real getScreenThreshold() { return threshold; }
	};
}

#endif
