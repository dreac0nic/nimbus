#ifndef WIND_CURRENT_H
#define WIND_CURRENT_H

#include <list>
#include <OgreRoot.h>

namespace Nimbus
{
	/** Type for storing a wind current path.
		Contains both position and direction/influence.
	*/
	typedef std::list<std::pair<Ogre::Vector2, Ogre::Vector2> > pathList;

	/** Stores a path representing wind input
		Used by WindMap
	*/
	class WindCurrent
	{
	private:
		/// Indicates if the wind current is temporary
		bool mTemporary;

		/// The amount of time the wind current has existed
		int mTimeAlive;

		/// The strength of the wind current
		Ogre::Real mStrength;

		/** A list of the influence vectors along a wind current.
			Contains pairs of position vectors with corresponding
			direction/influence vectors
		*/
		pathList mPath;

	public:
		WindCurrent(Ogre::Real strength, bool temporary = false);
		virtual ~WindCurrent(void);

		/** Adds a point to the wind current.

			@param position The position of the point to add to the wind current
			@param influence The influence of the point to add to the wind current
		*/
		void addPoint(Ogre::Vector2 position, Ogre::Vector2 influence);

		/** Increments the time that this wind current has been alive.
		*/
		void incrementTimer();

		/** Gets the path of this wind current.
			
			@return A pathList object containing the path of the wind current.
		*/
		pathList getPath();

		/** Gets the strength of this wind current.
			
			@return An Ogre::Real representing the strength of the wind current.
		*/
		Ogre::Real getStrength();

		/** True, then temporary, false, then permanent.

			@return A bool indicating whether or not this wind current is temporary.
		*/
		bool isTemporary();

		/** Returns the time that this timer has been alive.

			@return An int containing the number of timer increments since creation.
		*/
		int getTimeAlive();

		bool operator==(const WindCurrent &other) const;
		bool operator!=(const WindCurrent &other) const;
	};
}
#endif
