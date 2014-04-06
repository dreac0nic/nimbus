#ifndef TESTMODE_H
#define TESTMODE_H

#include "RunMode.h"
#include "EventSystem.h"

namespace Nimbus
{
	class TestMode :
		public RunMode
	{
	private:
		// Member variables
		Ogre::SceneManager* mSceneMgr;
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;

	protected:
		// From Nimbus::RunMode
		virtual bool initialize();

		/** Listens for the key event
		*/
		class KeyListener : 
			public EventListener
		{
		public:
			KeyListener() {}
			virtual ~KeyListener() {}

			// From Nimbus::EventListener
			virtual void handleEvent(payloadmap payload);
		} *keyListener;

	public:
		/** Creates a TestMode type run mode.
		 */
		TestMode(void);
		virtual ~TestMode(void);

		/** Runs the test mode.
		 */
		virtual RunMode* run(const Ogre::FrameEvent& evt);
	};
}

#endif
