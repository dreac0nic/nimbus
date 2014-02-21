#ifndef TESTMODE_H
#define TESTMODE_H

#include "RunMode.h"

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
		// RunMode
		virtual bool initialize();

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
