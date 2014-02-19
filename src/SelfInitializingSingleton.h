#ifndef SELFINITIALIZINGSINGLETON_H
#define SELFINITIALIZINGSINGLETON_H

/** A singleton base class which, if necessary, initializes itself upon
request.

Note: Because of how this class functions, a default constructor must be
available for class T. However, the singleton may be initialized to any desired
object by using the initialize() function.
*/
template <class T>
class SelfInitializingSingleton
{
private:
	// Singleton variable
	static T* singleton;

protected:
	/** Initializes the singleton to a specific instance of type T.
	@param instance A pointer to the instance of type T.
	*/
	static void initialize(T* instance)
	{
		// Make certain the instance is not null
		if(!instance == 0)
		{
			singleton = instance;
		}
	}

public:
	virtual ~SelfInitializingSingleton() {}

	/** Accessor method for the singleton.
	@return The singleton object, initializing it if necessary
	*/
	static T& getSingleton()
	{
		return *getSingletonPtr();
	}

	/** Accessor method for the singleton.
	(This requires that the child class have a public constructor... which kind of
	defeats the purpose of a singleton class... need to reconsider this.)
	@return A pointer to the singleton object, initializing it if necessary
	*/
	static T* getSingletonPtr()
	{
		if(singleton == 0)
		{
			singleton = new T();
		}

		return singleton;
	}
};

// Initialize the singleton object using template magic
template<class T>
T* SelfInitializingSingleton<T>::singleton = 0;

#endif
