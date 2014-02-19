#ifndef MANAGER_H
#define MANAGER_H

/** A generic manager class which features an update function.
Basically just guarantees that all managers have an update function. Makes
managers iterably updateable... which sounds complex but isn't.
*/
class Manager
{
public:
	Manager(void) {}
	virtual ~Manager(void) {}

	/** The super complex update function.
	@return A boolean indicating the success of the update function. If this
	returns false... stop the program.
	*/
	virtual bool update(void) = 0;
};

#endif
