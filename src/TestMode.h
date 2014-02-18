#ifndef TESTMODE_H
#define TESTMODE_H

#include "RunMode.h"

// This might be a very bad idea, we'll have to see how widely this spreads
using namespace Ogre;

class TestMode :
	public RunMode
{
private:
	// Member variables

	// Hacky injection... figure this out asap
	RenderWindow* mWindow;

	SceneManager* mSceneMgr;
	Camera* mCamera;
	Viewport* mViewport;

protected:
	// RunMode
	virtual bool initialize();

public:
	/** Creates a TestMode type run mode.
	@param window Nuke this parameter asap... or else put it in RunMode. This is a hack
	so that the RunMode can create it's own viewport... which should be a per RunMode
	activity.
	*/
	TestMode(RenderWindow* window);
	virtual ~TestMode(void);

	/** Runs the test mode.
	*/
	virtual RunMode* run(const FrameEvent& evt);
};

#endif
