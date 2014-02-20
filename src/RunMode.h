#ifndef RUNMODE_H
#define RUNMODE_H

#include <OgreFrameListener.h>

namespace Nimbus
{
	/** Controls the operation of an application in a specific mode.
	Contains an optional initialization framework if relevant to the run mode.
	*/
	class RunMode
	{
	protected:
		// Indicates if the RunMode has been initialized.
		bool initialized;

		/** Sets up all necessary components of the run mode.
		Should be called before run begins.
		@return A boolean indicating success of initialization.
		*/
		virtual bool initialize()
		{
			initialized = true;
			return true;
		}

		/** Stops the execution of the run mode.
		Should be called after the run completes.
		@return A boolean indicating success of stopping.
		*/
		virtual bool stop()
		{
			initialized = false;
			return true;
		}

	public:
		RunMode()
		{
			initialized = false;
		}

		virtual ~RunMode() {}

		/** Updates all information in the run mode. Runs a single frame.
		@param evt An Ogre::FrameEvent storing the amount of time since the last
		frame.
		@return A pointer to the next RunMode which should be run. Will generally
		return itself until a RunMode change is necessary. If the program should exit,
		it will return a 0 (null).
		*/
		virtual RunMode* run(const Ogre::FrameEvent& evt) = 0;
	};
}

#endif
