#ifndef TESTMODE_H
#define TESTMODE_H

#include "RunMode.h"

class TestMode :
	public RunMode
{
private:
	// Member variables

	// Hacky injection... figure this out asap
	Ogre::RenderWindow* mWindow;

	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;

protected:
	// RunMode
	virtual bool initialize();

public:
	/** Creates a TestMode type run mode.
	@param window Nuke this parameter asap... or else put it in RunMode. This is a hack
	so that the RunMode can create it's own viewport... which should be a per RunMode
	activity.
	*/
	TestMode(Ogre::RenderWindow* window);
	virtual ~TestMode(void);

	/** Runs the test mode.
	*/
	virtual RunMode* run(const Ogre::FrameEvent& evt);
};

#endif
